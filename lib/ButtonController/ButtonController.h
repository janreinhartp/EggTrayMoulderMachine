#ifndef BUTTONCONTROLLER_H
#define BUTTONCONTROLLER_H

#include <Arduino.h>
#include "PCF8575.h"

// Forward declare to avoid circular dependency
class LogController;
extern LogController logger;

// Button states
enum ButtonState {
    BUTTON_IDLE,
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    BUTTON_LONG_PRESS,
    BUTTON_REPEAT_FAST,
    BUTTON_REPEAT_SLOW
};

// Button class for individual button management
class Button {
private:
    uint8_t pin;
    bool lastState;
    bool currentState;
    unsigned long pressStartTime;
    unsigned long lastRepeatTime;
    bool isPressed;
    bool longPressDetected;
    bool usePCF;
    PCF8575* pcf;
    
    // Timing constants
    static const unsigned long DEBOUNCE_DELAY = 50;        // 50ms debounce
    static const unsigned long LONG_PRESS_TIME = 1000;     // 1 second for long press
    static const unsigned long FAST_REPEAT_TIME = 100;     // 100ms for fast scroll
    static const unsigned long SLOW_REPEAT_TIME = 500;     // 500ms for slow scroll
    static const unsigned long FAST_SCROLL_THRESHOLD = 3000; // Switch to fast after 3 seconds

public:
    Button();
    void init(uint8_t buttonPin, PCF8575* pcfExpander = nullptr);
    ButtonState update();
    bool isButtonPressed();
    void reset();
    
private:
    bool readPin();
};

// ButtonController manages all buttons
class ButtonController {
private:
    Button enterButton;
    Button upButton;
    Button downButton;
    
    uint8_t enterPin;
    uint8_t upPin;
    uint8_t downPin;
    
    PCF8575* pcfExpander;
    
    bool buttonsInitialized;

public:
    ButtonController();
    
    // Initialize buttons with PCF8575 expander
    void init(uint8_t enterPinNum, uint8_t upPinNum, uint8_t downPinNum, PCF8575* pcf);
    
    // Update all buttons (call in loop)
    void update();
    
    // Get button states
    ButtonState getEnterState();
    ButtonState getUpState();
    ButtonState getDownState();
    
    // Check if buttons are pressed
    bool isEnterPressed();
    bool isUpPressed();
    bool isDownPressed();
    
    // Reset all buttons
    void resetAll();
};

#endif // BUTTONCONTROLLER_H
