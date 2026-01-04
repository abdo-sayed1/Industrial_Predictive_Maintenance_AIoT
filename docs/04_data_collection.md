# 04 — Data Collection

This document explains how to collect sensor data for training and evaluation.

## Data format
- Time series CSV or a record-per-sample JSON. Include timestamps, sensor fields (gforce, temperature, current, encoder counts), and labels where available.

## Sampling & timing
- Typical sampling rate: 100 Hz for vibration is a good starting point; adjust as needed for your fault modes.
- Ensure timestamps are accurate and consistent. Use device time or add a synchronized host-side timestamp when capturing via MQTT.

## Methods
1. On-device logging: write CSV to SD card or flash then retrieve.
2. MQTT streaming: publish telemetry to `sensors/<device>/telemetry` and subscribe with a collector script to persist to disk.

### Quick MQTT capture example (Python)
```py
import paho.mqtt.client as mqtt
import csv

with open('capture.csv','w',newline='') as f:
	w = csv.writer(f)
	w.writerow(['ts','gforce','temperature','current','encoder','label'])

def on_message(client, userdata, msg):
	# parse payload and append row to file
	pass

# subscribe and start (fill in client callbacks and parsing)
```

## Labels & Ground Truth
- For supervised training, attach labels (fault type, normal) to segments of the recording or to each sample as appropriate.
- Use consistent naming and a small controlled set of fault classes.

## Quality & augmentation
- Record baseline (normal) data under different speeds/loads to ensure generalization.
- Augment by adding noise, resampling, or synthetic faults where applicable.
