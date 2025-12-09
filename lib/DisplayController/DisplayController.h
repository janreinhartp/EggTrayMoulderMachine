#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class DisplayController {
private:
    LiquidCrystal_I2C* lcd;
    uint8_t i2cAddress;
    uint8_t columns;
    uint8_t rows;
    bool initialized;
    
    // Current display content (for change detection)
    String currentLine1;
    String currentLine2;
    bool currentEditMode;
    
    // Cursor blink for editing mode
    unsigned long lastBlinkTime;
    bool cursorVisible;
    static const unsigned long BLINK_INTERVAL = 500; // 500ms blink rate

public:
    DisplayController();
    ~DisplayController();
    
    // Initialize LCD with I2C address
    void init(uint8_t address = 0x27, uint8_t cols = 20, uint8_t rows = 4);
    
    // Display text on specific lines
    void displayText(const char* line1, const char* line2 = nullptr, bool editing = false);
    void displayText(String line1, String line2 = "", bool editing = false);
    
    // Clear display
    void clear();
    
    // Update cursor blink (call in loop for editing mode)
    void update();
    
    // Set backlight
    void setBacklight(bool on);
    
    // Direct LCD access
    LiquidCrystal_I2C* getLCD() { return lcd; }
    
    // Show startup message
    void showStartup(const char* title, const char* version);
    
    // Show status message (temporary)
    void showStatus(const char* message, unsigned long duration = 2000);
    
    // Utility functions
    void centerText(char* buffer, const char* text, uint8_t width);
};

#endif // DISPLAYCONTROLLER_H
