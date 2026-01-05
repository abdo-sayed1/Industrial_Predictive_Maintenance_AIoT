#ifndef CONFIG_H
#define CONFIG_H

// ============ WiFi Configuration ============
#define WIFI_SSID "A"
#define WIFI_PASSWORD "151201Aa@"

// ============ MQTT Configuration ============
#define MQTT_BROKER "98.88.118.16"        // Change to your Ubuntu IP
#define MQTT_PORT 1883
#define MQTT_TOPIC "machine/sensors"
#define MQTT_TOPIC_CONTROL "machine/control"
#define MQTT_CLIENT_ID "esp32_machine_01"
#define BROKER_MODE true                 // true for embedded broker, false for external
// ============ Sensor Pins ============
// MPU6050 (I2C)
#define MPU6050_SDA 5
#define MPU6050_SCL 4

// DS18B20 (OneWire)
#define DS18B20_PIN 32

// Voltage Sensor (Analog)
#define VOLTAGE_PIN 35
// Current Sensor MPU 
#define CURRENT_PIN 34
// Rotary Encoder
#define ENCODER_A_PIN 18
#define MAX471_VOLTAGE_PIN 35
#define MAX471_CURRENT_PIN 34
// ============ Stepper Motor (A4988) Pins ============
#define STEPPER_STEP_PIN 18          // Step pulse pin
#define STEPPER_DIR_PIN 19           // Direction pin
#define STEPPER_ENABLE_PIN 25        // Enable pin (LOW = enabled)
#define STEPPER_MS1_PIN 14           // Microstep 1 (optional)
#define STEPPER_MS2_PIN 12           // Microstep 2 (optional)
#define STEPPER_MS3_PIN 13           // Microstep 3 (optional)

// ============ Stepper Motor Configuration ============
#define STEPS_PER_REVOLUTION 200     // 1.8° stepper = 200 steps/rev
#define MICROSTEPS 16                // A4988 microstepping (1, 2, 4, 8, 16)
#define MAX_SPEED 1000               // Maximum RPM
#define ACCELERATION 500             // Steps per second²
#define DEFAULT_SPEED 500            // Default RPM

// ============ Sampling Configuration ============
#define VIBRATION_SAMPLE_RATE 200          // Hz
#define TEMP_SAMPLE_RATE 1                 // Hz
#define CURRENT_SAMPLE_RATE 100            // Hz
#define PUBLISH_INTERVAL 1000              // ms (1 second)

// ============ Feature Extraction ============
#define FFT_SIZE 512
#define WINDOW_SIZE 1000                   // 5 seconds at 200Hz

// ============ Alert Thresholds ============
#define TEMP_ALERT_THRESHOLD 60.0          // °C
#define VIBRATION_ALERT_THRESHOLD 1.0      // g
#define CURRENT_ALERT_THRESHOLD 5.0        // A
#define ANOMALY_SCORE_THRESHOLD 0.7        // 0-1

// ============ Motor Control Modes ============
#define MOTOR_MODE_MANUAL 0                // Manual speed control
#define MOTOR_MODE_AUTO 1                  // AI-controlled speed
#define MOTOR_MODE_TEST 2                  // Test sequence mode

// ============ Debug ============
#define DEBUG_MODE true
#define SERIAL_BAUD 115200
    #define NUM_FEATURES    46   // from scaler_params.h
    #define NUM_CLASSES     7
    #define SAMPLE_DELAY_MS 10

#endif
