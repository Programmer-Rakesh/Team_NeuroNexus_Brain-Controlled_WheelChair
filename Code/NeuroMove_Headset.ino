#include <WiFiS3.h>
#include <WiFiUdp.h>
#include "arm_math.h"
#include <math.h>

// Wi-Fi AP Credentials & UDP
char ssid[] = "UnoR4_Network";
char pass[] = "password123";
WiFiUDP udp;
const unsigned int localUdpPort = 12345;
IPAddress receiverIP(192, 168, 4, 2);
const unsigned int receiverPort = 12345;

// PIN DEFINITIONS & PARAMETERS
#define EEG_PIN       A1
#define EOG_PIN       A2
#define LED_LEFT      3
#define LED_RIGHT     4
#define LED_BUILTIN   13
#define SAMPLE_RATE   512
#define FFT_SIZE      256
#define BAUD_RATE     115200

#define EEG_FORWARD_THRESHOLD 30.0
#define EEG_STOP_THRESHOLD    18.0
#define EOG_BASELINE          512
#define EOG_LEFT_THRESHOLD_LOW      250
#define EOG_LEFT_THRESHOLD_HIGH     260
#define EOG_RIGHT_THRESHOLD_HIGH   -250
#define EOG_RIGHT_THRESHOLD_LOW    -270
#define EOG_DEBOUNCE_MS       500
#define TURN_DURATION_MS      650
#define SMOOTHING_SAMPLES     7

const float EPS = 1e-6f;
#define BETA_LOW  13
#define BETA_HIGH 30

typedef struct {
  float beta;
  float total;
} BandpowerResults;

float inputBuffer_EEG[FFT_SIZE];
float fftOutputBuffer[FFT_SIZE];
float powerSpectrum[FFT_SIZE / 2];
arm_rfft_fast_instance_f32 S;
    
volatile uint16_t sampleIndex = 0;
volatile bool bufferReady = false;
int eogBuffer[SMOOTHING_SAMPLES];
int bufferIndexEOG = 0;
bool bufferFilledEOG = false;

unsigned long lastEOGCommand = 0;
unsigned long turnStartTime = 0;
bool isTurning = false;
int currentDirection = 0;
bool isMovingForward = false;

// FILTERS
float Notch(float input) {
  float output = input;
  {
    static float z1 = 0, z2 = 0;
    float x = output - (-1.56858163f * z1) - (0.96424138f * z2);
    output = 0.96508099f * x + (-1.56202714f * z1) + (0.96508099f * z2);
    z2 = z1; z1 = x;
  }
  {
    static float z1 = 0, z2 = 0;
    float x = output - (-1.61100358f * z1) - (0.96592171f * z2);
    output = 1.0f * x + (-1.61854514f * z1) + (1.0f * z2);
    z2 = z1; z1 = x;
  }
  return output;
}

float EEGFilter(float input) {
  float output = input;
  {
    static float z1 = 0, z2 = 0;
    float x = output - -1.22465158f * z1 - 0.45044543f * z2;
    output = 0.05644846f * x + 0.11289692f * z1 + 0.05644846f * z2;
    z2 = z1; z1 = x;
  }
  return output;
}

// BANDPOWER CALCULATION
BandpowerResults calculateBandpower(float *ps, float binRes, uint16_t halfSize) {
  BandpowerResults bp = {0};
  for (uint16_t i = 1; i < halfSize; i++) {
    float f = i * binRes;
    float p = ps[i];
    bp.total += p;
    if (f >= BETA_LOW && f < BETA_HIGH) bp.beta += p;
  }
  return bp;
}

// EOG SMOOTHING
int getSmoothedEOG() {
  eogBuffer[bufferIndexEOG] = analogRead(EOG_PIN);
  bufferIndexEOG = (bufferIndexEOG + 1) % SMOOTHING_SAMPLES;
  if (!bufferFilledEOG && bufferIndexEOG == 0) bufferFilledEOG = true;
  if (!bufferFilledEOG) return eogBuffer[0];
  long sum = 0;
  for (int i = 0; i < SMOOTHING_SAMPLES; i++) sum += eogBuffer[i];
  return sum / SMOOTHING_SAMPLES;
}

// SEND COMMAND VIA UDP (FAST, WITH TIMESTAMP)
void sendCommand(const char* cmd) {
  unsigned long sendTime = millis();
  udp.beginPacket(receiverIP, receiverPort);
  udp.write(cmd);
  udp.endPacket();
  Serial.print("Sent: ");
  Serial.print(cmd);
  Serial.print(" at ");
  Serial.println(sendTime);
}

// PROCESS EOG WITH COMMAND
void processEOG() {
  int smoothedEOG = getSmoothedEOG();
  int eogSignal = smoothedEOG - EOG_BASELINE;
  unsigned long now = millis();

  if (isTurning) {
    if (now - turnStartTime >= TURN_DURATION_MS) {
      sendCommand("0");
      isTurning = false;
      currentDirection = 0;
      digitalWrite(LED_LEFT, LOW);
      digitalWrite(LED_RIGHT, LOW);
      digitalWrite(LED_BUILTIN, LOW);
    }
    return;
  }

  if (now - lastEOGCommand < EOG_DEBOUNCE_MS) return;

  if (eogSignal > EOG_LEFT_THRESHOLD_LOW && eogSignal < EOG_LEFT_THRESHOLD_HIGH) {
    sendCommand("2");
    digitalWrite(LED_LEFT, HIGH);
    digitalWrite(LED_RIGHT, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    isTurning = true;
    currentDirection = 1;
    turnStartTime = now;
    lastEOGCommand = now;
  }
  else if (eogSignal < EOG_RIGHT_THRESHOLD_HIGH && eogSignal > EOG_RIGHT_THRESHOLD_LOW) {
    sendCommand("3");
    digitalWrite(LED_LEFT, LOW);
    digitalWrite(LED_RIGHT, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    isTurning = true;
    currentDirection = -1;
    turnStartTime = now;
    lastEOGCommand = now;
  }
}

// PROCESS EEG WITH COMMAND
void processEEG() {
  arm_rfft_fast_f32(&S, inputBuffer_EEG, fftOutputBuffer, 0);
  uint16_t halfSize = FFT_SIZE / 2;
  for (uint16_t i = 0; i < halfSize; i++) {
    float real = fftOutputBuffer[2 * i];
    float imag = fftOutputBuffer[2 * i + 1];
    powerSpectrum[i] = real * real + imag * imag;
  }
  float binRes = (float)SAMPLE_RATE / FFT_SIZE;
  BandpowerResults bp = calculateBandpower(powerSpectrum, binRes, halfSize);
  float bandPower = bp.beta / (bp.total + EPS) * 100.0;

  if (!isTurning) {
    if (bandPower > EEG_FORWARD_THRESHOLD && !isMovingForward) {
      sendCommand("01");
      digitalWrite(LED_RIGHT, HIGH);
      digitalWrite(LED_LEFT, HIGH);
      digitalWrite(LED_BUILTIN, HIGH);
      isMovingForward = true;
    }
    else if (bandPower < EEG_STOP_THRESHOLD && isMovingForward) {
      sendCommand("0");
      digitalWrite(LED_RIGHT, LOW);
      digitalWrite(LED_LEFT, LOW);
      digitalWrite(LED_BUILTIN, LOW);
      isMovingForward = false;
    }
  }
}

// SETUP
void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(EEG_PIN, INPUT);
  pinMode(EOG_PIN, INPUT);
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 0; i < SMOOTHING_SAMPLES; i++) eogBuffer[i] = EOG_BASELINE;
  arm_rfft_fast_init_f32(&S, FFT_SIZE);

  Serial.println("Setting up Access Point...");
  int status = WiFi.beginAP(ssid, pass);
  while (status != WL_AP_LISTENING) {
    delay(500);
    status = WiFi.beginAP(ssid, pass);
  }
  Serial.println("\nAccess Point started.");
  Serial.print("IP Address (UNO R4 AP): ");
  Serial.println(WiFi.localIP());

  udp.begin(localUdpPort);
  Serial.println("=== Brain-Controlled Wheelchair Sender Ready (AP Mode) ===");
}

// MAIN LOOP
void loop() {
  processEOG();

  static unsigned long lastMicros = 0;
  unsigned long nowMicros = micros();
  unsigned long dt = nowMicros - lastMicros;
  lastMicros = nowMicros;

  static long timer = 0;
  timer -= dt;

  if (timer < 0) {
    timer += 1000000 / SAMPLE_RATE;
    int rawEEG = analogRead(EEG_PIN);
    float filtEEG = EEGFilter(Notch(rawEEG));
    if (sampleIndex < FFT_SIZE) {
      inputBuffer_EEG[sampleIndex++] = filtEEG;
    }
    if (sampleIndex >= FFT_SIZE) {
      bufferReady = true;
    }
  }

  if (bufferReady) {
    processEEG();
    sampleIndex = 0;
    bufferReady = false;
  }
}
