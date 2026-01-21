/*
 * Scale Controller
 * Manages NAU7802 24-bit ADC for load cell weight measurements
 */

#ifndef SCALECONTROLLER_H
#define SCALECONTROLLER_H

#include <Arduino.h>
#include <SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h>

class ScaleController {
public:
    ScaleController();
    
    // Initialization
    bool init();
    
    // Calibration methods
    void startCalibration();
    bool calibrateZero();                    // Tare/zero calibration
    bool calibrateKnownWeight(float weight); // Calibrate with known weight
    void saveCalibration();
    void loadCalibration();
    
    // Weight reading
    float getWeight();                       // Get weight in grams
    long getRawReading();                    // Get raw ADC value
    bool isReady();                          // Check if new reading available
    
    // Calibration data access
    float getZeroOffset() { return zeroOffset; }
    float getCalibrationFactor() { return calibrationFactor; }
    void setZeroOffset(float offset) { zeroOffset = offset; }
    void setCalibrationFactor(float factor) { calibrationFactor = factor; }
    
    // Status
    bool isCalibrated() { return calibrated; }
    bool isConnected() { return connected; }

private:
    NAU7802 scale;
    
    float zeroOffset;
    float calibrationFactor;
    bool calibrated;
    bool connected;
};

#endif // SCALECONTROLLER_H
