#include <Wire.h>
#include <MPU6050_light.h>

// --- TFLITE ---
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "model.h"

// -----------------------------
#define WINDOW_SIZE 64
#define ERROR_THRESHOLD 0.01

// -----------------------------
MPU6050 mpu(Wire);

// Feature buffer
float acc_buffer[WINDOW_SIZE];
int index_buf = 0;
bool buffer_full = false;

// -----------------------------
constexpr int kTensorArenaSize = 40 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

const tflite::Model* model;
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input;
TfLiteTensor* output;

// -----------------------------
float accMagnitude() {
  float ax = mpu.getAccX();
  float ay = mpu.getAccY();
  float az = mpu.getAccZ();
  return sqrt(ax*ax + ay*ay + az*az);
}

// -----------------------------
void setup() {
  Serial.begin(115200);

  Wire.setPins(4, 5); // SDA=4, SCL=5
  Wire.begin();

  if (mpu.begin() != 0) {
    Serial.println("❌ MPU failed");
    while (1);
  }

  delay(1000);
  mpu.calcOffsets();

  model = tflite::GetModel(model_tflite);

  static tflite::MicroMutableOpResolver<10> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddReshape();
  resolver.AddAdd();
  resolver.AddSub();

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize
  );
  interpreter = &static_interpreter;

  interpreter->AllocateTensors();
  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("✅ System Ready");
}

// -----------------------------
void loop() {
  mpu.update();

  acc_buffer[index_buf++] = accMagnitude();

  if (index_buf >= WINDOW_SIZE) {
    index_buf = 0;
    buffer_full = true;
  }

  if (!buffer_full) return;

  // ---- Quantize Input ----
  float in_scale = input->params.scale;
  int in_zero = input->params.zero_point;

  for (int i = 0; i < WINDOW_SIZE; i++) {
    input->data.int8[i] =
      (int8_t)(acc_buffer[i] / in_scale + in_zero);
  }

  interpreter->Invoke();

  // ---- Error ----
  float error = 0;
  float out_scale = output->params.scale;
  int out_zero = output->params.zero_point;

  for (int i = 0; i < WINDOW_SIZE; i++) {
    float recon =
      (output->data.int8[i] - out_zero) * out_scale;
    float diff = acc_buffer[i] - recon;
    error += diff * diff;
  }

  error /= WINDOW_SIZE;

  Serial.print("Error: ");
  Serial.println(error, 6);

  if (error > ERROR_THRESHOLD)
    Serial.println("⚠️ FAULT DETECTED");
  else
    Serial.println("✅ NORMAL");

  delay(100);
}
