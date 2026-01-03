////mine and i made it for all features

#include <Wire.h>
#include <MPU6050_light.h>

// --- TENSORFLOW LITE INCLUDES ---
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

// --- LOCAL FILES ---
#include "model.h"

// -----------------------------
// CONFIGURATION
// -----------------------------
#define WINDOW_SIZE     64
#define FEATURES        3    // RMS, Peak, Mean Current
#define CURRENT_PIN     A0
#define ERROR_THRESHOLD 0.015
#define SAMPLE_DELAY    10   // ~100Hz

// -----------------------------
// GLOBAL OBJECTS
// -----------------------------
MPU6050 mpu(Wire);

// Sensor buffer
float feature_buffer[WINDOW_SIZE][FEATURES];
int buffer_index = 0;
bool buffer_ready = false;

// -----------------------------
// TENSORFLOW OBJECTS
// -----------------------------
constexpr int kTensorArenaSize = 20 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// -----------------------------
// HELPER FUNCTIONS
// -----------------------------
float computeRMS(float ax, float ay, float az) {
  return sqrt(ax * ax + ay * ay + az * az);
}

float readCurrent() {
  return analogRead(CURRENT_PIN) * (3.3 / 4095.0); // ESP32 ADC
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("\n--- ESP32 Stepper Anomaly Detection ---");

  // -----------------------------
  // MPU INIT
  // -----------------------------
  Wire.setPins(4, 5);  // SDA = 4, SCL = 5
  Wire.begin();

  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("❌ MPU6050 NOT FOUND");
    while (1);
  }

  Serial.println("✅ MPU6050 OK");
  Serial.println("Calibrating...");
  delay(1000);
  mpu.calcOffsets();
  Serial.println("Calibration Done");

  // -----------------------------
  // LOAD MODEL
  // -----------------------------
  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    Serial.println("❌ Model schema mismatch");
    while (1);
  }

  // -----------------------------
  // OP RESOLVER
  // -----------------------------
  static tflite::MicroMutableOpResolver<20> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddReshape();
  resolver.AddAdd();
  resolver.AddMul();
  resolver.AddMean();
  resolver.AddSub();

  // -----------------------------
  // INTERPRETER
  // -----------------------------
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize
  );
  interpreter = &static_interpreter;

  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("❌ AllocateTensors failed");
    while (1);
  }

  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("✅ TFLite Ready");
}

void loop() {
  mpu.update();

  // -----------------------------
  // COLLECT SENSOR DATA
  // -----------------------------
  float ax = mpu.getAccX();
  float ay = mpu.getAccY();
  float az = mpu.getAccZ();

  float rms = computeRMS(ax, ay, az);
  float peak = rms;
  float current = readCurrent();

  feature_buffer[buffer_index][0] = rms;
  feature_buffer[buffer_index][1] = peak;
  feature_buffer[buffer_index][2] = current;

  buffer_index++;

  if (buffer_index >= WINDOW_SIZE) {
    buffer_index = 0;
    buffer_ready = true;
  }

  if (!buffer_ready) {
    delay(SAMPLE_DELAY);
    return;
  }

  // -----------------------------
  // QUANTIZE INPUT
  // -----------------------------
  float scale = input->params.scale;
  int zero = input->params.zero_point;

  for (int i = 0; i < WINDOW_SIZE; i++) {
    for (int j = 0; j < FEATURES; j++) {
      int idx = i * FEATURES + j;
      input->data.int8[idx] =
        (int8_t)(feature_buffer[i][j] / scale + zero);
    }
  }

  // -----------------------------
  // RUN INFERENCE
  // -----------------------------
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("❌ Invoke failed");
    return;
  }

  // -----------------------------
  // RECONSTRUCTION ERROR
  // -----------------------------
  float error = 0.0;
  float out_scale = output->params.scale;
  int out_zero = output->params.zero_point;

  for (int i = 0; i < WINDOW_SIZE; i++) {
    for (int j = 0; j < FEATURES; j++) {
      int idx = i * FEATURES + j;
      float recon =
        (output->data.int8[idx] - out_zero) * out_scale;
      float diff = feature_buffer[i][j] - recon;
      error += diff * diff;
    }
  }

  error /= (WINDOW_SIZE * FEATURES);

  // -----------------------------
  // ALERT
  // -----------------------------
  Serial.print("Reconstruction Error: ");
  Serial.println(error, 6);

  if (error > ERROR_THRESHOLD) {
    Serial.println("⚠️ MISALIGNMENT / FAULT DETECTED");
  } else {
    Serial.println("✅ Normal Operation");
  }

  delay(SAMPLE_DELAY);
}
