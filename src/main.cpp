/*
 * Egg Tray Moulder Machine Controller
 * ESP32-C3 Based System
 * 
 * Hardware:
 * - LCD I2C Display (20x4)
 * - 2x PCF8575 I2C Expanders (16-bit)
 * - 1x ADS1115 ADC
 * - 1x 16-channel Relay Board (via PCF8575 #1)
 * - 1x 8-channel Relay Board (via PCF8575 #2)
 * - 3x Buttons: Enter, Up, Down (via PCF8575 #2)
 * - Water Flow Sensor (via PCF8575 #2)
 * - Servo Motor (Starch Discharge)
 * - Load Cell (HX711 amplifier)
 * 
 * Features:
 * - Non-blocking operation
 * - Settings saved to Preferences
 * - Multi-layer menu system
 * - Fast/slow scroll support
 * - Auto run sequence
 * - Manual test mode
 */

#include <Arduino.h>
#include <Wire.h>
#include "SimpleServo.h"

// Configuration includes
#include "HardwareConfig.h"
#include "PCF8575_PinMap.h"
#include "SettingsConfig.h"
#include "MenuConfig.h"

// Controller includes
#include <ButtonController.h>
#include <MenuController.h>
#include <DisplayController.h>
#include <LogController.h>
#include <ScaleController.h>

// ==================== GLOBAL OBJECTS ====================

ButtonController buttonController;
MenuController menuController;
DisplayController displayController;
ScaleController scaleController;
SimpleServo starchServo;

// ==================== RELAY STATE TRACKING ====================

// Relay states (true = ON/closed, false = OFF/open)
bool relayStates[24] = {false}; // 16 relays on PCF8575_1 + 8 relays on PCF8575_2

// Servo state
int servoAngle = 0;  // Current servo angle (0-180)

// Relay names for display
const char* relayNames[24] = {
    "Spare 1",              // 0  - PCF8575_1 P0
    "Spare 2",              // 1  - PCF8575_1 P1
    "Linear Door",          // 2  - PCF8575_1 P2
    "Spare 3",              // 3  - PCF8575_1 P3
    "Defective",            // 4  - PCF8575_1 P4
    "Conveyor",             // 5  - PCF8575_1 P5
    "Screw",                // 6  - PCF8575_1 P6
    "Defective",            // 7  - PCF8575_1 P7
    "Shredder Power",       // 8  - PCF8575_1 P8 (ON/OFF)
    "Pump",                 // 9  - PCF8575_1 P9
    "Mixer",                // 10 - PCF8575_1 P10
    "Shredder Main Power",  // 11 - PCF8575_1 P11
    "Valve",                // 12 - PCF8575_1 P12
    "Spare 4",              // 13 - PCF8575_1 P13
    "Heater",               // 14 - PCF8575_1 P14
    "Spare 5",              // 15 - PCF8575_1 P15
    "Vacuum",               // 16 - PCF8575_2 P0
    "Blower",               // 17 - PCF8575_2 P1
    "Mould A V/B",          // 18 - PCF8575_2 P2
    "Vacuum A/B",           // 19 - PCF8575_2 P3
    "Blower A/B",           // 20 - PCF8575_2 P4
    "Mould B V/B",          // 21 - PCF8575_2 P5
    "Fwd/Rev",              // 22 - PCF8575_2 P6
    "Up/Down"               // 23 - PCF8575_2 P7
};

// ==================== FORWARD DECLARATIONS ====================

// Display callback
void updateDisplay(const char* line1, const char* line2, bool editing);

// Process functions (to be implemented in future steps)
void processAutoRun();

// Relay control functions
void setRelay(uint8_t relayIndex, bool state);
void toggleRelay(uint8_t relayIndex);

// ==================== RELAY CONTROL FUNCTIONS ====================

void setRelay(uint8_t relayIndex, bool state) {
    if (relayIndex >= 24) return;
    
    relayStates[relayIndex] = state;
    
    // Relays are active LOW (LOW = ON, HIGH = OFF)
    uint8_t pinState = state ? LOW : HIGH;
    
    if (relayIndex < 16) {
        // PCF8575_1 (relays 0-15)
        pcf8575_1.digitalWrite(relayIndex, pinState);
    } else {
        // PCF8575_2 (relays 16-23 map to pins 0-7)
        pcf8575_2.digitalWrite(relayIndex - 16, pinState);
    }
}

void toggleRelay(uint8_t relayIndex) {
    if (relayIndex >= 24) return;
    setRelay(relayIndex, !relayStates[relayIndex]);
    
    // Force menu refresh to show updated state
    menuController.refresh();
}

// Individual relay toggle functions for menu actions
void toggleRelay0() { toggleRelay(0); }
void toggleRelay1() { toggleRelay(1); }
void toggleRelay2() { toggleRelay(2); }
void toggleRelay3() { toggleRelay(3); }
void toggleRelay4() { toggleRelay(4); }
void toggleRelay5() { toggleRelay(5); }
void toggleRelay6() { toggleRelay(6); }
void toggleRelay7() { toggleRelay(7); }
void toggleRelay8() { toggleRelay(8); }
void toggleRelay9() { toggleRelay(9); }
void toggleRelay10() { toggleRelay(10); }
void toggleRelay11() { toggleRelay(11); }
void toggleRelay12() { toggleRelay(12); }
void toggleRelay13() { toggleRelay(13); }
void toggleRelay14() { toggleRelay(14); }
void toggleRelay15() { toggleRelay(15); }
void toggleRelay16() { toggleRelay(16); }
void toggleRelay17() { toggleRelay(17); }
void toggleRelay18() { toggleRelay(18); }
void toggleRelay19() { toggleRelay(19); }
void toggleRelay20() { toggleRelay(20); }
void toggleRelay21() { toggleRelay(21); }
void toggleRelay22() { toggleRelay(22); }
void toggleRelay23() { toggleRelay(23); }

// Servo control functions
void setServoAngle(int angle) {
    if (angle < 0) angle = 0;
    if (angle > 270) angle = 270;
    servoAngle = angle;
    starchServo.write(servoAngle);
    logger.info("Servo", "Angle set to", servoAngle);
}

void toggleServo() {
    if (servoAngle == 240) {
        setServoAngle(0);   // Close position
        displayController.showStatus("Servo: Close", 1000);
    } else {
        setServoAngle(240); // Open position
        displayController.showStatus("Servo: Open", 1000);
    }
    menuController.refresh();
}

// ==================== MENU ACTION FUNCTIONS ====================

void startAutoRun() {
    logger.info("AutoRun", "Starting Auto Run...");
    systemRunning = true;
    systemStatus = "Running";
    displayController.showStatus("Starting Auto Run", 2000);
}

void stopAutoRun() {
    logger.info("AutoRun", "Stopping Auto Run...");
    systemRunning = false;
    systemStatus = "Stopped";
    displayController.showStatus("Stopped", 1000);
    menuController.reset();
}

void exitTestMode() {
    logger.info("Test", "Exiting Test Mode");
    
    // Turn off all relays when exiting test mode
    for (uint8_t i = 0; i < 24; i++) {
        if (relayStates[i]) {
            setRelay(i, false);
        }
    }
    
    displayController.showStatus("All Relays OFF", 1000);
    menuController.goBack();
}

void saveSettings() {
    logger.info("Settings", "Saving all settings...");
    menuController.saveAllSettings();
    displayController.showStatus("Settings Saved!", 2000);
}

void resetToDefaults() {
    logger.info("Settings", "Reset to defaults...");
    displayController.showStatus("Reset Defaults", 2000);
}

// ==================== SCALE CALIBRATION FUNCTIONS ====================

void calibrateScaleZero() {
    logger.info("Scale", "Starting zero calibration...");
    displayController.showStatus("Remove all weight", 2000);
    delay(2000);
    displayController.showStatus("Calibrating...", 1000);
    
    if (scaleController.calibrateZero()) {
        scaleController.saveCalibration();
        displayController.showStatus("Zero Set!", 2000);
        logger.info("Scale", "Zero calibration successful");
    } else {
        displayController.showStatus("Zero Failed!", 2000);
        logger.error("Scale", "Zero calibration failed");
    }
}

void calibrateScaleWithWeight() {
    logger.info("Scale", "Starting weight calibration...");
    
    char msg[21];
    snprintf(msg, 21, "Place %dg weight", scaleCalibrationWeight);
    displayController.showStatus(msg, 3000);
    delay(3000);
    displayController.showStatus("Calibrating...", 1000);
    
    if (scaleController.calibrateKnownWeight(scaleCalibrationWeight)) {
        scaleController.saveCalibration();
        displayController.showStatus("Calibrated!", 2000);
        logger.info("Scale", "Calibration successful");
    } else {
        displayController.showStatus("Cal Failed!", 2000);
        logger.error("Scale", "Calibration failed");
    }
}

void saveScaleCalibration() {
    scaleController.saveCalibration();
    displayController.showStatus("Scale Cal Saved!", 2000);
}

// ==================== DISPLAY CALLBACK ====================

void updateDisplay(const char* line1, const char* line2, bool editing) {
    displayController.displayText(line1, line2, editing);
}

void updateDisplay4Line(const char* line1, const char* line2, const char* line3, const char* line4) {
    displayController.displayText4Line(line1, line2, line3, line4);
}

// ==================== SETUP ====================

void setup() {
    // Initialize serial
    Serial.begin(115200);
    delay(2000);  // Wait for serial to be ready
    
    // Initialize logger with DEBUG level
    logger.init(LOG_DEBUG, true);
    
    logger.separator();
    logger.info("MAIN", "Egg Tray Moulder Machine v1.0");
    logger.separator();
    
    // Initialize I2C for ESP32-WROOM DevKit (SDA=21, SCL=22)
    Wire.begin(21, 22);
    Wire.setClock(100000);  // Set to 100kHz for better stability
    delay(100);  // Allow I2C to stabilize
    logger.info("I2C", "Initialized on ESP32 pins (SDA=21, SCL=22) at 100kHz");
    
    // Initialize PCF8575 expanders
    pcf8575_1.begin();
    logger.info("PCF8575_1", "Initialized at address 0x25");
    
    pcf8575_2.begin();
    logger.info("PCF8575_2", "Initialized at address 0x22");
    
    // Set all relay pins as outputs (default HIGH = relay OFF)
    for (uint8_t i = 0; i <= 15; i++) {
        pcf8575_1.pinMode(i, OUTPUT);
        pcf8575_1.digitalWrite(i, HIGH);  // Relay OFF
    }
    for (uint8_t i = 0; i <= 7; i++) {
        pcf8575_2.pinMode(i, OUTPUT);
        pcf8575_2.digitalWrite(i, HIGH);  // Relay OFF
    }
    
    // Configure direct GPIO pins for buttons and sensors
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_ENTER, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(SENSOR_WATER_FLOW, INPUT_PULLUP);
    pinMode(SENSOR_IR_TRAY, INPUT_PULLUP);
    logger.info("GPIO", "Buttons and sensors configured on direct pins");
    
    // Initialize display
    displayController.init(LCD_I2C_ADDRESS, 20, 4);
    logger.info("LCD", "Display initialized");
    displayController.showStartup("Egg Tray Moulder", "v1.0");
    delay(2000);
    
    // Initialize servo
    starchServo.attach(SERVO_PIN);
    starchServo.write(0);  // Start at closed position (0 degrees)
    servoAngle = 0;
    logger.info("Servo", "Initialized on pin", SERVO_PIN);
    
    // Initialize scale controller (NAU7802)
    if (scaleController.init()) {
        logger.info("Scale", "NAU7802 initialized successfully");
        if (scaleController.isCalibrated()) {
            logger.info("Scale", "Calibration loaded from Preferences");
        } else {
            logger.warning("Scale", "Not calibrated - please calibrate");
        }
    } else {
        logger.error("Scale", "NAU7802 initialization failed!");
    }
    
    // Test reading direct GPIO pins before button init
    logger.info("GPIO", "Reading button pins before init...");
    logger.info("GPIO", "BTN_UP", digitalRead(BTN_UP) ? "HIGH" : "LOW");
    logger.info("GPIO", "BTN_ENTER", digitalRead(BTN_ENTER) ? "HIGH" : "LOW");
    logger.info("GPIO", "BTN_DOWN", digitalRead(BTN_DOWN) ? "HIGH" : "LOW");
    
    // Initialize buttons (now using direct GPIO, not PCF8575)
    logger.debug("BTN", "Initializing buttons...");
    logger.debug("BTN", "Up pin", BTN_UP);
    logger.debug("BTN", "Enter pin", BTN_ENTER);
    logger.debug("BTN", "Down pin", BTN_DOWN);
    buttonController.init(BTN_ENTER, BTN_UP, BTN_DOWN, nullptr);  // nullptr = direct GPIO
    
    // Link menu structure
    linkMenus();
    logger.info("MENU", "Menu structure linked");
    
    // Initialize menu system
    menuController.init(menuLayers, TOTAL_LAYERS);
    menuController.setDisplayCallback(updateDisplay);
    menuController.setDisplay4LineCallback(updateDisplay4Line);
    logger.info("MENU", "Menu controller initialized");
    
    // Show initial menu
    menuController.refresh();
    
    logger.separator();
    logger.info("MAIN", "System Ready");
    logger.separator();
}

// ==================== MAIN LOOP ====================

void loop() {
    // Read raw GPIO pin states every second for debugging
    static unsigned long lastRawRead = 0;
    if (millis() - lastRawRead > 1000) {
        logger.debug("RAW", "BTN_UP", digitalRead(BTN_UP) ? "HIGH" : "LOW");
        logger.debug("RAW", "BTN_ENTER", digitalRead(BTN_ENTER) ? "HIGH" : "LOW");
        logger.debug("RAW", "BTN_DOWN", digitalRead(BTN_DOWN) ? "HIGH" : "LOW");
        lastRawRead = millis();
    }
    
    // Update button states
    ButtonState enterState = buttonController.getEnterState();
    ButtonState upState = buttonController.getUpState();
    ButtonState downState = buttonController.getDownState();
    
    // Debug log button state changes
    static ButtonState lastEnter = BUTTON_IDLE;
    static ButtonState lastUp = BUTTON_IDLE;
    static ButtonState lastDown = BUTTON_IDLE;
    
    if (enterState != BUTTON_IDLE && enterState != lastEnter) {
        logger.debug("BTN", "Enter button state", (int)enterState);
        lastEnter = enterState;
    }
    if (upState != BUTTON_IDLE && upState != lastUp) {
        logger.debug("BTN", "Up button state", (int)upState);
        lastUp = upState;
    }
    if (downState != BUTTON_IDLE && downState != lastDown) {
        logger.debug("BTN", "Down button state", (int)downState);
        lastDown = downState;
    }
    
    // Handle button inputs
    if (menuController.getState() == MENU_STATE_BROWSING) {
        // Browsing mode
        if (upState == BUTTON_PRESSED) {
            menuController.navigateUp();
        }
        if (downState == BUTTON_PRESSED) {
            menuController.navigateDown();
        }
        if (enterState == BUTTON_PRESSED) {
            menuController.selectItem();
        }
    } else if (menuController.getState() == MENU_STATE_EDITING) {
        // Editing mode
        if (upState == BUTTON_PRESSED || upState == BUTTON_REPEAT_SLOW) {
            menuController.incrementValue(false);
        }
        if (upState == BUTTON_REPEAT_FAST) {
            menuController.incrementValue(true);
        }
        if (downState == BUTTON_PRESSED || downState == BUTTON_REPEAT_SLOW) {
            menuController.decrementValue(false);
        }
        if (downState == BUTTON_REPEAT_FAST) {
            menuController.decrementValue(true);
        }
        if (enterState == BUTTON_PRESSED) {
            menuController.selectItem(); // Save and exit editing
        }
    }
    
    // Update display cursor blinking
    displayController.update();
    
    // Check display health and recover if needed
    displayController.checkHealth();
    
    // Process auto run if running
    if (systemRunning) {
        processAutoRun();
    }
}

// ==================== PROCESS FUNCTIONS ====================

void processAutoRun() {
    // Auto run process will be implemented in next phase
    // This is where the automatic sequence will execute
}
