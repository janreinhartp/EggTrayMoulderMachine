#include "SettingsConfig.h"

// ==================== WATER SETTINGS ====================
int waterAmount = 1000;          // ml
int waterFlowTimeout = 60;       // seconds

// ==================== STARCH SETTINGS ====================
int starchWeight = 500;          // grams
int starchDispenseTime = 10;     // seconds

// ==================== PAPER SHREDDER ====================
int shredderTime = 30;           // seconds

// ==================== MIXER SETTINGS ====================
int mixTime = 120;               // seconds
int mixSpeed = 100;              // percentage

// ==================== PULP PUMP ====================
int pumpTime = 45;               // seconds

// ==================== MOULDING TIMINGS ====================
int mouldSuctionTime = 8;        // seconds
int mouldBlowerTime = 5;         // seconds
int mouldCycleDelay = 2;         // seconds

// ==================== DRYING SETTINGS ====================
int dryingTime = 300;            // seconds
int dryingTemp = 80;             // degrees C

// ==================== CONVEYOR ====================
int conveyorSpeed = 50;          // percentage

// ==================== SCALE CALIBRATION ====================
int scaleCalibrationWeight = 500; // grams - known weight for calibration

// ==================== SYSTEM STATE ====================
bool testMode = false;
bool systemRunning = false;
String systemStatus = "Idle";
