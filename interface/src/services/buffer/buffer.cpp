#include "buffer.h"
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
void vbufferTask(void* pvParameters)
{
    char buffer[BUFFER_SIZE];
    while (1)
    {
        // Collect data from sensors
        float rpm = 1450.0; // Placeholder for actual RPM reading
        float temp = 42.3;  // Placeholder for actual temperature reading
        float current = 2.8; // Placeholder for actual current reading
        float voltage = 230.5; // Placeholder for actual voltage reading
        float vib_x = 0.15; // Placeholder for actual vibration x-axis reading
        float vib_y = 0.12; // Placeholder for actual vibration y-axis reading
        float vib_z = 0.08; // Placeholder for actual vibration z-axis reading
        float health_score = 87.5; // Placeholder for actual health score calculation
        const char* fault_type = "healthy"; // Placeholder for actual fault type determination

        // Format data into JSON
        snprintf(buffer, BUFFER_SIZE,
                 "{\"timestamp\": %lu, \"rpm\": %.2f, \"temp\": %.2f, \"current\": %.2f, \"voltage\": %.2f, "
                 "\"vib_x\": %.2f, \"vib_y\": %.2f, \"vib_z\": %.2f, \"health_score\": %.2f, \"fault_type\": \"%s\"}",
                 millis() / 1000, rpm, temp, current, voltage, vib_x, vib_y, vib_z, health_score, fault_type);

        // Publish data via MQTT
        mqttpublish("machine/data", buffer);

        // Wait for 2 seconds before next transmission
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}