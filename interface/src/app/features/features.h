#ifndef FEATURES_H
#define FEATURES_H
 #include <Arduino.h>
 #include <TensorFlowLite_ESP32.h>
 #include "tensorflow/lite/builtin_ops.h"
 #include "tensorflow/lite/micro/kernels/micro_ops.h"
 #include "tensorflow/lite/micro/micro_error_reporter.h"
 #include "tensorflow/lite/micro/micro_interpreter.h"
 #include "tensorflow/lite/micro/all_ops_resolver.h"
 #include "../inference/model_data.h"
 /*

 */
 void setupTFLite();
#endif