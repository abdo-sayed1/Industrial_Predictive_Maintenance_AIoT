[Presentation](presentation/index.html) — static HTML slides.

<!--
SVG Flowchart of project architecture
Save as project_flowchart.svg
-->
<svg xmlns="http://www.w3.org/2000/svg" width="1200" height="820" viewBox="0 0 1200 820" font-family="Arial, Helvetica, sans-serif">
  <defs>
    <marker id="arrow" viewBox="0 0 10 10" refX="10" refY="5" markerWidth="6" markerHeight="6" orient="auto-start-reverse">
      <path d="M 0 0 L 10 5 L 0 10 z" fill="#333"/>
    </marker>
    <style>
      .box { fill:#f7f7f7; stroke:#333; stroke-width:1; rx:6; ry:6; }
      .title { font-weight:700; font-size:14px; fill:#111; }
      .label { font-size:12px; fill:#111; }
      .sub { font-size:11px; fill:#333; }
    </style>
  </defs>

  <!-- Sensors -->
  <g transform="translate(20,20)">
    <rect class="box" x="0" y="0" width="230" height="180"/>
    <text class="title" x="115" y="22" text-anchor="middle">Sensors</text>
    <text class="sub" x="20" y="48">- MPU6050 (Vibration/Accel/Gyro)</text>
    <text class="sub" x="20" y="68">- DS18B20 (Temperature)</text>
    <text class="sub" x="20" y="88">- MAX471 (Current/Voltage)</text>
    <text class="sub" x="20" y="108">- Encoder (Speed/Position)</text>
    <text class="sub" x="20" y="128">- ADC (Extra analog sensors)</text>
  </g>

  <!-- HAL Drivers -->
  <g transform="translate(280,20)">
    <rect class="box" x="0" y="0" width="220" height="120"/>
    <text class="title" x="110" y="22" text-anchor="middle">HAL / Drivers</text>
    <text class="sub" x="12" y="48">- mpu6050.cpp</text>
    <text class="sub" x="12" y="68">- ds18b20.cpp</text>
    <text class="sub" x="12" y="88">- max471.cpp, adc.cpp, encoder.cpp</text>
  </g>

  <!-- Data Acquisition Task -->
  <g transform="translate(540,10)">
    <rect class="box" x="0" y="0" width="240" height="70"/>
    <text class="title" x="120" y="24" text-anchor="middle">Data Acquisition</text>
    <text class="sub" x="120" y="44" text-anchor="middle">reads HAL -> pushes raw to queues</text>
  </g>

  <!-- Raw Queues -->
  <g transform="translate(540,100)">
    <rect class="box" x="0" y="0" width="240" height="60"/>
    <text class="title" x="120" y="22" text-anchor="middle">Raw Data Queues</text>
    <text class="sub" x="120" y="42" text-anchor="middle">xRawFeatureDataQueue / xRawInferenceDataQueue</text>
  </g>

  <!-- Features Task -->
  <g transform="translate(280,200)">
    <rect class="box" x="0" y="0" width="240" height="100"/>
    <text class="title" x="120" y="22" text-anchor="middle">Features Task (vFeaturesTask)</text>
    <text class="sub" x="12" y="48">- normalize(), feature engineering</text>
    <text class="sub" x="12" y="68">- outputs: xFeatureQueue</text>
  </g>

  <!-- Inference Task -->
  <g transform="translate(540,220)">
    <rect class="box" x="0" y="0" width="300" height="140"/>
    <text class="title" x="150" y="22" text-anchor="middle">Inference Task</text>
    <text class="sub" x="12" y="48">- pulls features from xFeatureQueue</text>
    <text class="sub" x="12" y="64">- maps features to TFLite input tensor</text>
    <text class="sub" x="12" y="80">- interpreter->Invoke()</text>
    <text class="sub" x="12" y="96">- pushes results to xProcessedInferenceQueue</text>
  </g>

  <!-- TFLite block -->
  <g transform="translate(880,220)">
    <rect class="box" x="0" y="0" width="260" height="170"/>
    <text class="title" x="130" y="22" text-anchor="middle">TensorFlow Lite (micro)</text>
    <text class="sub" x="12" y="48">- setupTFLite()</text>
    <text class="sub" x="12" y="64">- MicroErrorReporter, AllOpsResolver</text>
    <text class="sub" x="12" y="80">- Model: g_model_data (model_data.cpp)</text>
    <text class="sub" x="12" y="96">- MicroInterpreter -> input/output tensors</text>
    <text class="sub" x="12" y="112">- tensor_arena[] (kTensorArenaSize)</text>
    <rect x="14" y="124" width="232" height="30" rx="4" ry="4" fill="#fff3cd" stroke="#c69000"/>
    <text class="sub" x="130" y="142" text-anchor="middle">Outputs: fault prob, fault type</text>
  </g>

  <!-- Outputs -->
  <g transform="translate(540,380)">
    <rect class="box" x="0" y="0" width="600" height="120"/>
    <text class="title" x="300" y="24" text-anchor="middle">Outputs / Actions</text>
    <text class="sub" x="20" y="50">- WiFi / MQTT (publish predictions & telemetry)</text>
    <text class="sub" x="20" y="70">- Display (TFT / screen drivers via TensorFlowLite_ESP32 lib)</text>
    <text class="sub" x="20" y="90">- Actuator control (Stepper A4988, alarms, logs)</text>
  </g>

  <!-- Other modules -->
  <g transform="translate(20,220)">
    <rect class="box" x="0" y="0" width="240" height="120"/>
    <text class="title" x="120" y="22" text-anchor="middle">Main / System</text>
    <text class="sub" x="12" y="48">- main.ino: task creation, setup()</text>
    <text class="sub" x="12" y="68">- init WiFi, MQTT, tasks</text>
    <text class="sub" x="12" y="88">- provides OS primitives (FreeRTOS)</text>
  </g>

  <!-- Build & Toolchain -->
  <g transform="translate(20,370)">
    <rect class="box" x="0" y="0" width="240" height="120"/>
    <text class="title" x="120" y="22" text-anchor="middle">Build & Upload</text>
    <text class="sub" x="12" y="48">- PlatformIO / platformio.ini</text>
    <text class="sub" x="12" y="68">- toolchain xtensa-esp32, esptool</text>
    <text class="sub" x="12" y="88">- upload to ESP32 (COM / BOOT sequence)</text>
  </g>

  <!-- Data flow arrows -->
  <!-- Sensors -> HAL -->
  <line x1="250" y1="100" x2="280" y2="100" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>

  <!-- HAL -> Data Acquisition -->
  <line x1="500" y1="50" x2="540" y2="40" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <line x1="500" y1="80" x2="540" y2="60" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>

  <!-- Data Acquisition -> Raw Queues -->
  <line x1="660" y1="45" x2="660" y2="100" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>

  <!-- Raw Queues -> Features Task -->
  <line x1="540" y1="160" x2="420" y2="200" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <text class="label" x="470" y="175" text-anchor="middle">xQueueReceive()</text>

  <!-- Features -> Feature Queue -->
  <line x1="360" y1="300" x2="540" y2="240" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <text class="label" x="450" y="270" text-anchor="middle">xQueueSend(xFeatureQueue)</text>

  <!-- Feature Queue -> Inference Task -->
  <line x1="520" y1="300" x2="540" y2="300" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <text class="label" x="400" y="310">(processed features)</text>

  <!-- Inference -> TFLite -->
  <line x1="840" y1="260" x2="880" y2="260" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <text class="label" x="720" y="250">map inputs -> interpreter</text>

  <!-- TFLite -> Inference Results -->
  <line x1="1140" y1="300" x2="840" y2="420" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <text class="label" x="990" y="360">prediction</text>

  <!-- Inference -> Outputs -->
  <line x1="640" y1="360" x2="540" y2="420" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <text class="label" x="630" y="390">xQueueSend(xProcessedInferenceQueue)</text>

  <!-- Outputs -> MQTT / Display / Actuators -->
  <line x1="780" y1="450" x2="960" y2="450" stroke="#333" stroke-width="2" marker-end="url(#arrow)"/>
  <text class="label" x="870" y="430" text-anchor="middle">publish / update</text>

  <!-- Model data relationship -->
  <rect x="880" y="410" width="260" height="60" rx="6" ry="6" fill="#eef" stroke="#333"/>
  <text class="sub" x="1010" y="432" text-anchor="middle">model_data.cpp / model_data.h (g_model_data)</text>
  <line x1="1010" y1="470" x2="1010" y2="392" stroke="#333" stroke-width="1" marker-end="url(#arrow)"/>

  <!-- Notes (warnings/debug) -->
  <g transform="translate(20,510)">
    <rect class="box" x="0" y="0" width="1120" height="260"/>
    <text class="title" x="560" y="22" text-anchor="middle">Developer Notes</text>
    <text class="sub" x="20" y="52">- Keep anonymous namespaces in .cpp only (implementation details)</text>
    <text class="sub" x="20" y="72">- model_data: define bytes in a single .cpp + extern in header to avoid multiple definitions</text>
    <text class="sub" x="20" y="92">- Use unique/clear variable names for tensors (avoid generic 'input')</text>
    <text class="sub" x="20" y="112">- Troubleshooting upload: check COM port, cable, boot/bootloader sequence</text>
    <text class="sub" x="20" y="132">- TensorFlowLite_ESP32 provides screen drivers (watch for implicit function declarations if interfaces missing)</text>
    <text class="sub" x="20" y="162">- Main runtime: FreeRTOS tasks + queues coordinate producers (acquisition/features) and consumer (inference)</text>
    <text class="sub" x="20" y="192">- Build managed by PlatformIO (platformio.ini) -> toolchain-xtensa-esp32</text>
  </g>
</svg>