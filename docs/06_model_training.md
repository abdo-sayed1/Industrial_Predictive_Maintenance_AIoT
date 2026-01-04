# 06 — Model Training & Export

This document covers training, evaluation, and exporting models to TensorFlow Lite for embedding in the firmware.

## Data preparation
- Consolidate labeled data into training and validation sets. Normalize features and window time-series into fixed-size inputs (e.g., 256 samples per window).
- Use consistent file formats and generate metadata (sampling rate, sensor order) so firmware mapping stays correct.

## Training
- Scripts: `AI/scripts/train.py` — adjust hyperparameters and model architecture here.
- Typical steps:
	1. Load and preprocess dataset
	2. Train model with validation split
	3. Evaluate and save best checkpoint

## Evaluation
- Track metrics like accuracy, precision/recall per class, and confusion matrix.
- Consider cross-validation or holdout tests across different operating conditions.

## Export to TFLite
- Use `AI/scripts/export.py` to convert checkpoint -> TFLite. Prefer quantized models (float16 or int8) for microcontrollers.
- After export, run a sanity check: use the TFLite interpreter to run inference on sample inputs and compare outputs to the original model.

## Embed in firmware
- The exported TFLite file can be converted to a C array (`xxd -i`) or use `export.py` helper to generate `model_data.h` / `model_data.cpp`.
- Place the final model bytes in `interface/src/app/inference/model_data.h` and ensure one `.cpp` defines the array to avoid multiple definition errors.

## Notes & best practices
- Keep a clear versioning scheme for model files and record the training configuration used to generate each model.
- Add unit tests that confirm input normalization on-device matches preprocessing used during training.
