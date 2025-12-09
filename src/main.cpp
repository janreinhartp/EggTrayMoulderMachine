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

// Configuration includes
#include "HardwareConfig.h"
#include "PCF8575_PinMap.h"
#include "SettingsConfig.h"
#include "MenuConfig.h"

// Controller includes
#include <ButtonController.h>
#include <MenuController.h>
#include <DisplayController.h>

// ==================== GLOBAL OBJECTS ====================

ButtonController buttonController;
MenuController menuController;
DisplayController displayController;

// ==================== FORWARD DECLARATIONS ====================

// Display callback
void updateDisplay(const char* line1, const char* line2, bool editing);

// Process functions (to be implemented in future steps)
void processAutoRun();

// ==================== MENU ACTION FUNCTIONS ====================

void startAutoRun() {
    Serial.println("Starting Auto Run...");
    systemRunning = true;
    systemStatus = "Running";
    displayController.showStatus("Starting Auto Run", 2000);
}

void stopAutoRun() {
    Serial.println("Stopping Auto Run...");
    systemRunning = false;
    systemStatus = "Stopped";
    displayController.showStatus("Stopped", 1000);
    menuController.reset();
}

void enterTestMode() {
    Serial.println("Entering Test Mode...");
    testMode = true;
    displayController.showStatus("Test Mode", 1000);
}

void exitTestMode() {
    Serial.println("Exiting Test Mode...");
    testMode = false;
    menuController.reset();
    displayController.showStatus("Test Exit", 1000);
}

void saveSettings() {
    Serial.println("Saving all settings...");
    menuController.saveAllSettings();
    displayController.showStatus("Settings Saved!", 2000);
}

void resetToDefaults() {
    Serial.println("Reset to defaults...");
    displayController.showStatus("Reset Defaults", 2000);
}

// ==================== DISPLAY CALLBACK ====================

void updateDisplay(const char* line1, const char* line2, bool editing) {
    displayController.displayText(line1, line2, editing);
}

// ==================== SETUP ====================

void setup() {
    // Initialize serial
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n\n=================================");
    Serial.println("Egg Tray Moulder Machine v1.0");
    Serial.println("=================================\n");
    
    // Initialize I2C
    Wire.begin(SDA_PIN, SCL_PIN);
    Serial.println("I2C initialized");
    
    // Initialize PCF8575 expanders
    pcf8575_1.begin();
    Serial.println("PCF8575 #1 initialized (16-ch relay)");
    
    pcf8575_2.begin();
    Serial.println("PCF8575 #2 initialized (8-ch relay + buttons)");
    
    // Set all relay pins as outputs (default HIGH = relay OFF)
    for (uint8_t i = 0; i <= 15; i++) {
        pcf8575_1.pinMode(i, OUTPUT);
        pcf8575_1.digitalWrite(i, HIGH);  // Relay OFF
    }
    for (uint8_t i = 0; i <= 7; i++) {
        pcf8575_2.pinMode(i, OUTPUT);
        pcf8575_2.digitalWrite(i, HIGH);  // Relay OFF
    }
    
    // Initialize display
    displayController.init(LCD_I2C_ADDRESS, 20, 4);
    displayController.showStartup("Egg Tray Moulder", "v1.0");
    delay(2000);
    
    // Initialize buttons (connected via PCF8575 #2)
    buttonController.init(BTN_ENTER, BTN_UP, BTN_DOWN, &pcf8575_2);
    
    // Link menu structure
    linkMenus();
    
    // Initialize menu system
    menuController.init(menuLayers, TOTAL_LAYERS);
    menuController.setDisplayCallback(updateDisplay);
    
    // Show initial menu
    char line1[21], line2[21];
    bool editing;
    menuController.getCurrentDisplay(line1, line2, editing);
    displayController.displayText(line1, line2, editing);
    
    Serial.println("\n=== System Ready ===\n");
}

// ==================== MAIN LOOP ====================

void loop() {
    // Update button states
    ButtonState enterState = buttonController.getEnterState();
    ButtonState upState = buttonController.getUpState();
    ButtonState downState = buttonController.getDownState();
    
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
    
    // Process auto run if running
    if (systemRunning) {
        processAutoRun();
    }
    
    // Small delay for stability
    delay(10);
}

// ==================== PROCESS FUNCTIONS ====================

void processAutoRun() {
    // Auto run process will be implemented in next phase
    // This is where the automatic sequence will execute
}
