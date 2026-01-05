# Motor Fault Detection - ML Training Pipeline

## 📋 Overview

This Python script trains a lightweight neural network to classify motor faults using sensor data from accelerometers, gyroscopes, temperature, current, and RPM sensors. The trained model is optimized for deployment on ESP32 microcontrollers using TensorFlow Lite with INT8 quantization.

## 🎯 Model Purpose

**Classification Task:** Detect 7 different motor conditions in real-time
- **Class 0:** Normal
- **Class 1:** Imbalance
- **Class 2:** Misalignment
- **Class 3:** Bearing Fault
- **Class 4:** Looseness
- **Class 5:** Overload
- **Class 6:** Overheat

## 📁 Project Structure

```
project/
├── fault.csv                              # Training dataset
├── motor_fault_training.py                # Main training script
├── motor_fault_model.h5                   # Trained Keras model (output)
├── motor_fault_model.tflite               # TFLite model (output)
├── motor_fault_model_quantized.tflite     # Quantized model for ESP32 (output)
├── scaler.pkl                             # Scaler parameters (output)
├── confusion_matrix.png                   # Evaluation visualization (output)
└── README.md                              # This file
```

## 🔧 Requirements

### Python Version
- Python 3.8 or higher

### Dependencies

Install required packages:

```bash
pip install numpy pandas tensorflow scikit-learn matplotlib seaborn
```

Or use requirements.txt:

```bash
pip install -r requirements.txt
```

**requirements.txt:**
```
numpy>=1.21.0
pandas>=1.3.0
tensorflow>=2.10.0
scikit-learn>=1.0.0
matplotlib>=3.4.0
seaborn>=0.11.0
```

## 📊 Dataset Format

The training data must be in CSV format with the following columns (in exact order):

| Column | Description | Unit |
|--------|-------------|------|
| ax | Accelerometer X-axis | g (gravity) |
| ay | Accelerometer Y-axis | g |
| az | Accelerometer Z-axis | g |
| gx | Gyroscope X-axis | deg/s |
| gy | Gyroscope Y-axis | deg/s |
| gz | Gyroscope Z-axis | deg/s |
| temp | Temperature | °C |
| current | Motor current | Amperes |
| rpm | Rotations per minute | RPM |
| label | Fault class (0-6) | Integer |

**Example CSV:**
```csv
ax,ay,az,gx,gy,gz,temp,current,rpm,label
0.05,0.02,0.98,0.12,-0.05,0.08,25.3,2.1,1500,0
0.15,0.08,0.95,1.23,0.45,0.67,28.5,2.8,1520,1
...
```

## 🚀 Usage

### Basic Usage

```bash
python motor_fault_training.py
```

### Step-by-Step Process

The script performs the following steps automatically:

1. **Load Dataset** - Reads `fault.csv`
2. **Preprocess Data** - Separates features and labels
3. **Normalize Features** - Applies StandardScaler (zero mean, unit variance)
4. **Split Data** - 80% training, 20% testing (stratified split)
5. **Build Model** - Creates lightweight neural network
6. **Train Model** - Trains with early stopping
7. **Evaluate** - Generates accuracy metrics and confusion matrix
8. **Convert to TFLite** - Creates TensorFlow Lite model
9. **Quantize** - Applies INT8 quantization for ESP32
10. **Test Inference** - Validates TFLite model on sample data

## 📈 Model Architecture

The neural network is designed for ESP32 constraints:

```
Input Layer (9 features)
    ↓
Dense Layer (32 neurons, ReLU)
    ↓
Dropout (30%)
    ↓
Dense Layer (16 neurons, ReLU)
    ↓
Dropout (20%)
    ↓
Output Layer (7 classes, Softmax)
```

**Model Specifications:**
- **Total Parameters:** ~1,000 parameters
- **Model Size (quantized):** < 10 KB
- **Input Shape:** (1, 9)
- **Output Shape:** (1, 7)
- **Quantization:** INT8 (8-bit integers)

## 📤 Output Files

After successful training, the script generates:

### 1. **motor_fault_model.h5**
- Full Keras model with float32 weights
- Use for: Further training, Python inference

### 2. **motor_fault_model.tflite**
- Standard TensorFlow Lite model
- Use for: Testing TFLite compatibility

### 3. **motor_fault_model_quantized.tflite** ⭐
- INT8 quantized model
- **Use this for ESP32 deployment**
- Size: ~3-5 KB

### 4. **scaler.pkl**
- StandardScaler parameters (mean and std)
- Contains normalization values for deployment

### 5. **confusion_matrix.png**
- Visual evaluation of model performance
- Shows per-class accuracy

## 🎯 Key Training Parameters

You can modify these in the script:

```python
# Data split
test_size = 0.2              # 20% for testing
random_state = 42            # Reproducibility

# Model architecture
hidden_layer_1 = 32          # First hidden layer neurons
hidden_layer_2 = 16          # Second hidden layer neurons
dropout_1 = 0.3              # Dropout rate layer 1
dropout_2 = 0.2              # Dropout rate layer 2

# Training
epochs = 100                 # Maximum training epochs
batch_size = 32              # Samples per batch
validation_split = 0.2       # 20% of training for validation
patience = 15                # Early stopping patience
```

## 📊 Performance Metrics

The script outputs:

### Training Metrics
- Training accuracy per epoch
- Validation accuracy per epoch
- Loss curves

### Test Metrics
- **Overall Accuracy:** Model accuracy on test set
- **Per-Class Precision:** Precision for each fault type
- **Per-Class Recall:** Recall for each fault type
- **F1-Scores:** Harmonic mean of precision and recall
- **Confusion Matrix:** Visual representation of predictions

### Example Output
```
Test Accuracy: 94.52%

Classification Report:
                 precision    recall  f1-score   support

       Normal       0.96      0.98      0.97       250
    Imbalance       0.93      0.91      0.92       230
 Misalignment       0.94      0.93      0.94       245
...
```

## 🔍 Scaler Parameters

The script prints scaler parameters needed for ESP32 deployment:

```python
Mean values: [0.059  0.046  0.042  0.277  0.262  0.281  43.42  2.169  1476.02]
Std values:  [0.128  0.128  0.129  1.331  1.314  1.335  6.597  0.434  58.008]
```

**⚠️ IMPORTANT:** Copy these exact values to your Arduino code's `scaler_params.h` file!

## 🐛 Troubleshooting

### Issue: Low Accuracy

**Possible Causes:**
- Insufficient training data
- Class imbalance in dataset
- Overlapping fault signatures

**Solutions:**
- Collect more samples per class
- Use data augmentation
- Adjust model architecture (increase neurons)
- Try different normalization techniques

### Issue: Model Too Large

**Solutions:**
- Reduce hidden layer sizes
- Remove one hidden layer
- Use more aggressive quantization
- Apply pruning techniques

### Issue: Overfitting

**Symptoms:**
- High training accuracy, low test accuracy
- Large gap between training and validation loss

**Solutions:**
- Increase dropout rates
- Add more training data
- Reduce model complexity
- Use L1/L2 regularization

### Issue: Import Errors

**Solution:**
```bash
# Ensure TensorFlow is properly installed
pip install --upgrade tensorflow

# For Apple Silicon Macs
pip install tensorflow-macos tensorflow-metal
```

## 🔄 Model Updating

To retrain with new data:

1. Add new samples to `fault.csv`
2. Ensure labels are correct (0-6)
3. Run the training script again
4. Copy new scaler values to Arduino code
5. Convert new `.tflite` file to `.h` file
6. Re-upload to ESP32

## 📝 Customization

### Changing Number of Classes

If you have different fault types:

```python
# Update class names
class_names = ['Class0', 'Class1', 'Class2', ...]

# Update output layer
keras.layers.Dense(NUM_CLASSES, activation='softmax')
```

### Adding More Features

If you have additional sensors:

```python
# Update feature columns
X = df[['ax', 'ay', 'az', ..., 'new_feature']].values

# Update input layer
keras.layers.Input(shape=(NUM_FEATURES,))
```

### Adjusting for Different Hardware

For more powerful devices:
- Increase hidden layer sizes (64, 128 neurons)
- Remove quantization step
- Use float32 instead of INT8

## 📚 References

- [TensorFlow Lite Documentation](https://www.tensorflow.org/lite)
- [Quantization Guide](https://www.tensorflow.org/lite/performance/post_training_quantization)
- [Scikit-learn StandardScaler](https://scikit-learn.org/stable/modules/generated/sklearn.preprocessing.StandardScaler.html)

## 📧 Support

For issues or questions:
1. Check the troubleshooting section
2. Verify dataset format
3. Ensure all dependencies are installed
4. Check TensorFlow compatibility with your system

## 📄 License

This project is for educational and research purposes.

---

**Next Step:** After training, proceed to the Arduino deployment README for ESP32 integration.