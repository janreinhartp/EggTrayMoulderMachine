/*
 * Scale Controller Implementation
 * Manages NAU7802 24-bit ADC for load cell weight measurements
 */

#include "ScaleController.h"
#include <Preferences.h>
#include <LogController.h>

extern Preferences preferences;
extern LogController logger;

ScaleController::ScaleController() {
    zeroOffset = 0;
    calibrationFactor = 1.0;
    calibrated = false;
    connected = false;
}

bool ScaleController::init() {
    logger.info("Scale", "Initializing NAU7802...");
    
    // Initialize NAU7802
    if (scale.begin() == false) {
        logger.error("Scale", "NAU7802 not detected!");
        connected = false;
        return false;
    }
    
    connected = true;
    logger.info("Scale", "NAU7802 detected");
    
    // Configure scale
    scale.setSampleRate(NAU7802_SPS_80);  // 80 samples per second
    scale.setGain(NAU7802_GAIN_128);      // Gain of 128
    scale.calibrateAFE();                 // Calibrate analog front end
    
    // Load saved calibration
    loadCalibration();
    
    logger.info("Scale", calibrated ? "Calibrated" : "Not calibrated");
    
    return true;
}

void ScaleController::startCalibration() {
    logger.info("Scale", "Starting calibration sequence");
    calibrated = false;
}

bool ScaleController::calibrateZero() {
    if (!connected) {
        logger.error("Scale", "Not connected!");
        return false;
    }
    
    logger.info("Scale", "Calibrating zero...");
    
    // Wait for scale to be ready
    delay(500);
    
    // Take average of multiple readings
    long sum = 0;
    const int samples = 10;
    
    for (int i = 0; i < samples; i++) {
        while (!scale.available()) {
            delay(1);
        }
        sum += scale.getReading();
        delay(50);
    }
    
    zeroOffset = sum / (float)samples;
    
    logger.info("Scale", "Zero calibration complete");
    
    return true;
}

bool ScaleController::calibrateKnownWeight(float knownWeight) {
    if (!connected) {
        logger.error("Scale", "Not connected!");
        return false;
    }
    
    if (knownWeight <= 0) {
        logger.error("Scale", "Invalid known weight!");
        return false;
    }
    
    logger.info("Scale", "Starting weight calibration...");
    
    // Wait for scale to be ready
    delay(500);
    
    // Take average of multiple readings
    long sum = 0;
    const int samples = 10;
    
    for (int i = 0; i < samples; i++) {
        while (!scale.available()) {
            delay(1);
        }
        sum += scale.getReading();
        delay(50);
    }
    
    float rawAverage = sum / (float)samples;
    float rawDifference = rawAverage - zeroOffset;
    
    if (rawDifference == 0) {
        logger.error("Scale", "No weight detected!");
        return false;
    }
    
    calibrationFactor = rawDifference / knownWeight;
    calibrated = true;
    
    logger.info("Scale", "Calibration complete");
    
    return true;
}

void ScaleController::saveCalibration() {
    if (!connected) return;
    
    logger.info("Scale", "Saving calibration...");
    
    preferences.putFloat("scaleZero", zeroOffset);
    preferences.putFloat("scaleFactor", calibrationFactor);
    preferences.putBool("scaleCal", calibrated);
    
    logger.info("Scale", "Calibration saved");
}

void ScaleController::loadCalibration() {
    logger.debug("Scale", "Loading calibration...");
    
    zeroOffset = preferences.getFloat("scaleZero", 0.0);
    calibrationFactor = preferences.getFloat("scaleFactor", 1.0);
    calibrated = preferences.getBool("scaleCal", false);
    
    // Validate loaded values
    if (isnan(zeroOffset) || isnan(calibrationFactor) || 
        calibrationFactor == 0 || abs(calibrationFactor) > 1000000) {
        logger.warning("Scale", "Invalid calibration data, using defaults");
        zeroOffset = 0;
        calibrationFactor = 1.0;
        calibrated = false;
    }
}

float ScaleController::getWeight() {
    if (!connected || !calibrated) {
        return 0.0;
    }
    
    if (!scale.available()) {
        return 0.0; // No new data
    }
    
    long rawValue = scale.getReading();
    float weight = (rawValue - zeroOffset) / calibrationFactor;
    
    // Return 0 for negative weights
    return (weight < 0) ? 0.0 : weight;
}

long ScaleController::getRawReading() {
    if (!connected) {
        return 0;
    }
    
    if (!scale.available()) {
        return 0;
    }
    
    return scale.getReading();
}

bool ScaleController::isReady() {
    if (!connected) {
        return false;
    }
    
    return scale.available();
}
