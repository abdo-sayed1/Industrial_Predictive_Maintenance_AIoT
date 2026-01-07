[Presentation](presentation/index.html) — static HTML slides.

<html>
  <svg xmlns="http://www.w3.org/2000/svg" width="1000" height="900" viewBox="0 0 1000 900" font-family="Segoe UI, Arial, sans-serif">
  <defs>
    <marker id="arrow" viewBox="0 0 10 10" refX="10" refY="5" markerWidth="6" markerHeight="6" orient="auto">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="#444"/>
    </marker>
    <style>
      .box { fill:#ffffff; stroke:#444; stroke-width:1.5; rx:8; ry:8; }
      .title { font-weight:bold; font-size:14px; fill:#000; }
      .sub { font-size:12px; fill:#444; }
      .label { font-size:11px; fill:#0056b3; font-weight: bold; }
      .highlight { fill:#fff3cd; stroke:#c69000; }
      .system-box { fill:#f8f9fa; stroke:#dee2e6; stroke-dasharray: 4; }
    </style>
  </defs>

  <rect class="system-box" x="10" y="10" width="980" height="620" rx="10"/>
  <text x="20" y="30" class="sub" font-weight="bold">ESP32 Firmware Environment (FreeRTOS)</text>

  <g transform="translate(40,60)">
    <rect class="box" width="220" height="140"/>
    <text class="title" x="110" y="25" text-anchor="middle">Physical Sensors</text>
    <text class="sub" x="15" y="55">• MPU6050 (Vib/Gyro)</text>
    <text class="sub" x="15" y="75">• DS18B20 (Temp)</text>
    <text class="sub" x="15" y="95">• MAX471 (Current)</text>
    <text class="sub" x="15" y="115">• Encoder / ADC</text>
  </g>

  <g transform="translate(300,60)">
    <rect class="box" width="220" height="140"/>
    <text class="title" x="110" y="25" text-anchor="middle">HAL / Drivers</text>
    <text class="sub" x="15" y="55">• mpu6050.cpp</text>
    <text class="sub" x="15" y="75">• ds18b20.cpp</text>
    <text class="sub" x="15" y="95">• max471.cpp / adc.cpp</text>
    <text class="sub" x="15" y="115">• encoder.cpp</text>
  </g>

  <g transform="translate(560,60)">
    <rect class="box" width="380" height="60"/>
    <text class="title" x="190" y="25" text-anchor="middle">Task: Data Acquisition</text>
    <text class="sub" x="190" y="45" text-anchor="middle">Reads HAL → Serializes Data</text>
  </g>

  <g transform="translate(560,150)">
    <rect class="box" width="380" height="50" style="stroke-dasharray: 5,5;"/>
    <text class="title" x="190" y="22" text-anchor="middle">xRawDataQueues</text>
    <text class="sub" x="190" y="40" text-anchor="middle">RTOS Buffer (Inter-task Communication)</text>
  </g>

  <g transform="translate(560,250)">
    <rect class="box" width="380" height="80"/>
    <text class="title" x="190" y="25" text-anchor="middle">Task: Feature Engineering</text>
    <text class="sub" x="190" y="50" text-anchor="middle">normalize(), FFT, Scaling</text>
    <text class="sub" x="190" y="68" text-anchor="middle">Outputs: xFeatureQueue</text>
  </g>

  <g transform="translate(40,380)">
    <rect class="box" width="320" height="150"/>
    <text class="title" x="160" y="25" text-anchor="middle">Task: Inference Engine</text>
    <text class="sub" x="20" y="55">1. Pop xFeatureQueue</text>
    <text class="sub" x="20" y="75">2. Map to Input Tensor</text>
    <text class="sub" x="20" y="95">3. Invoke Interpreter</text>
    <text class="sub" x="20" y="115">4. Push Result to xProcessedQueue</text>
  </g>

  <g transform="translate(400,380)">
    <rect class="box highlight" width="280" height="150"/>
    <text class="title" x="140" y="25" text-anchor="middle">TensorFlow Lite Micro</text>
    <text class="sub" x="15" y="55">• AllOpsResolver</text>
    <text class="sub" x="15" y="75">• MicroInterpreter</text>
    <text class="sub" x="15" y="95">• Arena: kTensorArenaSize</text>
    <text class="sub" x="15" y="115">• Model: g_model_data</text>
  </g>

  <g transform="translate(720,380)">
    <rect class="box" width="220" height="70"/>
    <text class="title" x="110" y="25" text-anchor="middle">Model Data</text>
    <text class="sub" x="110" y="50" text-anchor="middle">model_data.cpp (Binary)</text>
  </g>

  <g transform="translate(40,560)">
    <rect class="box" width="900" height="60"/>
    <text class="title" x="450" y="25" text-anchor="middle">Action / Output Layer</text>
    <text class="sub" x="450" y="45" text-anchor="middle">WiFi (MQTT) | TFT Display | Actuators (A4988 Stepper) | Local Logs</text>
  </g>

  <g transform="translate(40,650)">
    <rect class="box" width="440" height="80" style="fill:#eef;"/>
    <text class="title" x="220" y="25" text-anchor="middle">Main / Setup</text>
    <text class="sub" x="20" y="50">• main.ino (Setup Tasks/Queues)</text>
    <text class="sub" x="20" y="68">• PlatformIO Build Config</text>
  </g>

  <g transform="translate(500,650)">
    <rect class="box" width="440" height="230" style="fill:#fdfdfe;"/>
    <text class="title" x="20" y="25">Developer Notes</text>
    <text class="sub" x="20" y="55">• Use 'extern' for g_model_data to prevent duplicates.</text>
    <text class="sub" x="20" y="75">• Keep implementation in anonymous namespaces.</text>
    <text class="sub" x="20" y="95">• Ensure COM port &amp; Boot sequence for ESP32.</text>
    <text class="sub" x="20" y="115">• Check tensor_arena alignment for Xtensa.</text>
  </g>

  <path d="M 260 130 H 300" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  <path d="M 520 100 H 560" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  <path d="M 750 120 V 150" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  <path d="M 750 200 V 250" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  <path d="M 560 290 H 200 V 380" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  <text class="label" x="210" y="360">xFeatureQueue</text>
  
  <path d="M 360 455 H 400" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  <path d="M 720 415 H 680" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  
  <path d="M 200 530 V 560" stroke="#444" fill="none" marker-end="url(#arrow)"/>
  <text class="label" x="210" y="550">Processed Result</text>

</svg>

</html>
