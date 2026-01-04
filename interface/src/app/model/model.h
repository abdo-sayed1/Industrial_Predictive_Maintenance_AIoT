#ifndef MODEL_H
#define MODEL_H
    #include <Arduino.h>
    const uint8_t* getfaultmodel();
    const unsigned short int getfaultmodellength();
    const char** getfaultlabels();
    #include "../../../src/config.h"
#endif 