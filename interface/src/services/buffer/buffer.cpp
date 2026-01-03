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
DS18B20_Simple ds18b20(DS18B20_PIN);
MAX471 max471;

void vbufferTask(void* pvParameters)
{
    char buffer[BUFFER_SIZE];
    xQueueHandle data_queue = get_data_queue();
    MachineData_t sensorData;
    while (1)
    {
        // Collect data from sensors
        if(xQueueReceive(data_queue, &sensorData, portMAX_DELAY) != pdPASS)
        {
            continue; // Failed to receive data
        }
        float rpm = MAX_SPEED; // Placeholder for actual RPM reading
        float health_score = 87.5; // Placeholder for actual health score calculation
        const char* fault_type = "healthy"; // Placeholder for actual fault type determination 
        // Format data into JSON
        snprintf(buffer, BUFFER_SIZE,
                 "{\"timestamp\": %lu,\
                  \"rpm\": %.2f, \
                  \"temp\": %.2f,\
                   \"current\": %.2f,\
                    \"voltage\": %.2f, "
                 "\"gforce total\": %.2f,\
                  \"gforce rms\": %.2f, \
                  \"vib_z\": %.2f, \
                  \"health_score\": %.2f,\
                   \"fault_type\": \
                   \"%s\"}",
                 (millis() / 1000), 
                 rpm, 
                 sensorData.temperature, 
                 sensorData.current, 
                 sensorData.voltage, 
                 sensorData.gforce, 
                 sensorData.gforce_rms, 0.0f, health_score, fault_type);

        // Publish data via MQTT
        mqttpublish("machine/data", buffer);

        // Wait for 2 seconds before next transmission
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}