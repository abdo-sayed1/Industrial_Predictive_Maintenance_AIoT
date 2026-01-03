#ifndef FEATURES_H
#define FEATURES_H
 #include "../inference/model_data.h"
 /*

 */
    #include "../../../src/config.h"
    const char* fault_labels[NUM_CLASSES] = 
    {
    "Normal",
    "Imbalance",
    "Misalignment",
    "Bearing Fault",
    "Mechanical Looseness",
    "Overload",
    "Overheat"
    };
#endif