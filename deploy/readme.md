# Motor Fault Detection - ESP32 Deployment

## 📋 Overview

This Arduino sketch deploys a trained neural network on ESP32 to perform real-time motor fault classification using TensorFlow Lite for Microcontrollers. The system reads sensor data from an MPU6050 IMU, temperature sensor, current sensor, and RPM sensor to predict motor health conditions.

## 🎯 Features

- ✅ Real-time inference on ESP32 (< 2ms per prediction)
- ✅ 7-class motor fault classification
- ✅ INT8 quantized model (< 10 KB)
- ✅ Low power consumption
- ✅ Serial output for monitoring
- ✅ Automatic sensor calibration

## 🔧 Hardware Requirements

### Microcontroller
- **ESP32 Development Board** (any variant)
  - Recommended: ESP32-DevKitC, ESP32-WROOM-32
  - Minimum: 4MB Flash, 520KB RAM

### Sensors Required

| Sensor | Purpose | Interface | Pins |
|--------|---------|-----------|------|
| MPU6050 | Accelerometer + Gyroscope | I2C | SDA (GPIO21), SCL (GPIO22) |
| Temperature Sensor | Motor temperature | Analog | GPIO35 |
| Current Sensor | Motor current | Analog | GPIO34 |
| RPM Sensor | Motor speed | Digital/Analog | GPIO32 |

### Optional Components
- USB cable for programming and power
- Breadboard and jumper wires
- Power supply (5V or 3.3V depending on sensors)

## 📁 Project Structure

```
motor_fault_detection/
├── motor_fault_detection.ino          # Main Arduino sketch
├── motor_fault_model.h                # Converted TFLite model (required)
├── scaler_params.h                    # Normalization parameters (required)
└── README.md                          # This file
```

## 📚 Required Libraries

Install these libraries via **Arduino IDE → Tools → Manage Libraries**:

### 1. MPU6050_light
- **Author:** rfetick
- **Version:** 1.0.0 or higher
- **Purpose:** MPU6050/MPU6500 IMU interface

### 2. Arduino_TensorFlowLite
- **Author:** TensorFlow Authors
- **Version:** Latest available
- **Purpose:** TensorFlow Lite inference engine

**Installation:**
```
1. Open Arduino IDE
2. Go to Tools → Manage Libraries
3. Search "MPU6050_light"
4. Click Install
5. Search "Arduino_TensorFlowLite" (or "TensorFlowLite_ESP32")
6. Click Install
```

## 🔌 Hardware Connections

### MPU6050 IMU Wiring

```
MPU6050          ESP32
────────────────────────
VCC       →      3.3V
GND       →      GND
SDA       →      GPIO 21
SCL       →      GPIO 22
```

### Temperature Sensor (Example: LM35)

```
LM35             ESP32
────────────────────────
VCC       →      5V
GND       →      GND
OUT       →      GPIO 35 (Analog)
```

### Current Sensor (Example: ACS712)

```
ACS712           ESP32
────────────────────────
VCC       →      5V
GND       →      GND
OUT       →      GPIO 34 (Analog)
```

### RPM Sensor (Example: Hall Effect)

```
Hall Sensor      ESP32
────────────────────────
VCC       →      3.3V
GND       →      GND
OUT       →      GPIO 32 (Digital)
```

**⚠️ Important:** 
- Use 3.3V for ESP32 I/O pins (not 5V!)
- Add pull-up resistors (4.7kΩ) on I2C lines if needed
- Check your specific sensor datasheets for voltage requirements

## 📝 Setup Instructions

### Step 1: Prepare Model Files

#### A. Convert TFLite Model to C Header

After training your Python model, you'll have `motor_fault_model_quantized.tflite`. Convert it:

```bash
# On Linux/Mac
xxd -i motor_fault_model_quantized.tflite > motor_fault_model.h

# On Windows (install xxd first or use online converter)
xxd -i motor_fault_model_quantized.tflite > motor_fault_model.h
```

#### B. Edit motor_fault_model.h

Open the file and ensure it looks like this:

```cpp
const unsigned char motor_fault_model_quantized[] = {
  0x1c, 0x00, 0x00, 0x00, 0x54, 0x46, 0x4c, 0x33, 0x14, 0x00, 0x20, 0x00,
  // ... rest of hex data ...
};
unsigned int motor_fault_model_quantized_len = 3648;
```

**Key changes:**
- Remove `_tflite` from array name
- Add `const` keyword (saves RAM)
- Ensure array name matches what's in the sketch

### Step 2: Create Scaler Parameters File

Create `scaler_params.h` with your training scaler values:

```cpp
#ifndef SCALER_PARAMS_H
#define SCALER_PARAMS_H

// ⚠️ REPLACE THESE VALUES WITH YOUR PYTHON TRAINING OUTPUT!
// Copy from Python script output: "Mean values:" and "Std values:"

const float scaler_mean[9] = {
  0.0599141163,    // ax mean
  0.0468072370,    // ay mean
  0.0421709463,    // az mean
  0.277288616,     // gx mean
  0.262001281,     // gy mean
  0.281187325,     // gz mean
  43.4209505,      // temp mean
  2.16994154,      // current mean
  1476.02631       // rpm mean
};

const float scaler_std[9] = {
  0.12853713,      // ax std
  0.1281364,       // ay std
  0.12954736,      // az std
  1.33143455,      // gx std
  1.31496109,      // gy std
  1.33515316,      // gz std
  6.59782181,      // temp std
  0.43492526,      // current std
  58.00813606      // rpm std
};

#endif
```

### Step 3: Organize Files

Place all three files in the same folder:
```
motor_fault_detection/
├── motor_fault_detection.ino
├── motor_fault_model.h
└── scaler_params.h
```

### Step 4: Configure Arduino IDE

1. **Install ESP32 Board Support**
   - File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Tools → Board → Boards Manager
   - Search "esp32" and install

2. **Select Board**
   - Tools → Board → ESP32 Arduino → ESP32 Dev Module

3. **Select Port**
   - Tools → Port → (your ESP32 port)

4. **Configure Upload Settings**
   - Upload Speed: 921600
   - Flash Frequency: 80MHz
   - Partition Scheme: Default 4MB

### Step 5: Upload Code

1. Connect ESP32 via USB
2. Click **Upload** button (→)
3. Wait for compilation and upload
4. Open Serial Monitor (115200 baud)

## 🖥️ Serial Monitor Output

After successful upload, you should see:

```
========================================
ESP32 Motor Fault Detection System
========================================

>>> Initializing Sensors <<<

MPU6050 Status: 0
✅ MPU6050 Connected
Calibrating... Keep board still.
✅ Calibration Done
✅ Analog sensors configured

>>> Initializing TensorFlow Lite <<<

✅ Model loaded
✅ Operations registered
✅ Interpreter created
✅ Tensors allocated

--- Model Information ---
Input shape: [1, 9]
Input type: INT8
Output shape: [1, 7]
Arena used: 8192 / 20480 bytes
-------------------------

✅ System Ready - Starting inference loop

========================================

--- Raw Sensor Data ---
Accel: [0.051, -0.023, 0.981] g
Gyro:  [0.234, -0.156, 0.089] deg/s
Temp:    25.34 °C
Current: 2.45 A
RPM:     1498

--- Normalized Input ---
[0.123, -0.456, 0.789, 0.234, -0.156, 0.089, 0.341, 0.567, -0.123]

⚡ Inference time: 1234 µs

--- Class Scores ---
Class 0 (Normal): 0.8521
Class 1 (Imbalance): 0.0234
Class 2 (Misalignment): 0.0156
Class 3 (Bearing): 0.0089
Class 4 (Looseness): 0.0123
Class 5 (Overload): 0.0456
Class 6 (Overheat): 0.0421
Confidence: 85.21%

>>> PREDICTION RESULT <<<
Detected Fault: Normal (Class 0)
========================================
```

## ⚙️ Configuration Options

Edit these values at the top of the sketch:

```cpp
// Timing
#define INFERENCE_INTERVAL 2000    // Time between predictions (ms)

// Memory
#define TENSOR_ARENA_SIZE (20 * 1024)  // Increase if model needs more

// Sensor Pins
#define TEMP_SENSOR_PIN 35
#define CURRENT_SENSOR_PIN 34
#define RPM_SENSOR_PIN 32

// Model
#define NUM_FEATURES 9
#define NUM_CLASSES 7
```

## 🔧 Customizing Sensor Functions

The sketch includes placeholder functions that you need to replace with actual sensor code:

### Temperature Sensor

```cpp
float readTemperature() {
  // Example for LM35 (10mV/°C)
  int raw = analogRead(TEMP_SENSOR_PIN);
  float voltage = raw * 3.3 / 4095.0;
  return voltage * 100.0;  // Convert to °C
}
```

### Current Sensor

```cpp
float readCurrent() {
  // Example for ACS712-30A (66mV/A, 1.5V offset)
  int raw = analogRead(CURRENT_SENSOR_PIN);
  float voltage = raw * 3.3 / 4095.0;
  return (voltage - 1.5) / 0.066;
}
```

### RPM Sensor

```cpp
// Use interrupt-based counting for accuracy
volatile unsigned long pulse_count = 0;
unsigned long last_rpm_time = 0;

void IRAM_ATTR pulseCounter() {
  pulse_count++;
}

void setup() {
  // In setup():
  pinMode(RPM_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RPM_SENSOR_PIN), pulseCounter, RISING);
}

float readRPM() {
  unsigned long current_time = millis();
  unsigned long elapsed = current_time - last_rpm_time;
  
  if (elapsed >= 1000) {  // Calculate every second
    float rpm = (pulse_count * 60.0) / (elapsed / 1000.0);
    pulse_count = 0;
    last_rpm_time = current_time;
    return rpm;
  }
  return 0;
}
```

## 🐛 Troubleshooting

### Issue: MPU6050 Not Detected

**Symptoms:**
```
MPU Status: 1
❌ MPU6050 Connection Failed!
```

**Solutions:**
1. Check wiring (SDA, SCL, VCC, GND)
2. Verify I2C address (default 0x68, alternative 0x69)
3. Add pull-up resistors (4.7kΩ) on SDA/SCL
4. Try I2C scanner sketch to detect device

```cpp
// Change I2C address if needed
MPU6050 mpu(Wire, 0x69);  // Try alternate address
```

### Issue: Compilation Errors

**Error:** `'kTfLiteFloat32' was not declared`

**Solution:** Ensure TensorFlow Lite library is properly installed
```
Tools → Manage Libraries → Search "TensorFlowLite" → Reinstall
```

**Error:** `motor_fault_model.h: No such file`

**Solution:** Place all files in sketch folder and restart Arduino IDE

### Issue: Model Allocation Failed

**Symptoms:**
```
❌ AllocateTensors() failed!
```

**Solutions:**
1. Increase `TENSOR_ARENA_SIZE`:
   ```cpp
   #define TENSOR_ARENA_SIZE (30 * 1024)  // Try 30KB
   ```
2. Check model file is correctly included
3. Verify model is quantized (INT8, not FLOAT32)

### Issue: Wrong Predictions

**Causes:**
- Incorrect scaler parameters
- Sensor readings in wrong units
- Model trained on different data

**Solutions:**
1. Verify `scaler_params.h` matches Python output
2. Check sensor calibration
3. Ensure units match training data:
   - Acceleration in g (9.81 m/s²)
   - Gyroscope in deg/s
   - Temperature in °C
   - Current in Amperes
   - RPM in revolutions per minute

### Issue: Slow Inference

**Expected:** < 5ms per inference
**If slower:**
1. Check `TENSOR_ARENA_SIZE` (don't make it too large)
2. Verify INT8 quantization (not FLOAT32)
3. Reduce inference frequency

## 📊 Performance Metrics

| Metric | Value |
|--------|-------|
| Inference Time | ~1-2 ms |
| Model Size | ~3-5 KB |
| RAM Usage | ~10-15 KB |
| Power Consumption | ~80-120 mA (active) |
| Update Rate | Configurable (default 2s) |

## 🔒 Power Optimization

For battery-powered applications:

```cpp
void loop() {
  // Run inference
  runInference();
  
  // Enter light sleep between measurements
  esp_sleep_enable_timer_wakeup(INFERENCE_INTERVAL * 1000);
  esp_light_sleep_start();
}
```

## 🌐 Adding WiFi Connectivity

To send predictions to a server:

```cpp
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "your_ssid";
const char* password = "your_password";
const char* serverURL = "http://your-server.com/api/fault";

void sendPrediction(int fault_class) {
  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  
  String json = "{\"fault_class\":" + String(fault_class) + 
                ",\"timestamp\":" + String(millis()) + "}";
  
  int httpCode = http.POST(json);
  http.end();
}
```

## 📝 Next Steps

1. **Calibrate Sensors:** Run calibration routines for accurate readings
2. **Test Predictions:** Compare with known fault conditions
3. **Add Logging:** Store predictions to SD card or cloud
4. **Create Alerts:** Add buzzer/LED for fault warnings
5. **Build Enclosure:** Protect ESP32 and sensors from environment

## 📚 Additional Resources

- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [TensorFlow Lite Micro](https://www.tensorflow.org/lite/microcontrollers)
- [MPU6050 Datasheet](https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/)

## 📧 Support

For issues:
1. Check troubleshooting section
2. Verify all connections
3. Test sensors individually
4. Check Serial Monitor output for error messages

## 📄 License

This project is for educational and research purposes.

---

**Previous Step:** Complete Python model training (see Python README)
**Next Step:** Deploy and test on real motor system