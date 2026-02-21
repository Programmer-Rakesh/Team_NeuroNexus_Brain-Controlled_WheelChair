#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "UnoR4_Network";
const char* password = "password123";
WiFiUDP udp;
const unsigned int localPort = 12345;

#define IN1  25
#define IN2  26
#define IN3  27
#define IN4  14
#define ENA  32
#define ENB  33

// Ultrasonic Pins
#define TRIG_FRONT 5
#define ECHO_FRONT 18
#define TRIG_DOWN 19
#define ECHO_DOWN 21

int motorSpeed = 120;
unsigned long lastReconnectAttempt = 0;

// Safety thresholds
int obstacleDistance = 10;   // cm
int ditchDistance = 25;      // cm

// ================= MOTOR FUNCTIONS =================
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void forward() {
  Serial.println(">>> MOVING FORWARD <<<");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnLeft() {
  Serial.println("Confirmed: Turning Left");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnRight() {
  Serial.println("Confirmed: Turning Right");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void backward() {
  Serial.println("Confirmed: Moving Backward");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

// ================= ULTRASONIC FUNCTION =================
long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = (duration == 0) ? 999 : (duration * 0.034 / 2);
  return distance;
}

// ================= WIFI CONNECT =================
void connectWiFi() {
  Serial.println("\nConnecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  udp.begin(localPort);
  Serial.println("UDP started.");
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_DOWN, OUTPUT);
  pinMode(ECHO_DOWN, INPUT);
  
  stopMotors();
  connectWiFi();
}

// ================= LOOP =================
void loop() {
  // WiFi reconnect
  if (WiFi.status() != WL_CONNECTED) {
    if (millis() - lastReconnectAttempt > 5000) {
      Serial.println("WiFi lost. Reconnecting...");
      lastReconnectAttempt = millis();
      connectWiFi();
    }
    return;
  }

  // Read ultrasonic sensors
  long frontDist = readUltrasonic(TRIG_FRONT, ECHO_FRONT);
  long downDist = readUltrasonic(TRIG_DOWN, ECHO_DOWN);
  
  bool safetyTriggered = false;

  // Safety Check (sets flag but doesn't stop motors)
  if (frontDist > 0 && frontDist < obstacleDistance) {
    Serial.print("CRITICAL: Obstacle at "); 
    Serial.print(frontDist); 
    Serial.println("cm.");
    safetyTriggered = true;
  } 
  else if (downDist > ditchDistance) {
    Serial.print("CRITICAL: Ditch at "); 
    Serial.print(downDist); 
    Serial.println("cm.");
    safetyTriggered = true;
  }

  // UDP Control - Safety ONLY for FORWARD
  int packetLen = udp.parsePacket();
  if (packetLen > 0) {
    unsigned long receiveTime = millis();
    if (packetLen == 1 || packetLen == 2) {
      char incomingPacket[3];
      int len = udp.read(incomingPacket, packetLen);
      incomingPacket[len] = 0;

      Serial.print("Received: ");
      Serial.print(incomingPacket);
      Serial.print(" at ");
      Serial.println(receiveTime);

      if (strcmp(incomingPacket, "01") == 0 || incomingPacket[0] == '0' || incomingPacket[0] == '1' || 
          incomingPacket[0] == '2' || incomingPacket[0] == '3') {
        
        if (incomingPacket[0] == '1' || strcmp(incomingPacket, "01") == 0) {
          // FORWARD - Check safety FIRST
          if (safetyTriggered) {
            Serial.println("Blocked by safety!");
          } else {
            forward();
          }
        } 
        else if (incomingPacket[0] == '2') {
          turnLeft();  // No safety check - always allowed
        }
        else if (incomingPacket[0] == '3') {
          turnRight(); // No safety check - always allowed
        }
        else if (incomingPacket[0] == '0') {
          stopMotors();
        }
      }
    }
  }
  
  delay(50);
}
