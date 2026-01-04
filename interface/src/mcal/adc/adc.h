#ifndef ADC_H
#define ADC_H
#if __cplusplus 
extern "C" {
#endif
    unsigned int ADC_Read(void);    
    void SensorInit(void);
    #define PotInit() SensorInit()
    #define ADC_Init() SensorInit()
    #define ADC 34
    float ReadVoltage();
#if __cplusplus 
}
#endif
#endif