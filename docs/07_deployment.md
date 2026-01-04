# 07 — Deployment

This document describes how to build and deploy firmware and cloud stack to test devices.

## Firmware build & upload
- Build in `interface/`:
	- `pio run` (build)
	- `pio run -t upload` (upload to device over serial/USB)
- Use `pio device monitor --port COMx --baud 115200` to watch boot messages and logs.

## Updating the model on device
- Replace `interface/src/app/inference/model_data.h` with the new exported model bytes and increment model version in code/comments.
- Rebuild and upload firmware.

## Cloud deployment (local)
- Start the compose stack: `docker compose -f cloud/docker-compose.yml up -d`.
- Update Grafana dashboards under `cloud/grafana/provisioning` if adding metrics.

## Versioning & releases
- Tag firmware and model versions in Git. Include training config and data snapshot for each model release.

## Monitoring & debug
- Use MQTT topics and InfluxDB metrics to monitor health and behavior. Configure alerts in Grafana if necessary.
