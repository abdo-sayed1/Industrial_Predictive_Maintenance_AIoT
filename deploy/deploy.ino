/*
 * ESP32 Motor Fault Detection - Native TensorFlow Lite
 * Based on instructor's gesture recognition template
 * Adapted for single-sample motor fault classification
 */

#include <Wire.h>
#include <MPU6050_light.h>  // Install via Library Manager

// --- TENSORFLOW LITE INCLUDES ---
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

// --- LOCAL FILES (Create these in your sketch folder!) ---
#include "motor_fault_model.h"     // Your converted .tflite model
#include "scaler_params.h"          // Scaler mean and std values

// ============================================================================
// CONFIGURATION
// ============================================================================
#define NUM_FEATURES 9          // ax, ay, az, gx, gy, gz, temp, current, rpm
#define NUM_CLASSES 7           // 0-6: Normal, Imbalance, Misalignment, Bearing, Looseness, Overload, Overheat
#define TENSOR_ARENA_SIZE (20 * 1024)  // 20KB for model execution
#define INFERENCE_INTERVAL 2000  // Run inference every 2 seconds (ms)

// Sensor Pins (Adjust for your hardware)
#define TEMP_SENSOR_PIN 35
#define CURRENT_SENSOR_PIN 34
#define RPM_SENSOR_PIN 32

// ============================================================================
// GLOBALS
// ============================================================================

// TensorFlow Lite Objects
uint8_t tensor_arena[TENSOR_ARENA_SIZE];
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// MPU6050 Sensor
MPU6050 mpu(Wire);

// Fault Labels
const char* fault_labels[NUM_CLASSES] = {
  "Normal",
  "Imbalance",
  "Misalignment",
  "Bearing",
  "Looseness",
  "Overload",
  "Overheat"
};

// Timing
unsigned long last_inference_time = 0;

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("\n========================================");
  Serial.println("ESP32 Motor Fault Detection System");
  Serial.println("========================================\n");

  // --- A. Initialize Sensors ---
  initializeSensors();

  // --- B. Initialize TensorFlow Lite ---
  initializeTFLite();

  Serial.println("\n✅ System Ready - Starting inference loop\n");
  Serial.println("========================================\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================
void loop() {
  // Check if it's time for next inference
  unsigned long current_time = millis();
  
  if (current_time - last_inference_time >= INFERENCE_INTERVAL) {
    last_inference_time = current_time;
    
    // Update MPU data
    mpu.update();
    
    // Read all sensors
    float sensor_data[NUM_FEATURES];
    readAllSensors(sensor_data);
    
    // Print raw readings
    printSensorData(sensor_data);
    
    // Run ML inference
    int predicted_class = runInference(sensor_data);
    
    // Display result
    if (predicted_class >= 0) {
      Serial.println("\n>>> PREDICTION RESULT <<<");
      Serial.print("Detected Fault: ");
      Serial.print(fault_labels[predicted_class]);
      Serial.print(" (Class ");
      Serial.print(predicted_class);
      Serial.println(")");
    } else {
      Serial.println("❌ Inference failed!");
    }
    
    Serial.println("========================================\n");
  }
}

// ============================================================================
// INITIALIZE SENSORS
// ============================================================================
void initializeSensors() {
  Serial.println(">>> Initializing Sensors <<<\n");
  
  // Initialize I2C for MPU6050
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print("MPU6050 Status: ");
  Serial.println(status);
  
  if (status != 0) {
    Serial.println("❌ MPU6050 Connection Failed!");
    Serial.println("Check wiring: SDA->GPIO21, SCL->GPIO22");
    while (1) delay(1000);
  }
  
  Serial.println("✅ MPU6050 Connected");
  Serial.println("Calibrating... Keep board still.");
  delay(1000);
  
  mpu.calcOffsets();  // Auto-calibrate
  Serial.println("✅ Calibration Done");
  
  // Configure analog pins
  pinMode(TEMP_SENSOR_PIN, INPUT);
  pinMode(CURRENT_SENSOR_PIN, INPUT);
  pinMode(RPM_SENSOR_PIN, INPUT);
  
  Serial.println("✅ Analog sensors configured\n");
}

// ============================================================================
// INITIALIZE TENSORFLOW LITE
// ============================================================================
void initializeTFLite() {
  Serial.println(">>> Initializing TensorFlow Lite <<<\n");
  
  // 1. Load Model
  model = tflite::GetModel(motor_fault_model_quantized);
  
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.print("❌ Model schema mismatch! Model:");
    Serial.print(model->version());
    Serial.print(" Expected:");
    Serial.println(TFLITE_SCHEMA_VERSION);
    while (1);
  }
  Serial.println("✅ Model loaded");
  
  // 2. Define Required Operations
  static tflite::MicroMutableOpResolver<10> micro_op_resolver;
  
  // Add operations used by your model
  micro_op_resolver.AddFullyConnected();  // Dense layers
  micro_op_resolver.AddSoftmax();         // Output activation
  micro_op_resolver.AddRelu();            // Hidden layer activation
  micro_op_resolver.AddQuantize();        // Quantization
  micro_op_resolver.AddDequantize();      // Dequantization
  
  Serial.println("✅ Operations registered");
  
  // 3. Create Interpreter
  static tflite::MicroInterpreter static_interpreter(
    model,
    micro_op_resolver,
    tensor_arena,
    TENSOR_ARENA_SIZE,
    nullptr,
    nullptr
  );
  interpreter = &static_interpreter;
  Serial.println("✅ Interpreter created");
  
  // 4. Allocate Tensors
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  
  if (allocate_status != kTfLiteOk) {
    Serial.println("❌ AllocateTensors() failed!");
    while (1);
  }
  Serial.println("✅ Tensors allocated");
  
  // 5. Get Input/Output Pointers
  input = interpreter->input(0);
  output = interpreter->output(0);
  
  // 6. Print Model Info
  Serial.println("\n--- Model Information ---");
  Serial.print("Input shape: [");
  Serial.print(input->dims->data[0]);
  Serial.print(", ");
  Serial.print(input->dims->data[1]);
  Serial.println("]");
  
  Serial.print("Input type: ");
  Serial.println(input->type == kTfLiteFloat32 ? "FLOAT32" : "INT8");
  
  Serial.print("Output shape: [");
  Serial.print(output->dims->data[0]);
  Serial.print(", ");
  Serial.print(output->dims->data[1]);
  Serial.println("]");
  
  Serial.print("Arena used: ");
  Serial.print(interpreter->arena_used_bytes());
  Serial.print(" / ");
  Serial.print(TENSOR_ARENA_SIZE);
  Serial.println(" bytes");
  Serial.println("-------------------------");
}

// ============================================================================
// READ ALL SENSORS
// ============================================================================
void readAllSensors(float* data) {
  // Read IMU (indices 0-5)
  data[0] = mpu.getAccX();   // Accelerometer X (in g)
  data[1] = mpu.getAccY();   // Accelerometer Y
  data[2] = mpu.getAccZ();   // Accelerometer Z
  data[3] = mpu.getGyroX();  // Gyroscope X (in deg/s)
  data[4] = mpu.getGyroY();  // Gyroscope Y
  data[5] = mpu.getGyroZ();  // Gyroscope Z
  
  // Read Temperature (index 6)
  data[6] = readTemperature();
  
  // Read Current (index 7)
  data[7] = readCurrent();
  
  // Read RPM (index 8)
  data[8] = readRPM();
}

// ============================================================================
// RUN ML INFERENCE
// ============================================================================
int runInference(float* sensor_data) {
  // Step 1: Normalize using scaler parameters from scaler_params.h
  float normalized[NUM_FEATURES];
  
  for (int i = 0; i < NUM_FEATURES; i++) {
    normalized[i] = (sensor_data[i] - scaler_mean[i]) / scaler_std[i];
  }
  
  Serial.println("\n--- Normalized Input ---");
  Serial.print("[");
  for (int i = 0; i < NUM_FEATURES; i++) {
    Serial.print(normalized[i], 3);
    if (i < NUM_FEATURES - 1) Serial.print(", ");
  }
  Serial.println("]");
  
  // Step 2: Copy to input tensor
  if (input->type == kTfLiteFloat32) {
    // Float model
    for (int i = 0; i < NUM_FEATURES; i++) {
      input->data.f[i] = normalized[i];
    }
  } else if (input->type == kTfLiteInt8) {
    // Quantized model
    float input_scale = input->params.scale;
    int input_zero_point = input->params.zero_point;
    
    for (int i = 0; i < NUM_FEATURES; i++) {
      float quantized = normalized[i] / input_scale + input_zero_point;
      input->data.int8[i] = (int8_t)constrain(quantized, -128, 127);
    }
  }
  
  // Step 3: Run inference
  unsigned long start = micros();
  TfLiteStatus invoke_status = interpreter->Invoke();
  unsigned long duration = micros() - start;
  
  if (invoke_status != kTfLiteOk) {
    Serial.println("❌ Invoke() failed!");
    return -1;
  }
  
  Serial.print("⚡ Inference time: ");
  Serial.print(duration);
  Serial.println(" µs");
  
  // Step 4: Process output
  int predicted_class = 0;
  float max_score = -1000.0;
  
  Serial.println("\n--- Class Scores ---");
  
  for (int i = 0; i < NUM_CLASSES; i++) {
    float score;
    
    if (output->type == kTfLiteFloat32) {
      score = output->data.f[i];
    } else if (output->type == kTfLiteInt8) {
      float output_scale = output->params.scale;
      int output_zero_point = output->params.zero_point;
      score = (output->data.int8[i] - output_zero_point) * output_scale;
    }
    
    Serial.print("Class ");
    Serial.print(i);
    Serial.print(" (");
    Serial.print(fault_labels[i]);
    Serial.print("): ");
    Serial.println(score, 4);
    
    if (score > max_score) {
      max_score = score;
      predicted_class = i;
    }
  }
  
  Serial.print("Confidence: ");
  Serial.print(max_score * 100, 2);
  Serial.println("%");
  
  return predicted_class;
}

// ============================================================================
// SENSOR READING FUNCTIONS (Customize for your hardware)
// ============================================================================

float readTemperature() {
  // TODO: Implement based on your temperature sensor
  // Example for analog sensor:
  // int raw = analogRead(TEMP_SENSOR_PIN);
  // float voltage = raw * 3.3 / 4095.0;
  // return voltage * 100.0;  // Convert to °C
  
  // PLACEHOLDER: Random value for testing
  return random(200, 300) / 10.0;  // 20-30°C
}

float readCurrent() {
  // TODO: Implement based on your current sensor
  // Example for ACS712:
  // int raw = analogRead(CURRENT_SENSOR_PIN);
  // float voltage = raw * 3.3 / 4095.0;
  // return (voltage - 1.65) / 0.066;  // ACS712-30A
  
  // PLACEHOLDER: Random value for testing
  return random(20, 30) / 10.0;  // 2-3A
}

float readRPM() {
  // TODO: Implement based on your RPM sensor
  // Typically requires interrupt-based pulse counting
  
  // PLACEHOLDER: Random value for testing
  return random(1400, 1600);  // 1400-1600 RPM
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void printSensorData(float* data) {
  Serial.println("--- Raw Sensor Data ---");
  Serial.print("Accel: [");
  Serial.print(data[0], 3); Serial.print(", ");
  Serial.print(data[1], 3); Serial.print(", ");
  Serial.print(data[2], 3); Serial.println("] g");
  
  Serial.print("Gyro:  [");
  Serial.print(data[3], 3); Serial.print(", ");
  Serial.print(data[4], 3); Serial.print(", ");
  Serial.print(data[5], 3); Serial.println("] deg/s");
  
  Serial.print("Temp:    ");
  Serial.print(data[6], 2);
  Serial.println(" °C");
  
  Serial.print("Current: ");
  Serial.print(data[7], 2);
  Serial.println(" A");
  
  Serial.print("RPM:     ");
  Serial.println(data[8], 0);
}