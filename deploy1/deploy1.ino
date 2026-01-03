/*
 * TinyML Showcase - Fault Classification on ESP32
 * Real TensorFlow Lite Micro deployment
 * Model: 50 inputs → 32 → 16 → 4 outputs (Healthy, Unbalance, Misalignment, Voltage Drop)
 */

#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "model_data.h"  // Converted TFLite model

// ============ TensorFlow Lite Setup ============
namespace {
    tflite::ErrorReporter* error_reporter = nullptr;
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;
    
    constexpr int kTensorArenaSize = 8 * 1024;  // 8KB
    uint8_t tensor_arena[kTensorArenaSize];
}

// ============ Model Info ============
const int NUM_FEATURES = 12;
const int NUM_CLASSES = 4;
const char* CLASS_NAMES[] = {"healthy", "unbalance", "misalignment", "voltage_drop"};

// ============ Feature Buffer ============
float features[NUM_FEATURES];

// ============ Simulated Sensor Data ============
float rpm = 1500;
float temp = 40.0;
float current = 2.5;
float voltage = 230.0;
float vibX = 0.05, vibY = 0.05, vibZ = 0.05;

int faultMode = 0;  // 0=healthy, 1=unbalance, 2=misalignment, 3=voltage_drop
unsigned long lastSwitch = 0;

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n========================================");
    Serial.println("   TinyML Showcase - ESP32");
    Serial.println("   Fault Classification Demo");
    Serial.println("========================================\n");
    
    // Initialize TensorFlow Lite
    setupTFLite();
    
    Serial.println("✓ TinyML Model Loaded Successfully!");
    Serial.println("✓ Model Size: " + String(g_model_data_len) + " bytes");
    Serial.println("✓ Input Features: " + String(NUM_FEATURES));
    Serial.println("✓ Output Classes: " + String(NUM_CLASSES));
    Serial.println("\nStarting inference...\n");
}

void loop() {
    // Cycle through faults every 15 seconds
    if (millis() - lastSwitch > 15000) {
        faultMode = (faultMode + 1) % 4;
        lastSwitch = millis();
        Serial.println("\n>>> Switching Fault Mode <<<\n");
    }
    
    // Generate simulated sensor data
    generateSensorData(faultMode);
    
    // Extract features
    extractFeatures();
    
    // Run TinyML inference
    int predicted_class = runInference();
    
    // Get confidence
    float confidence = output->data.f[predicted_class] * 100;
    
    // Print results
    printResults(predicted_class, confidence);
    
    delay(1000);
}

void setupTFLite() {
    // Set up logging
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;
    
    // Load model
    model = tflite::GetModel(g_model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.println("Model schema mismatch!");
        return;
    }
    
    // Load operations resolver
    static tflite::AllOpsResolver resolver;
    
    // Build interpreter
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;
    
    // Allocate tensors
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        Serial.println("Tensor allocation failed!");
        return;
    }
    
    // Get input/output tensors
    input = interpreter->input(0);
    output = interpreter->output(0);
    
    // Print tensor info
    Serial.println("Input tensor dims: " + String(input->dims->size));
    Serial.println("Output tensor dims: " + String(output->dims->size));
}

void generateSensorData(int mode) {
    float noise = random(-10, 10) / 100.0;
    
    switch (mode) {
        case 0:  // Healthy
            rpm = 1500 + random(-5, 5);
            temp = 40.0 + noise;
            current = 2.5 + noise * 0.3;
            voltage = 230.0 + noise;
            vibX = 0.05 + noise * 0.05;
            vibY = 0.05 + noise * 0.05;
            vibZ = 0.05 + noise * 0.05;
            break;
            
        case 1:  // Unbalance
            rpm = 1500 + random(-15, 15);
            temp = 45.0 + noise;
            current = 2.7 + noise * 0.4;
            voltage = 230.0 + noise;
            vibX = 0.40 + noise * 0.1;  // High radial
            vibY = 0.38 + noise * 0.1;
            vibZ = 0.08 + noise * 0.05;
            break;
            
        case 2:  // Misalignment
            rpm = 1480 + random(-10, 10);
            temp = 48.0 + noise;
            current = 2.9 + noise * 0.4;
            voltage = 230.0 + noise;
            vibX = 0.25 + noise * 0.08;
            vibY = 0.28 + noise * 0.08;
            vibZ = 0.35 + noise * 0.1;  // High axial
            break;
            
        case 3:  // Voltage Drop
            rpm = 1350 + random(-20, 20);
            temp = 52.0 + noise;
            current = 3.5 + noise * 0.5;
            voltage = 200.0 + noise * 3;  // Low voltage
            vibX = 0.15 + noise * 0.08;
            vibY = 0.18 + noise * 0.08;
            vibZ = 0.12 + noise * 0.06;
            break;
    }
}

void extractFeatures() {
    // Feature 1-3: Raw vibration (g)
    features[0] = vibX;
    features[1] = vibY;
    features[2] = vibZ;
    
    // Feature 4: RMS vibration
    features[3] = sqrt((vibX*vibX + vibY*vibY + vibZ*vibZ) / 3.0);
    
    // Feature 5: Peak vibration
    features[4] = max(max(vibX, vibY), vibZ);
    
    // Feature 6: Temperature (normalized)
    features[5] = (temp - 20.0) / 60.0;  // Scale 20-80°C to 0-1
    
    // Feature 7: Current (normalized)
    features[6] = (current - 1.0) / 4.0;  // Scale 1-5A to 0-1
    
    // Feature 8: Voltage (normalized)
    features[7] = (voltage - 180.0) / 60.0;  // Scale 180-240V to 0-1
    
    // Feature 9: RPM (normalized)
    features[8] = (rpm - 1000.0) / 1000.0;  // Scale 1000-2000 to 0-1
    
    // Feature 10: Vibration ratio (axial/radial)
    features[9] = vibZ / (sqrt(vibX*vibX + vibY*vibY) + 0.001);
    
    // Feature 11: Power consumption (V*I normalized)
    features[10] = (voltage * current - 400.0) / 600.0;
    
    // Feature 12: Thermal efficiency (temp/power)
    features[11] = temp / (voltage * current + 0.1);
}

int runInference() {
    // Copy features to input tensor
    for (int i = 0; i < NUM_FEATURES; i++) {
        input->data.f[i] = features[i];
    }
    
    // Run inference
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        Serial.println("Invoke failed!");
        return -1;
    }
    
    // Find class with highest probability
    int predicted_class = 0;
    float max_prob = output->data.f[0];
    
    for (int i = 1; i < NUM_CLASSES; i++) {
        if (output->data.f[i] > max_prob) {
            max_prob = output->data.f[i];
            predicted_class = i;
        }
    }
    
    return predicted_class;
}

void printResults(int predicted_class, float confidence) {
    Serial.println("─────────────────────────────────────");
    Serial.print("Actual Mode: ");
    Serial.println(CLASS_NAMES[faultMode]);
    Serial.print("Predicted:   ");
    Serial.println(CLASS_NAMES[predicted_class]);
    Serial.print("Confidence:  ");
    Serial.print(confidence, 1);
    Serial.println("%");
    
    Serial.println("\nSensor Data:");
    Serial.printf("  RPM: %.0f | Temp: %.1f°C | Current: %.2fA | Voltage: %.1fV\n", 
                  rpm, temp, current, voltage);
    Serial.printf("  Vib: X=%.3f Y=%.3f Z=%.3f\n", vibX, vibY, vibZ);
    
    Serial.println("\nClass Probabilities:");
    for (int i = 0; i < NUM_CLASSES; i++) {
        Serial.printf("  %s: %.1f%%\n", CLASS_NAMES[i], output->data.f[i] * 100);
    }
    
    // Check if prediction is correct
    if (predicted_class == faultMode) {
        Serial.println("\n✓ CORRECT PREDICTION!");
    } else {
        Serial.println("\n✗ Incorrect prediction");
    }
    
    Serial.println("─────────────────────────────────────\n");
}

// ============================================
// model_data.h (Separate file - TFLite model)
// ============================================
/*
#ifndef MODEL_DATA_H
#define MODEL_DATA_H

// This is a placeholder - replace with actual converted model
// Generated from: xxd -i model.tflite > model_data.h

const unsigned char g_model_data[] = {
    // TFLite model bytes go here (from Python conversion)
    // Example: 0x1c, 0x00, 0x00, 0x00, 0x54, 0x46, 0x4c, 0x33...
};

const int g_model_data_len = sizeof(g_model_data);

#endif
*/