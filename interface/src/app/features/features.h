#ifndef FEATURES_H
#define FEATURES_H
 #include "../inference/model_data.h"
 /*

 */
    #include "../../../src/config.h"
    void vFeaturesTask(void *pvParameters);
    float normalize(float value, float max_val);
#endif