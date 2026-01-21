#include "DisplayController.h"

DisplayController::DisplayController() {
    lcd = nullptr;
    i2cAddress = 0x27;
    columns = 20;
    rows = 4;
    initialized = false;
    currentLine1 = "";
    currentLine2 = "";
    currentEditMode = false;
    lastBlinkTime = 0;
    cursorVisible = true;
    lastHealthCheck = 0;
    consecutiveFailures = 0;
}

DisplayController::~DisplayController() {
    if (lcd) {
        delete lcd;
    }
}

void DisplayController::init(uint8_t address, uint8_t cols, uint8_t rows_) {
    i2cAddress = address;
    columns = cols;
    rows = rows_;
    
    // Add delay for I2C stability on ESP32
    delay(100);
    
    // Create LCD object
    lcd = new LiquidCrystal_I2C(i2cAddress, columns, rows);
    
    // Initialize LCD with delays for ESP32
    lcd->init();
    delay(50);
    lcd->backlight();
    delay(50);
    lcd->clear();
    delay(50);
    
    // Test write to confirm LCD is working
    lcd->setCursor(0, 0);
    lcd->print("Initializing...");
    delay(100);
    
    initialized = true;
    
    Serial.println("DisplayController initialized");
    Serial.print("I2C Address: 0x"); Serial.println(i2cAddress, HEX);
    Serial.print("Size: "); Serial.print(columns); Serial.print("x"); Serial.println(rows);
}

void DisplayController::displayText(const char* line1, const char* line2, bool editing) {
    if (!initialized || !lcd) return;
    
    String l1 = String(line1);
    String l2 = line2 ? String(line2) : "";
    
    // Check if content changed
    if (l1 != currentLine1 || l2 != currentLine2 || editing != currentEditMode) {
        currentLine1 = l1;
        currentLine2 = l2;
        currentEditMode = editing;
        
        // Clear display
        lcd->clear();
        
        // Line 1 - pad with spaces to clear remnants
        lcd->setCursor(0, 0);
        char padded1[21];
        snprintf(padded1, 21, "%-20s", currentLine1.c_str());
        lcd->print(padded1);
        
        // Line 2 - pad with spaces to clear remnants
        if (currentLine2.length() > 0) {
            lcd->setCursor(0, 1);
            char padded2[21];
            snprintf(padded2, 21, "%-20s", currentLine2.c_str());
            lcd->print(padded2);
        }
        
        // Show cursor if editing
        if (editing) {
            lcd->cursor();
            lcd->blink();
            lcd->setCursor(currentLine2.length(), 1);
        } else {
            lcd->noCursor();
            lcd->noBlink();
        }
    }
}

void DisplayController::displayText(String line1, String line2, bool editing) {
    displayText(line1.c_str(), line2.c_str(), editing);
}

void DisplayController::displayText4Line(const char* line1, const char* line2, const char* line3, const char* line4) {
    if (!initialized || !lcd) return;
    
    // Clear display
    lcd->clear();
    
    // Line 1 (row 0)
    lcd->setCursor(0, 0);
    char padded1[21];
    snprintf(padded1, 21, "%-20s", line1);
    lcd->print(padded1);
    
    // Line 2 (row 1)
    lcd->setCursor(0, 1);
    char padded2[21];
    snprintf(padded2, 21, "%-20s", line2);
    lcd->print(padded2);
    
    // Line 3 (row 2)
    lcd->setCursor(0, 2);
    char padded3[21];
    snprintf(padded3, 21, "%-20s", line3);
    lcd->print(padded3);
    
    // Line 4 (row 3)
    lcd->setCursor(0, 3);
    char padded4[21];
    snprintf(padded4, 21, "%-20s", line4);
    lcd->print(padded4);
}

void DisplayController::clear() {
    if (!initialized || !lcd) return;
    
    lcd->clear();
    lcd->noCursor();
    lcd->noBlink();
    currentLine1 = "";
    currentLine2 = "";
    currentEditMode = false;
}

void DisplayController::update() {
    if (!initialized || !lcd || !currentEditMode) return;
    
    // Blink cursor in edit mode
    unsigned long currentTime = millis();
    if (currentTime - lastBlinkTime >= BLINK_INTERVAL) {
        lastBlinkTime = currentTime;
        cursorVisible = !cursorVisible;
        
        if (cursorVisible) {
            lcd->cursor();
        } else {
            lcd->noCursor();
        }
    }
}

void DisplayController::setBacklight(bool on) {
    if (!initialized || !lcd) return;
    
    if (on) {
        lcd->backlight();
    } else {
        lcd->noBacklight();
    }
}

void DisplayController::showStartup(const char* title, const char* version) {
    if (!initialized || !lcd) return;
    
    lcd->clear();
    
    // Center title on first line
    char buffer[21];
    centerText(buffer, title, columns);
    lcd->setCursor(0, 0);
    lcd->print(buffer);
    
    // Center version on second line
    if (version) {
        centerText(buffer, version, columns);
        lcd->setCursor(0, 1);
        lcd->print(buffer);
    }
    
    Serial.println("Startup screen displayed");
}

void DisplayController::showStatus(const char* message, unsigned long duration) {
    if (!initialized || !lcd) return;
    
    // Save current state
    String savedLine1 = currentLine1;
    String savedLine2 = currentLine2;
    bool savedEditMode = currentEditMode;
    
    // Show status message
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Status:");
    lcd->setCursor(0, 1);
    lcd->print(message);
    
    delay(duration);
    
    // Restore previous display
    currentLine1 = "";
    currentLine2 = "";
    displayText(savedLine1.c_str(), savedLine2.c_str(), savedEditMode);
}

void DisplayController::centerText(char* buffer, const char* text, uint8_t width) {
    uint8_t textLen = strlen(text);
    uint8_t padding = (width > textLen) ? (width - textLen) / 2 : 0;
    
    memset(buffer, ' ', width);
    buffer[width] = '\0';
    
    if (textLen <= width) {
        memcpy(buffer + padding, text, textLen);
    } else {
        memcpy(buffer, text, width);
    }
}

bool DisplayController::checkI2CConnection() {
    // Try to communicate with LCD via I2C
    Wire.beginTransmission(i2cAddress);
    uint8_t error = Wire.endTransmission();
    return (error == 0);  // 0 = success
}

void DisplayController::forceRefresh() {
    if (!initialized || !lcd) return;
    
    // Save current content
    String savedLine1 = currentLine1;
    String savedLine2 = currentLine2;
    bool savedEditMode = currentEditMode;
    
    // Clear and redraw
    lcd->clear();
    delay(5);
    
    // Force redraw by clearing current state
    currentLine1 = "";
    currentLine2 = "";
    displayText(savedLine1.c_str(), savedLine2.c_str(), savedEditMode);
}

void DisplayController::checkHealth() {
    if (!initialized) return;
    
    unsigned long currentTime = millis();
    
    // Only check periodically to avoid I2C bus congestion
    if (currentTime - lastHealthCheck < HEALTH_CHECK_INTERVAL) {
        return;
    }
    
    lastHealthCheck = currentTime;
    
    // Check I2C connection
    if (!checkI2CConnection()) {
        consecutiveFailures++;
        Serial.print("LCD I2C failure detected (");
        Serial.print(consecutiveFailures);
        Serial.println("/2)");
        
        // If multiple failures, reinitialize LCD
        if (consecutiveFailures >= MAX_FAILURES_BEFORE_RESET) {
            Serial.println("LCD reinitialization triggered!");
            
            // Delete and recreate LCD object
            if (lcd) {
                delete lcd;
            }
            
            lcd = new LiquidCrystal_I2C(i2cAddress, columns, rows);
            lcd->init();
            lcd->backlight();
            lcd->clear();
            
            // Restore display content
            if (currentLine1.length() > 0) {
                displayText(currentLine1.c_str(), currentLine2.c_str(), currentEditMode);
            }
            
            consecutiveFailures = 0;
            Serial.println("LCD reinitialized successfully");
        }
    } else {
        // Connection OK, reset failure counter
        if (consecutiveFailures > 0) {
            Serial.println("LCD I2C connection restored");
            consecutiveFailures = 0;
        }
    }
}
