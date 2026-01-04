#ifndef SCHEDULER_H
#define SCHEDULER_H
    #include <Arduino.h>
    #include "../inference/inference.h"
    #include "../features/features.h"
    #include "../../services/mqtt/mqtt.h"
    #include "../../services/buffer/buffer.h"
    #include "../../hal/encoder/encoder.h"
    void setupScheduler();
#endif 