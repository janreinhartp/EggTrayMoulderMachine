/*
 * Settings Configuration
 * All configurable parameters for the Egg Tray Moulder Machine
 */

#ifndef SETTINGSCONFIG_H
#define SETTINGSCONFIG_H

#include <Arduino.h>

// ==================== WATER SETTINGS ====================
extern int waterAmount;          // ml
extern int waterFlowTimeout;     // seconds

// ==================== STARCH SETTINGS ====================
extern int starchWeight;         // grams
extern int starchDispenseTime;   // seconds

// ==================== PAPER SHREDDER ====================
extern int shredderTime;         // seconds

// ==================== MIXER SETTINGS ====================
extern int mixTime;              // seconds
extern int mixSpeed;             // percentage

// ==================== PULP PUMP ====================
extern int pumpTime;             // seconds

// ==================== MOULDING TIMINGS ====================
extern int mouldSuctionTime;     // seconds
extern int mouldBlowerTime;      // seconds
extern int mouldCycleDelay;      // seconds

// ==================== DRYING SETTINGS ====================
extern int dryingTime;           // seconds
extern int dryingTemp;           // degrees C

// ==================== CONVEYOR ====================
extern int conveyorSpeed;        // percentage

// ==================== SCALE CALIBRATION ====================
extern int scaleCalibrationWeight; // grams - known weight for calibration

// ==================== SYSTEM STATE ====================
extern bool testMode;
extern bool systemRunning;
extern String systemStatus;

#endif // SETTINGSCONFIG_H
