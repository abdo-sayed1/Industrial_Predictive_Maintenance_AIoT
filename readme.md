# ⚙️ MOTOR FAULT DETECTION SYSTEM

## 🚀 System Architecture
This project follows a modular IoT architecture designed for high-performance motor monitoring.

1. **Broker Connection:** Receives JSON telemetry via MQTT (Topic: `motor/sensor/data`).
2. **Processing:** Node-RED logic and alerting engine.
3. **Frontend:** Custom HTML5 dashboard connected via WebSockets.

## 🛠️ SETUP INSTRUCTIONS

### 1. Backend (Node-RED)
* **Palette:** Install `node-red-node-email` and `node-red-node-twilio`.
* **Import:** Load the `flows.json` file.
* **Broker:** Update the `mqtt-broker` configuration node to your EC2 IP.

### 2. Frontend (HTML Dashboard)
* Open `dashboard.html` in a text editor.
* Search for the WebSocket URL and change it to: `ws://<YOUR_EC2_IP>:1880/ws/motor`.

## 📊 DATA SCHEMA

**Topic:** `motor/sensor/data`
```json
{
    "ax": 0.0, 
    "ay": 0.0,
    "az": 0.0,
    "gx": 0.0,
    "gy": 0.0,
    "gz": 0.0,
    "temp": 35.5, 
    "current": 2.5,
    "rpm": 1500,
    "prediction": 0,
    "confidence": 0.95,
    "probabilities": [0.95, 0.02, 0.01, 0.01, 0.005, 0.003, 0.002]
}
```

**Fault Indices:**
- `0`: Normal
- `1`: Imbalance
- `2`: Misalignment
- `3`: Bearing
- `4`: Looseness
- `5`: Overload
- `6`: Overheat

## 🔒 SECURITY & PRIVACY
* **Scrubbing:** Always uncheck "Export Credentials" when saving `flows.json` from Node-RED.
* **Rate Limit:** Alerts are capped at 1 per minute via a Delay Node to prevent notification storms.

## ❓ TROUBLESHOOTING
* **Connection Failed:** Ensure your AWS Security Group has Port `1880` open for WebSockets and Port `1883` for MQTT.
* **No Data in HTML:** Check the browser console (F12) to verify the WebSocket handshake is successful.
* **Charts Not Updating:** Verify data format matches the schema above - all 9 sensor values must be present.
* **MPU6050 Issues:** Check I2C connections (SDA→GPIO21, SCL→GPIO22) and run I2C scanner to verify address.
