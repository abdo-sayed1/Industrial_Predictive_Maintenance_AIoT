#include <Wire.h>
#include <MPU6050_light.h>
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "model.h"  // your quantized TFLite model

// --- TENSORFLOW LITE SETUP ---
const int kTensorArenaSize = 20 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
const int WINDOW_SIZE = 50;
const int FEATURES = 1;

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// --- MPU6050 SETUP ---
MPU6050 mpu(Wire);
float sensor_buffer[WINDOW_SIZE]; // single feature buffer
int buffer_index = 0;
bool is_recording = false;
const float ACCEL_THRESHOLD = 0.02; // small threshold to start recording

// --- MinMaxScaler parameters and anomaly threshold ---
// Replace these values with actual numbers from your Python code
const float SCALER_MIN = 0.0;    // min of training RMS
const float SCALER_MAX = 0.02;   // max of training RMS
const float THRESHOLD  = 0.05;   // mean + 3*std of reconstruction error

void setup() {
  Serial.begin(115200);

  Wire.setPins(4, 5); // SDA=4, SCL=5
  Wire.begin();
  while(!Serial) delay(10);

  Wire.begin();
  if (mpu.begin() != 0) {
    Serial.println("MPU6050 not detected!");
    while(1) delay(10);
  }
  mpu.calcOffsets();
  Serial.println("MPU6050 ready.");

  // Load TFLite model
  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("Model schema mismatch!");
    while(1);
  }

  // Setup interpreter
  static tflite::MicroMutableOpResolver<10> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddSub();
  resolver.AddMul();
  resolver.AddMean();

  static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("Tensor allocation failed!");
    while(1);
  }

  input = interpreter->input(0);
  output = interpreter->output(0);
  Serial.println("TFLite Interpreter ready.");
}

void loop() {
  mpu.update();

  float accel_x = mpu.getAccX();
  
  // Start recording if movement detected
  if (!is_recording && fabs(accel_x) > ACCEL_THRESHOLD) {
    is_recording = true;
    buffer_index = 0;
    Serial.println("Recording...");
  }

  if (is_recording) {
    // Fill buffer with single feature
    sensor_buffer[buffer_index] = accel_x;
    buffer_index++;
    delay(10); // sample rate ~100Hz

    if (buffer_index >= WINDOW_SIZE) {
      // 1️⃣ Compute RMS over the window
      float rms = 0;
      for (int i = 0; i < WINDOW_SIZE; i++) rms += sensor_buffer[i] * sensor_buffer[i];
      rms = sqrt(rms / WINDOW_SIZE);

      // 2️⃣ Normalize RMS using MinMaxScaler parameters
      float scaled = (rms - SCALER_MIN) / (SCALER_MAX - SCALER_MIN);
      input->data.f[0] = scaled;

      // 3️⃣ Run inference
      if (interpreter->Invoke() != kTfLiteOk) {
        Serial.println("Inference failed!");
        is_recording = false;
        return;
      }

      // 4️⃣ Compute reconstruction error (MSE)
      float recon = output->data.f[0];
      float mse = (scaled - recon) * (scaled - recon);

      // 5️⃣ Compare with threshold
      if (mse > THRESHOLD) Serial.println("⚠️ Anomaly detected!");
      else Serial.println("✅ Normal reading");

      // Reset
      is_recording = false;
      buffer_index = 0;
      delay(500); // cooldown
    }
  }
}
