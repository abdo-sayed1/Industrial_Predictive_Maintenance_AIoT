# 01 — Hardware Setup

Purpose: describe the physical components, wiring, and basic calibration required to build the Industrial Predictive Maintenance (IPM) device used in this project.

## Bill of Materials (BOM)
- ESP32 development board (ESP32-WROOM or similar)
- MPU6050 (accelerometer + gyroscope)
- DS18B20 temperature sensor
- MAX471 (current/voltage sensor) or equivalent
- Quadrature encoder (for speed/position)
- Stepper motor + A4988 driver (if using actuators)
- Power supply (5V / appropriate current rating)
- Level shifters, connector wires, protoboard or PCB

## Wiring overview
- MPU6050: connect SDA/SCL to I2C pins (commonly GPIO21=SDA, GPIO22=SCL), power to 3.3V and GND.
- DS18B20: single-wire to a GPIO with a 4.7k pull-up to 3.3V, power to 3.3V and GND.
- MAX471: measure V+ and V-, output analog to ADC pins (use a stable Vref).
- Encoder: connect A/B to interrupt-capable GPIOs and VCC/GND as required.
- A4988: VMOT to motor supply, GND common with ESP32, STEP/DIR/ENABLE to GPIOs; set microstep jumpers and current limit carefully.

## Power & safety
- Use a regulated power supply; separate high-current motor supply from logic when possible.
- Double-check motor wiring and A4988 current limit before running motors.

## Layout & assembly tips
- Keep sensor wiring short and twisted where practical (reduce noise for analog/motion sensors).
- Place decoupling capacitors near sensors and driver power pins.

## Calibration
- MPU6050: run a static calibration to remove offset and scale (use library helper functions or record baseline while device is static).
- ADC: measure a known reference to validate readings and map voltage to expected ranges.

## Testing the hardware
1. Power the board and confirm the serial console at 115200 bps outputs boot messages.
2. Use included test sketches/drivers in `interface/src/hal/` to validate each sensor (MPU, DS18B20, MAX471, encoder).

## Notes
- If you replace sensors, update wiring and corresponding driver code under `interface/src/hal/`.
