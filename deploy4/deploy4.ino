#include <Wire.h>
#include <MPU6050_light.h>

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "fault_model.h"
#include "scaler_params.h"

/* ---------------- CONFIG ---------------- */
#define WINDOW_SIZE     64
#define NUM_FEATURES    46   // from scaler_params.h
#define NUM_CLASSES     7
#define SAMPLE_DELAY_MS 10

/* ---------------- MPU ---------------- */
MPU6050 mpu(Wire);

/* ---------------- TFLite ---------------- */
constexpr int kTensorArenaSize = 25 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

const tflite::Model* model;
tflite::MicroInterpreter* interpreter;
TfLiteTensor* input;
TfLiteTensor* output;

/* ---------------- Buffers ---------------- */
float ax_buf[WINDOW_SIZE], ay_buf[WINDOW_SIZE], az_buf[WINDOW_SIZE];
float gx_buf[WINDOW_SIZE], gy_buf[WINDOW_SIZE], gz_buf[WINDOW_SIZE];
float rpm_buf[WINDOW_SIZE];
float current_buf[WINDOW_SIZE];
float temp_buf[WINDOW_SIZE];

int buf_index = 0;

/* ---------------- Labels ---------------- */
const char* fault_labels[NUM_CLASSES] = {
  "Normal",
  "Imbalance",
  "Misalignment",
  "Bearing Fault",
  "Mechanical Looseness",
  "Overload",
  "Overheat"
};

/* ---------------- SENSOR STUBS ---------------- */
// Replace these with real sensors
float readRPM()        { return 1800.0; }
float readCurrent()    { return 0.45; }
float readTemperature(){ return 40.0; }

/* ---------------- FEATURE HELPERS ---------------- */
float rms(float* x) {
  float s = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) s += x[i] * x[i];
  return sqrt(s / WINDOW_SIZE);
}

float p2p(float* x) {
  float mn = x[0], mx = x[0];
  for (int i = 1; i < WINDOW_SIZE; i++) {
    if (x[i] < mn) mn = x[i];
    if (x[i] > mx) mx = x[i];
  }
  return mx - mn;
}

float stddev(float* x) {
  float m = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) m += x[i];
  m /= WINDOW_SIZE;

  float s = 0;
  for (int i = 0; i < WINDOW_SIZE; i++)
    s += (x[i] - m) * (x[i] - m);

  return sqrt(s / WINDOW_SIZE);
}

/* ---------------- SETUP ---------------- */
void setup() {
  Serial.begin(115200);

  Wire.setPins(4, 5); // SDA=4, SCL=5
  Wire.begin();

  mpu.begin();
  mpu.calcOffsets();

  model = tflite::GetModel(fault_model);
  static tflite::MicroMutableOpResolver<10> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddSoftmax();

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  interpreter->AllocateTensors();
  input = interpreter->input(0);
  output = interpreter->output(0);

  Serial.println("✅ Fault Detection System Ready");
}

/* ---------------- LOOP ---------------- */
void loop() {
  mpu.update();

  ax_buf[buf_index] = mpu.getAccX();
  ay_buf[buf_index] = mpu.getAccY();
  az_buf[buf_index] = mpu.getAccZ();

  gx_buf[buf_index] = mpu.getGyroX();
  gy_buf[buf_index] = mpu.getGyroY();
  gz_buf[buf_index] = mpu.getGyroZ();

  rpm_buf[buf_index]     = readRPM();
  current_buf[buf_index] = readCurrent();
  temp_buf[buf_index]    = readTemperature();

  buf_index++;

  if (buf_index >= WINDOW_SIZE) {
    runInference();
    buf_index = 0;
  }

  delay(SAMPLE_DELAY_MS);
}

/* ---------------- INFERENCE ---------------- */
void runInference() {
  float feat[NUM_FEATURES];
  int k = 0;

  /* ---- Accel features ---- */
  feat[k++] = rms(ax_buf);
  feat[k++] = p2p(ax_buf);
  feat[k++] = stddev(ax_buf);

  feat[k++] = rms(ay_buf);
  feat[k++] = p2p(ay_buf);
  feat[k++] = stddev(ay_buf);

  feat[k++] = rms(az_buf);
  feat[k++] = p2p(az_buf);
  feat[k++] = stddev(az_buf);

  /* ---- Gyro ---- */
  feat[k++] = rms(gx_buf);
  feat[k++] = rms(gy_buf);
  feat[k++] = rms(gz_buf);

  /* ---- Cross-domain ---- */
  float vib_rms = sqrt(
    rms(ax_buf)*rms(ax_buf) +
    rms(ay_buf)*rms(ay_buf) +
    rms(az_buf)*rms(az_buf));

  feat[k++] = vib_rms / (rpm_buf[0] + 1e-3);
  feat[k++] = current_buf[0] * rpm_buf[0];
  feat[k++] = temp_buf[0];

  /* ---- Normalize ---- */
  for (int i = 0; i < NUM_FEATURES; i++) {
    feat[i] = (feat[i] - scaler_mean[i]) / scaler_std[i];
  }

  /* ---- Quantize input ---- */
  float scale = input->params.scale;
  int zero = input->params.zero_point;

  for (int i = 0; i < NUM_FEATURES; i++) {
    int8_t q = (int8_t)(feat[i] / scale + zero);
    input->data.int8[i] = q;
  }

  interpreter->Invoke();

  /* ---- Output ---- */
  int best = 0;
  int8_t maxv = output->data.int8[0];

  for (int i = 1; i < NUM_CLASSES; i++) {
    if (output->data.int8[i] > maxv) {
      maxv = output->data.int8[i];
      best = i;
    }
  }

  Serial.print("⚠️ Fault Detected: ");
  Serial.println(fault_labels[best]);
}
