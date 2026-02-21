# 🧠🦽 NeuroMove – Brain-Controlled Wheelchair | Hybrid BCI Healthcare Project

**Individual + Team Hardware Project – Rakesh Roy**  
B.Tech ECE | Robotics • Brain-Computer Interface • Embedded Systems • Edge AI  

---

## 📌 Project Overview
**NeuroMove** is a full-scale **Brain-Controlled Wheelchair (BCI-Based Mobility System)** designed for individuals with severe motor impairments, paralysis, or limited mobility.

Unlike conventional electric wheelchairs that require joystick or physical interaction, NeuroMove enables **thought-based navigation using EEG signals**, enhanced with **EOG (eye movement) integration** for improved accuracy and safety.

This system is designed to be:
- ✅ Non-invasive  
- ✅ Affordable  
- ✅ Real-time responsive  
- ✅ Deployment-oriented (Adult-size wheelchair)  

---

## 📸 Project Visuals (Overall)

<p align="center">
  <img src="https://github.com/Programmer-Rakesh/Team_NeuroNexus_Brain-Controlled_WheelChair/blob/main/Media/WhatsApp%20Image%202026-02-21%20at%2010.23.27%20AM.jpeg" width="400">
</p>

<!-- IMAGE PLACEHOLDER – BCI SETUP + ELECTRODE PLACEMENT -->

<p align="center">
  <img src="https://github.com/Programmer-Rakesh/Team_NeuroNexus_Brain-Controlled_WheelChair/blob/main/Media/WhatsApp%20Image%202026-02-21%20at%2010.22.51%20AM.jpeg" width="320" height="240">
  <img src="https://github.com/Programmer-Rakesh/Team_NeuroNexus_Brain-Controlled_WheelChair/blob/main/Media/WhatsApp%20Image%202026-02-21%20at%2010.22.52%20AM.jpeg" width="320" height="240">
</p>

---

## 🎯 Problem Statement

### 🚨 The Challenge
- Over **75 million people worldwide** require wheelchairs.
- In India alone, approximately **12 million people** are affected by locomotor disabilities.
- Nearly **40% of powered wheelchair users struggle with steering tasks** such as navigating doorways.
- Individuals with paralysis cannot operate joystick-based systems.

### ❗ Core Problem
How can we design a **non-invasive, affordable, real-time brain-controlled mobility solution** for people who cannot use traditional wheelchairs?

---

## 💡 Our Solution – NeuroMove

NeuroMove is a **Hybrid BCI Wheelchair System** that uses:

- 🧠 **EEG (Brain Signals)** for intention detection
- 👁 **EOG (Eye Signals)** for accuracy and validation
- ⚙️ Smart signal fusion algorithms
- 🔋 Real-time motor driver control

> “NeuroMove is more than a wheelchair — it’s freedom powered by the mind.”

---

## 🧠 Core Technologies Used

### 🔧 Hardware
- BioAmp EXG Pill (EEG + EOG Acquisition)
- Adult-Size Electric Wheelchair Chassis
- Microcontroller (Arduino / STM32)
- Motor Driver Modules
- Rechargeable Battery Pack
- Gel Electrodes (Non-Invasive)

### 💻 Software & Tools
- Python (Signal Processing & Classification)
- Embedded C/C++
- Custom Signal Filtering Algorithms
- Real-Time Data Interpretation Pipeline
- Serial Debugging & Monitoring

---

## 🏗 System Architecture

<!-- IMAGE PLACEHOLDER – BLOCK DIAGRAM -->

### Signal Flow:

Brain (EEG) + Eye (EOG)  
↓  
BioAmp EXG Signal Acquisition  
↓  
Signal Filtering & Noise Reduction  
↓  
Hybrid Signal Fusion Algorithm  
↓  
Command Mapping (Forward / Left / Right / Stop)  
↓  
Motor Driver  
↓  
Wheelchair Movement  

---

## 🔬 Working Principle

### Step 1 – Signal Acquisition
- EEG electrodes capture brain wave patterns (Focus / Intent)
- EOG captures eye movement (Left / Right confirmation)

### Step 2 – Smart Filtering
- Removal of noise and environmental interference
- Artifact rejection (muscle movement, blink noise)

### Step 3 – Hybrid Fusion Logic
- EEG determines intention
- EOG confirms direction
- Combined logic reduces false triggers

### Step 4 – Execution
- Control signals sent to motor drivers
- Real-time movement execution

---

## 🔄 Development Phases

---

### 🔹 Phase 1 – Signal Testing & Noise Analysis
**Status:** Completed ✅

- Captured raw EEG signals
- Observed noise patterns
- Implemented initial filtering pipeline

**Observations:**
- EEG signals highly sensitive to movement
- Noise mitigation required advanced filtering

---

### 🔹 Phase 2 – Hybrid EEG + EOG Integration
**Status:** Completed ✅

- Added EOG for directional confirmation
- Improved control reliability
- Reduced false movements

**Results:**
- Increased command accuracy
- Improved real-time responsiveness

---

### 🔹 Phase 3 – Motor Integration & Real-World Testing
**Status:** Prototype Ready ✅

- Integrated with adult-size wheelchair chassis
- Real-time control achieved
- Stable Forward / Left / Right movement

**Challenges:**
- Signal noise in crowded environments
- Calibration variability between users

---

## 🧪 Prototype

<!-- IMAGE PLACEHOLDER – PROTOTYPE DEMO IMAGE -->

<p align="center">
  <img src="YOUR_IMAGE_LINK_HERE" width="650">
</p>

### MVP Estimated Cost:
~₹30,000 (Affordable compared to high-end BCI systems)

---

## 🚀 Key Features

- 🧠 Thought-Based Navigation
- 👁 Hybrid EEG + EOG Control
- ⚡ Real-Time Response
- 🛡 Non-Invasive System
- 💰 Cost-Effective Accessibility
- 🧩 Modular & Upgradeable Architecture

---

## 🔍 Why It’s Different

| Feature | NeuroMove | Typical BCI Systems |
|----------|------------|--------------------|
| Mobility | Full Adult Wheelchair | Mini/Prototype |
| Cost | Affordable (~₹30K) | High-End / Research Only |
| Control | Hybrid EEG + EOG | EEG Only |
| Accessibility | Deployment Ready | Educational |

---

## 🏥 Target Audience

- Individuals with paralysis
- Severe motor impairment patients
- Elderly individuals with mobility limitations
- Hospitals & rehabilitation centers
- Home-care healthcare providers

---

## 💼 Business Model (Future Scope)

- B2B: Hospitals & Clinics
- B2C: Direct-to-home wheelchair solutions
- Custom electrode calibration services
- Maintenance & support model

---

## ⚠️ Feasibility & Showstoppers

### Potential Issues:
- EEG Signal Noise
- User Calibration Time
- Environmental Interference

### Mitigation:
- Smart filters
- Hybrid EOG validation
- User-specific calibration profile

---

## 📈 Future Improvements

- AI-based adaptive learning model
- Obstacle detection (Ultrasonic / LiDAR)
- Emergency auto-stop system
- Mobile app monitoring
- Autonomous assist mode

---

## 🎥 Demo & Testing Videos

<!-- VIDEO PLACEHOLDER -->

🔗 Demo Video Link:  
`ADD YOUTUBE OR DRIVE LINK HERE`

---

## 📂 Repository Structure
