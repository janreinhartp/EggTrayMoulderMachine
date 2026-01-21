#ifndef SIMPLE_SERVO_H
#define SIMPLE_SERVO_H

#include <Arduino.h>

class SimpleServo {
public:
    SimpleServo() : _pin(-1), _resolutionBits(12), _minUs(500), _maxUs(2500), _freq(50) {}

    bool attach(int pin) {
        _pin = pin;
        // Set resolution per pin (Arduino-ESP32 v3 API requires pin)
        analogWriteResolution(_pin, _resolutionBits);
        // Set 50 Hz for servo
        analogWriteFrequency(_pin, _freq);
        // Start at minimum (closed)
        write(0);
        return true;
    }

    void write(int angle) {
        if (angle < 0) angle = 0;
        if (angle > 270) angle = 270;
        // Map angle to pulse width between _minUs and _maxUs
        int us = map(angle, 0, 270, _minUs, _maxUs);
        writeMicroseconds(us);
    }

    void writeMicroseconds(int us) {
        if (_pin < 0) return;
        // At 50 Hz, period is 20,000 us. Duty ratio = us / 20000
        const int periodUs = 20000;
        float duty = (float)us / (float)periodUs; // 0.0 .. 1.0
        uint32_t maxDuty = (1u << _resolutionBits) - 1u;
        uint32_t dutyCount = (uint32_t)(duty * maxDuty);
        analogWrite(_pin, dutyCount);
    }

    void detach() {
        if (_pin < 0) return;
        analogWrite(_pin, 0);
        _pin = -1;
    }

private:
    int _pin;
    uint8_t _resolutionBits;
    int _minUs;
    int _maxUs;
    int _freq;
};

#endif // SIMPLE_SERVO_H
