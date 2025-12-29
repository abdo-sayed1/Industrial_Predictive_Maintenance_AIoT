# 🏭 Industrial Predictive Maintenance System

<div align="center">

![Status](https://img.shields.io/badge/Status-In%20Development-yellow)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![AI](https://img.shields.io/badge/AI-TensorFlow%20Lite-orange)
![License](https://img.shields.io/badge/License-MIT-green)

**An AIoT-based predictive maintenance system for industrial machinery using real-time sensor data, machine learning, and cloud analytics.**

[Features](#-features) • [Architecture](#-system-architecture) • [Installation](#-installation) • [Usage](#-usage) • [Documentation](#-documentation)

</div>

---

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [System Architecture](#-system-architecture)
- [Technology Stack](#-technology-stack)
- [Hardware Components](#-hardware-components)
- [Fault Detection Capabilities](#-fault-detection-capabilities)
- [Installation](#-installation)
- [Project Structure](#-project-structure)
- [Data Flow](#-data-flow)
- [Machine Learning Pipeline](#-machine-learning-pipeline)
- [Dashboard & Visualization](#-dashboard--visualization)
- [Usage](#-usage)
- [API Reference](#-api-reference)
- [Contributing](#-contributing)
- [Documentation](#-documentation)
- [Roadmap](#-roadmap)
- [License](#-license)
- [Contact](#-contact)

---

## 🔍 Overview

This project implements an end-to-end **Industrial Predictive Maintenance System** that combines IoT sensors, edge computing, and artificial intelligence to predict machinery failures before they occur. The system monitors critical parameters such as vibration, temperature, current, and speed to detect anomalies and classify fault types in real-time.

### Key Objectives

- ✅ **Real-time monitoring** of industrial machinery health
- ✅ **Early fault detection** using ML-powered anomaly detection
- ✅ **Reduce downtime** through predictive maintenance scheduling
- ✅ **Cost optimization** by preventing catastrophic failures
- ✅ **Scalable architecture** supporting multiple machines

---

## ✨ Features

### 🔧 Hardware Integration
- Multi-sensor data acquisition (vibration, temperature, current, voltage, speed)
- ESP32-based edge computing for real-time processing
- High-frequency sampling (up to 200 Hz) for vibration analysis
- Modular HAL/MCAL architecture for easy sensor expansion

### 🤖 Machine Learning
- TensorFlow Lite model optimized for ESP32
- Anomaly detection using autoencoder/isolation forest
- Multi-class fault classification (imbalance, misalignment, bearing faults, overload)
- Feature engineering with 50+ derived parameters
- Real-time inference with <100ms latency

### ☁️ Cloud Infrastructure
- MQTT-based telemetry pipeline
- Time-series data storage in InfluxDB
- Real-time visualization with Grafana
- Dockerized microservices architecture
- Scalable for fleet management

### 📊 Analytics & Visualization
- Interactive Grafana dashboards
- Historical trend analysis
- Fault prediction alerts
- Health score monitoring
- Maintenance scheduling recommendations

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         EDGE LAYER (ESP32)                      │
├─────────────────────────────────────────────────────────────────┤
│  Sensors → HAL → MCAL → Feature Extraction → TinyML Inference  │
│    ↓         ↓      ↓           ↓                    ↓          │
│  MPU6050  DS18B20  ACS712    FFT/RMS           Anomaly Score    │
│  Encoder   Voltage                            Fault Classification│
└────────────────────────────┬────────────────────────────────────┘
                             │ MQTT (WiFi)
                             ↓
┌─────────────────────────────────────────────────────────────────┐
│                      CLOUD LAYER (Docker)                       │
├─────────────────────────────────────────────────────────────────┤
│  MQTT Broker → Telegraf → InfluxDB → Grafana                   │
│  (Mosquitto)   (Pipeline) (Database) (Visualization)           │
└─────────────────────────────────────────────────────────────────┘
                             │
                             ↓
┌─────────────────────────────────────────────────────────────────┐
│                      AI TRAINING LAYER                          │
├─────────────────────────────────────────────────────────────────┤
│  Data Collection → Feature Engineering → Model Training         │
│                  → TFLite Conversion → Edge Deployment          │
└─────────────────────────────────────────────────────────────────┘
```

---

## 🛠️ Technology Stack

### **Edge Computing**
- **Microcontroller:** ESP32 (Dual-core, WiFi/BLE)
- **Framework:** Arduino/PlatformIO
- **ML Runtime:** TensorFlow Lite Micro
- **Communication:** MQTT Protocol

### **Cloud Infrastructure**
- **Message Broker:** Eclipse Mosquitto (MQTT)
- **Data Pipeline:** Telegraf
- **Database:** InfluxDB 2.x (Time-Series)
- **Visualization:** Grafana 10.x
- **Orchestration:** Docker Compose

### **AI/ML Stack**
- **Language:** Python 3.9+
- **ML Framework:** TensorFlow 2.13 / Keras
- **Data Processing:** NumPy, Pandas, SciPy
- **Model Compression:** TensorFlow Lite Converter
- **Training:** Jupyter Notebooks

---

## 🔌 Hardware Components

| Component | Model | Purpose | Interface |
|-----------|-------|---------|-----------|
| **Microcontroller** | ESP32 DevKit | Edge computing & connectivity | - |
| **IMU** | MPU6050 | 3-axis vibration monitoring | I2C |
| **Temperature** | DS18B20 | Thermal monitoring | OneWire |
| **Current Sensor** | ACS712 | Electrical load measurement | Analog |
| **Voltage Sensor** | Voltage Divider | Power supply monitoring | Analog |
| **Speed Sensor** | Rotary Encoder | RPM measurement | Digital (Interrupt) |
| **Stepper Motor** | NEMA 17/23 | Load simulation & control | Step/Dir |
| **Stepper Driver** | A4988 | Stepper motor control | Digital (Step/Dir/Enable) |
| **Test Rig** | Motor + Flywheel | Fault simulation platform | - |

### Wiring Diagram
> See `/docs/01_hardware_setup.md` for detailed wiring instructions and pin configurations.

---

## 🎯 Fault Detection Capabilities

The system can detect and classify the following machinery faults:

| Fault Type | Detection Method | Key Indicators |
|------------|------------------|----------------|
| **Imbalance** | Vibration analysis | High 1× RPM frequency, increases with speed² |
| **Misalignment** | Multi-axis vibration | High 2× RPM, axial vibration, thermal rise |
| **Bearing Fault** | High-frequency analysis | Energy spike at 5-20 kHz, temperature increase |
| **Mechanical Looseness** | Vibration harmonics | Multiple harmonics, speed instability |
| **Overload** | Current/thermal monitoring | High current, rapid temperature rise, speed drop |

### Feature Extraction
The system extracts **50+ features** from raw sensor data:
- **Time Domain:** RMS, Peak-to-Peak, Variance, Kurtosis, Crest Factor
- **Frequency Domain:** FFT, Dominant Frequency, Harmonic Analysis
- **Cross-Domain:** Vibration-RPM correlation, Current-Temperature trends
- **Statistical:** Moving averages, Rate of change, Anomaly scores

---

## 📦 Installation

### Prerequisites
- **Hardware:** ESP32 + Sensors (see hardware section)
- **Software:** 
  - Docker Desktop (for cloud stack)
  - PlatformIO or Arduino IDE (for firmware)
  - Python 3.9+ (for AI training)
  - Git

### Quick Start

#### 1️⃣ Clone Repository
```bash
git clone https://github.com/yourusername/predictive-maintenance-aiot.git
cd predictive-maintenance-aiot
```

#### 2️⃣ Setup Cloud Infrastructure (Docker)
```bash
cd cloud
# Edit .env file with your credentials
docker-compose up -d
```

**Services will be available at:**
- Grafana: `http://localhost:3000` (admin/admin)
- InfluxDB: `http://localhost:8086`
- MQTT Broker: `localhost:1883`

#### 3️⃣ Flash ESP32 Firmware
```bash
cd interface
# Edit config.h with your WiFi credentials and MQTT broker IP
pio run --target upload
pio device monitor
```

#### 4️⃣ Train AI Model (Optional - Pre-trained model included)
```bash
cd AI
pip install -r requirements.txt
jupyter notebook notebooks/03_train_model.ipynb
```

---

## 📁 Project Structure

```
predictive-maintenance-aiot/
│
├── AI/                              # Machine Learning Pipeline
│   ├── data/                        # Training datasets
│   │   ├── raw/                     # Raw sensor CSV files
│   │   └── processed/               # Featured & labeled data
│   ├── notebooks/                   # Jupyter notebooks
│   ├── models/                      # Trained models (H5, TFLite)
│   └── scripts/                     # Training scripts
│
├── cloud/                           # Cloud Infrastructure
│   ├── mosquitto/                   # MQTT broker config
│   ├── telegraf/                    # Data pipeline config
│   ├── influxdb/                    # Time-series database
│   ├── grafana/                     # Dashboards & datasources
│   ├── docker-compose.yml           # Orchestration file
│   └── .env                         # Environment variables
│
├── interface/                       # ESP32 Firmware
│   ├── src/
│   │   ├── hal/                     # Hardware Abstraction Layer
│   │   │   ├── mpu6050/             # Vibration sensor
│   │   │   ├── ds18b20/             # Temperature sensor
│   │   │   ├── acs712/              # Current sensor
│   │   │   ├── voltage/             # Voltage sensor
│   │   │   ├── encoder/             # Speed sensor
│   │   │   └── stepper/             # Stepper motor (A4988)
│   │   ├── mcal/                    # Microcontroller Abstraction
│   │   │   ├── i2c/                 # I2C driver
│   │   │   ├── adc/                 # ADC driver
│   │   │   ├── gpio/                # GPIO/Interrupt driver
│   │   │   └── pwm/                 # PWM driver (for stepper)
│   │   ├── services/                # Application Services
│   │   │   ├── mqtt/                # MQTT client
│   │   │   ├── wifi/                # WiFi manager
│   │   │   └── buffer/              # Data buffering
│   │   └── app/                     # Application Layer
│   │       ├── features/            # Feature extraction
│   │       ├── inference/           # TinyML inference
│   │       └── motor_control/       # Stepper motor control
│   ├── main.ino                     # Main application
│   ├── config.h                     # Configuration file
│   └── platformio.ini               # Build configuration
│
├── docs/                            # Documentation
│   ├── 01_hardware_setup.md
│   ├── 02_software_setup.md
│   ├── 03_docker_setup.md
│   ├── 04_data_collection.md
│   ├── 05_fault_injection.md
│   ├── 06_model_training.md
│   └── 07_deployment.md
│
└── scripts/                         # Utility scripts
    ├── backup.sh                    # Database backup
    └── test_mqtt.py                 # MQTT testing
```

---

## 🔄 Data Flow

```
┌─────────────┐
│   Sensors   │  Sample at 200Hz (vibration), 100Hz (current), 1Hz (temp)
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Buffering  │  Circular buffer (1 second window)
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Features   │  Extract 50+ features (RMS, FFT, correlations)
└──────┬──────┘
       │
       ↓
┌─────────────┐
│ ML Inference│  Predict health score & fault type
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  MQTT Pub   │  Publish JSON payload every 1 second
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Mosquitto  │  MQTT Broker
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  Telegraf   │  Parse & forward to InfluxDB
└──────┬──────┘
       │
       ↓
┌─────────────┐
│  InfluxDB   │  Store time-series data
└──────┬──────┘
       │
       ↓
┌─────────────┐
│   Grafana   │  Visualize & alert
└─────────────┘
```

### MQTT Message Format
```json
{
  "timestamp": 1640000000,
  "rpm": 1450,
  "temp": 42.3,
  "current": 2.8,
  "voltage": 230.5,
  "vib_x": 0.15,
  "vib_y": 0.12,
  "vib_z": 0.08,
  "health_score": 87.5,
  "fault_type": "healthy"
}
```

---

## 🧠 Machine Learning Pipeline

### 1. Data Collection
- **Healthy Data:** 40% of dataset
  - Multiple speeds (500, 1000, 1500 RPM)
  - Various load conditions
  - Different ambient temperatures
  
- **Fault Data:** 60% of dataset
  - Imbalance (progressive weights: 5g, 10g, 20g, 50g)
  - Misalignment (shaft offsets: 0.5mm, 1mm, 2mm)
  - Bearing degradation (lubrication reduction)
  - Mechanical looseness (bolt loosening)
  - Overload conditions (friction brake)

### 2. Feature Engineering
```python
# Time Domain Features
- RMS Vibration (X, Y, Z)
- Peak-to-Peak
- Standard Deviation
- Kurtosis, Skewness
- Crest Factor

# Frequency Domain Features
- FFT Analysis
- Dominant Frequency
- Harmonic Amplitudes
- High-frequency Energy (bearing indicator)

# Cross-Domain Features
- Vibration/RPM Ratio
- Current/Temperature Correlation
- Power Efficiency
- Mechanical Efficiency Index
```

### 3. Model Architecture
**Approach 1: Anomaly Detection (Unsupervised)**
- Autoencoder (50 → 20 → 10 → 20 → 50)
- Trained only on healthy data
- Output: Anomaly score (0-1)

**Approach 2: Fault Classification (Supervised)**
- Neural Network (50 → 128 → 64 → 32 → 5)
- Classes: Healthy, Imbalance, Misalignment, Bearing Fault, Overload
- Output: Fault type + confidence

### 4. Model Optimization
- **Quantization:** float32 → int8 (8x size reduction)
- **Pruning:** Remove redundant weights
- **TFLite Conversion:** Optimize for ESP32
- **Target Size:** <100KB

---

## 📊 Dashboard & Visualization

### Grafana Dashboards

**Real-time Monitoring Panel:**
- Live sensor readings (RPM, Temperature, Current, Voltage)
- 3-axis vibration waveforms
- Health score gauge (0-100)
- Fault type indicator

**Trend Analysis Panel:**
- Historical health score trends
- Temperature vs time
- Current consumption patterns
- Vibration amplitude trends

**Alert Panel:**
- Active fault warnings
- Threshold violations
- Maintenance recommendations
- Downtime predictions

> Access Grafana at `http://localhost:3000` after running `docker-compose up`

---

## 🚀 Usage

### Basic Operation

1. **Power on the system**
   ```bash
   # ESP32 will auto-connect to WiFi and MQTT broker
   # Check serial monitor for status
   ```

2. **Monitor in Grafana**
   - Open browser: `http://localhost:3000`
   - Login: `admin` / `admin`
   - Navigate to "Machine Monitoring" dashboard

3. **Test fault injection**
   - Add weight to flywheel (imbalance)
   - Monitor vibration increase
   - Observe ML prediction change

### Advanced Operations

#### Data Export
```bash
# Export InfluxDB data for retraining
influx query 'from(bucket:"machine_data") |> range(start: -7d)' --raw > export.csv
```

#### Model Update
```bash
cd AI
python scripts/train.py --data data/processed/new_data.csv
python scripts/export.py --model models/model.h5
# Flash new model to ESP32
```

#### Remote Monitoring
```bash
# Setup port forwarding or reverse proxy (nginx)
# Access from anywhere: https://yourdomain.com/grafana
```

---

## 📡 API Reference

### MQTT Topics

| Topic | Direction | Format | Description |
|-------|-----------|--------|-------------|
| `machine/sensors` | ESP32 → Cloud | JSON | Sensor data + ML predictions |
| `machine/control` | Cloud → ESP32 | JSON | Control commands |
| `machine/alerts` | ESP32 → Cloud | JSON | Critical fault alerts |

### REST API (Planned)
```
GET  /api/machines              # List all machines
GET  /api/machines/{id}/status  # Current status
GET  /api/machines/{id}/history # Historical data
POST /api/machines/{id}/control # Send command
```

---

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines
- Follow existing code structure (HAL/MCAL/Services)
- Add unit tests for new features
- Update documentation
- Use meaningful commit messages

---

## 📚 Documentation

Detailed documentation is available in the `/docs` folder:

- **[Hardware Setup](docs/01_hardware_setup.md)** - Wiring diagrams and assembly
- **[Software Setup](docs/02_software_setup.md)** - Development environment
- **[Docker Setup](docs/03_docker_setup.md)** - Cloud infrastructure
- **[Data Collection](docs/04_data_collection.md)** - Training data guidelines
- **[Fault Injection](docs/05_fault_injection.md)** - Creating realistic faults
- **[Model Training](docs/06_model_training.md)** - AI pipeline details
- **[Deployment](docs/07_deployment.md)** - Production deployment

---

## 🗺️ Roadmap

### Phase 1: Core System ✅
- [x] Hardware integration
- [x] Basic ML model
- [x] MQTT pipeline
- [x] Grafana dashboard

### Phase 2: Enhanced Intelligence 🔄
- [ ] Multiple machine support
- [ ] Advanced fault patterns (cavitation, resonance)
- [ ] Predictive RUL (Remaining Useful Life)
- [ ] Auto-retraining pipeline

### Phase 3: Production Ready 📋
- [ ] Web-based configuration UI
- [ ] Mobile app (Flutter)
- [ ] RESTful API
- [ ] Alert system (Email, SMS, Telegram)
- [ ] Maintenance scheduling integration
- [ ] Cloud deployment (AWS/Azure)

### Phase 4: Advanced Features 🚀
- [ ] Fleet management
- [ ] Digital twin simulation
- [ ] AR maintenance guidance
- [ ] Blockchain maintenance logs

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👥 Contact

**Project Maintainer:** 
Abdelrahman Sayed Ahmed

- 📧 Email: abdo.siyed.official@gmail.com 
- 💼 LinkedIn: https://www.linkedin.com/in/abdo-sayed1
- 🐙 GitHub: 

Karim Walid Fawzy

- 📧 Email: Karim@gmail.com
- 💼 LinkedIn: 
- 🐙 GitHub: https://github.com/KarimWalidFawzy
  
  Mariam Ahmed Goher
  
- 📧 Email:  mariamgoher@gmail.com
- 💼 LinkedIn: https://www.linkedin.com/in/mariam-ahmed-91b500216
- 🐙 GitHub: https://github.com/starAwesome123

**Project Link:** https://github.com/abdo-sayed1/Industrial_Predictive_Maintenance_AIoT

---

## 🙏 Acknowledgments

- **Supervisor:** [Supervisor Name] - Project guidance and mentorship
- **Institution:** [University/Company Name]
- **References:**
  - ISO 13374 (Condition monitoring and diagnostics)
  - IEEE 1856 (Framework for prognostics)
  - TensorFlow Lite Micro team
  - Open-source community

---

## 📊 Project Statistics

![Languages](https://img.shields.io/github/languages/count/yourusername/predictive-maintenance-aiot)
![Top Language](https://img.shields.io/github/languages/top/yourusername/predictive-maintenance-aiot)
![Code Size](https://img.shields.io/github/languages/code-size/yourusername/predictive-maintenance-aiot)
![Last Commit](https://img.shields.io/github/last-commit/yourusername/predictive-maintenance-aiot)

---

<div align="center">

**⭐ If this project helped you, please consider giving it a star! ⭐**

Made with ❤️ for Industrial IoT and Predictive Maintenance

</div>
