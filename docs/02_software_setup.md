# 02 — Software Setup

This document explains how to set up the development environment for firmware and model work.

## Prerequisites
- OS: Windows (tested), macOS, Linux should work similarly
- Python 3.10+ (for AI scripts)
- PlatformIO extension (VS Code) or CLI `platformio`
- Docker (for cloud stack)

## Python & AI dependencies
1. Create and activate a virtual environment:
	- python -m venv .venv
	- .\.venv\Scripts\activate    (Windows)
2. Install requirements for AI tools:
	- pip install -r AI/requirements.txt

## PlatformIO (firmware)
- Install the PlatformIO extension for VS Code or use the CLI.
- Open the `interface/` folder in VS Code to load PlatformIO project and toolchain.
- Common commands:
  - Build: `pio run`
  - Upload: `pio run -t upload`
  - Serial monitor: `pio device monitor --port COMx --baud 115200`

## Docker / Compose
- Ensure Docker Desktop is installed and running for the cloud stack.
- See `docs/03_docker_setup.md` for details running the services.

## Useful tips
- Keep Python environment isolated for reproducibility.
- If adding new Python dependencies, add them to `AI/requirements.txt`.
