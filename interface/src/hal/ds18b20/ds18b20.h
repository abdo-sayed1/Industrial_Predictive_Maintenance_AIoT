#ifndef DS18B20_SIMPLE_H
#define DS18B20_SIMPLE_H

#include <Arduino.h>

class DS18B20_Simple {
private:
    uint8_t _pin;
    portMUX_TYPE _mux = portMUX_INITIALIZER_UNLOCKED;
    unsigned long _lastConversionStart = 0;
    bool _isConverting = false;

    void writeBit(uint8_t bit) {
        portENTER_CRITICAL(&_mux);
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        if (bit) {
            delayMicroseconds(3);
            pinMode(_pin, INPUT);
            delayMicroseconds(60);
        } else {
            delayMicroseconds(60);
            pinMode(_pin, INPUT);
            delayMicroseconds(10);
        }
        portEXIT_CRITICAL(&_mux);
    }

    uint8_t readBit() {
        uint8_t r;
        portENTER_CRITICAL(&_mux);
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        delayMicroseconds(3);
        pinMode(_pin, INPUT);
        delayMicroseconds(10);
        r = digitalRead(_pin);
        delayMicroseconds(50);
        portEXIT_CRITICAL(&_mux);
        return r;
    }

    void writeByte(uint8_t data) {
        for (uint8_t i = 0; i < 8; i++) {
            writeBit(data & 0x01);
            data >>= 1;
        }
    }

    uint8_t readByte() {
        uint8_t data = 0;
        for (uint8_t i = 0; i < 8; i++) {
            if (readBit()) data |= (1 << i);
        }
        return data;
    }

public:
    DS18B20_Simple(uint8_t pin) : _pin(pin) {}

    bool reset() {
        bool presence;
        portENTER_CRITICAL(&_mux);
        pinMode(_pin, OUTPUT);
        digitalWrite(_pin, LOW);
        delayMicroseconds(480);
        pinMode(_pin, INPUT);
        delayMicroseconds(70);
        presence = (digitalRead(_pin) == 0);
        delayMicroseconds(410);
        portEXIT_CRITICAL(&_mux);
        return presence;
    }

    // Starts the temperature conversion without blocking
    void requestTemperature() {
        if (!reset()) return;
        writeByte(0xCC); // Skip ROM
        writeByte(0x44); // Start Conversion
        _lastConversionStart = millis();
        _isConverting = true;
    }

    // Checks if enough time has passed (750ms) and returns the value
    float getTemperature() {
        if (!_isConverting) return -888.0; // Not requested
        if (millis() - _lastConversionStart < 750) return -777.0; // Still busy

        if (!reset()) return -999.0;
        writeByte(0xCC);
        writeByte(0xBE); // Read Scratchpad

        uint8_t lowByte = readByte();
        uint8_t highByte = readByte();
        _isConverting = false;

        int16_t raw = (highByte << 8) | lowByte;
        return (float)raw / 16.0;
    }
};

#endif