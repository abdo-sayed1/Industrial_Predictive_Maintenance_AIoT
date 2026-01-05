#include "buffer.h"
#include "../../hal/electrical_sensors/max471.h"
#include "../../hal/mpu6050/mpu6050.h"
#include "../../hal/ds18b20/ds18b20.h"
/*
Mqtt message format to send data to the server
{
  "timestamp": 1640000000,
  "rpm": 1450,
  "temp": 42.3,
  "current": 2.8,
  "voltage": 230.5,
  "vib_x": 0.15,
  "vib_y": 0.12,
  "vib_z": 0.08,
  "health_score": 87.5,
  "fault_type": "healthy"
}
*/
portMUX_TYPE mqttMutex = portMUX_INITIALIZER_UNLOCKED;
DS18B20_Simple ds18b20(DS18B20_PIN);
MAX471 max471;

void vbufferTask(void* pvParameters)
{
    char buffer[BUFFER_SIZE<<4]; // Increased buffer size for larger JSON payload
    xQueueHandle data_queue = get_data_queue();
    MachineData_t sensorData;
    MachineData_t DataBuffer[20];
    uint8_t i=0;
    TickType_t xLastTickCount;
    const char** mfault_labels = getfaultlabels();
    while (1)
    {
        // Collect data from sensors
        if(xQueueReceive(data_queue, &sensorData, portMAX_DELAY) != pdPASS)
        {
            continue; // Failed to receive data
        }
        // Store in buffer
        DataBuffer[i++] = sensorData;
        if(i < 20)
        {
            continue; // Buffer not full yet
        }
        portENTER_CRITICAL(&mqttMutex);
        i=0; // Reset index for next batch
        // Prepare JSON payload
        unsigned long timestamp = millis() / 1000; // Current time in seconds
        /*
        include all 20 samples in the JSON which contain health score and fault type
        */
        int len = snprintf(buffer, sizeof(buffer), "{\"timestamp\": %lu, \"data\": [", timestamp);
        for(uint8_t j=0; j<20; j++)
        {
            len += snprintf(buffer + len, sizeof(buffer) - len,
                "{\"A_x\":%.2f,\"A_y\":%.2f,\"A_z\":%.2f,\"G_x\":%.2f,\"G_y\":%.2f,\"G_z\":%.2f,\"Total gforce\": %.3f, \"temperature\": %.2f, \"current\": %.2f, \"voltage\": %.2f, \"speed\": %.2f, \"gforce_rms\": %.3f, \"isAnomaly\": %s, \"faultType\": %d,\"Health Score\": %.2f,\"Fault Label\":%s}%s",
                DataBuffer[j].a_x,
                DataBuffer[j].a_y,
                DataBuffer[j].a_z,
                DataBuffer[j].g_x,
                DataBuffer[j].g_y,
                DataBuffer[j].g_z,
                DataBuffer[j].gforce,
                DataBuffer[j].temperature,
                DataBuffer[j].current,
                DataBuffer[j].voltage,
                DataBuffer[j].speed,
                DataBuffer[j].gforce_rms,
                DataBuffer[j].isAnomaly ? "true" : "false",
                DataBuffer[j].faultType,
                DataBuffer[j].healthScore,
                mfault_labels[DataBuffer[j].faultType],
                (j < 19) ? "," : ""
            );
        }
        len += snprintf(buffer + len, sizeof(buffer) - len, "]}");
        // Publish data via MQTT
        mqttpublish("machine/data", buffer);
        portEXIT_CRITICAL(&mqttMutex);
        // Wait for 2 seconds before next transmission
        //xLastTickCount = xTaskGetTickCount();
        //vTaskDelayUntil(&xLastTickCount,pdMS_TO_TICKS(2000));
    }
}