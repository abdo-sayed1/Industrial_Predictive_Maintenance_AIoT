# 03 — Docker & Cloud Stack

This explains how to run the local cloud stack (MQTT, InfluxDB, Telegraf, Grafana) for testing and visualization.

## Start stack
From the repository root:

```bash
docker compose -f cloud/docker-compose.yml up -d
# To bring it down: docker compose -f cloud/docker-compose.yml down
```

## Services & ports
- Mosquitto (MQTT) — default 1883
- InfluxDB — default 8086
- Grafana — default 3000 (pre-provisioned dashboards under `cloud/grafana/provisioning`)
- Telegraf — configured with `cloud/telegraf/telegraf.conf`

## Common tasks
- View logs: `docker compose -f cloud/docker-compose.yml logs -f`
- Reset InfluxDB data (use with caution): stop stack, remove data volume containers, start again.

## Notes
- Edit any environment variables in compose file or use an `.env` for secrets and custom ports.
