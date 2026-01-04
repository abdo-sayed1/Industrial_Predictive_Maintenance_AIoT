# 05 — Fault Injection

Purpose: describe how to create labeled fault data for training and validation.

## Types of fault injection
- Physical: apply mechanical faults such as misalignment, added imbalance mass, loose bearings, or friction.
- Electrical: vary supply voltage, add current spikes, or short phases (careful and use protection).
- Software: toggle states or inject abnormal sensor values in firmware to simulate failures (useful for repeatable tests).

## Procedure
1. Define fault scenarios and parameters (e.g., imbalance magnitude, temperature increase, reduced voltage).
2. For each scenario, run a controlled experiment and record data using the collection method (see `docs/04_data_collection.md`).
3. Label the data with the scenario id and parameters.

## Safety
- Prioritize safety when doing physical or electrical faults: perform tests with protective enclosures and emergency stop switches.

## Automating injections
- Add a mode in firmware that enables controlled parameter changes (e.g., enable a test profile via serial or MQTT).
- Record the timestamps of injection events to align ground truth with sensor data.

## Notes
- Prefer many small, repeatable injections over single large destructive tests — this creates more usable training samples.
