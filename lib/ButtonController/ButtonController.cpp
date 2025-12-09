#include "ButtonController.h"

// ========== Button Class Implementation ==========

Button::Button() {
    pin = 0;
    lastState = HIGH;
    currentState = HIGH;
    pressStartTime = 0;
    lastRepeatTime = 0;
    isPressed = false;
    longPressDetected = false;
    usePCF = false;
    pcf = nullptr;
}

void Button::init(uint8_t buttonPin, PCF8575* pcfExpander) {
    pin = buttonPin;
    pcf = pcfExpander;
    usePCF = (pcf != nullptr);
    
    if (usePCF) {
        // PCF8575 pins are configured as inputs with pullup by default
        pcf->pinMode(pin, INPUT);
    } else {
        // Direct GPIO
        pinMode(pin, INPUT_PULLUP);
    }
    
    lastState = HIGH;
    currentState = HIGH;
    pressStartTime = 0;
    lastRepeatTime = 0;
    isPressed = false;
    longPressDetected = false;
}

bool Button::readPin() {
    if (usePCF && pcf) {
        return pcf->digitalRead(pin);
    } else {
        return digitalRead(pin);
    }
}

ButtonState Button::update() {
    // Read current button state (active LOW with pullup)
    bool reading = readPin();
    unsigned long currentTime = millis();
    
    // Debounce logic
    if (reading != lastState) {
        lastState = reading;
        return BUTTON_IDLE;
    }
    
    // Button just pressed (transition from HIGH to LOW)
    if (reading == LOW && !isPressed) {
        isPressed = true;
        pressStartTime = currentTime;
        lastRepeatTime = currentTime;
        longPressDetected = false;
        currentState = LOW;
        return BUTTON_PRESSED;
    }
    
    // Button released
    if (reading == HIGH && isPressed) {
        isPressed = false;
        currentState = HIGH;
        longPressDetected = false;
        return BUTTON_RELEASED;
    }
    
    // Button held down
    if (reading == LOW && isPressed) {
        unsigned long pressDuration = currentTime - pressStartTime;
        unsigned long repeatDuration = currentTime - lastRepeatTime;
        
        // Long press detection (1 second)
        if (!longPressDetected && pressDuration >= LONG_PRESS_TIME) {
            longPressDetected = true;
            return BUTTON_LONG_PRESS;
        }
        
        // Fast scroll (after 3 seconds of holding)
        if (pressDuration >= FAST_SCROLL_THRESHOLD) {
            if (repeatDuration >= FAST_REPEAT_TIME) {
                lastRepeatTime = currentTime;
                return BUTTON_REPEAT_FAST;
            }
        }
        // Slow scroll (between long press and fast scroll threshold)
        else if (longPressDetected && repeatDuration >= SLOW_REPEAT_TIME) {
            lastRepeatTime = currentTime;
            return BUTTON_REPEAT_SLOW;
        }
    }
    
    return BUTTON_IDLE;
}

bool Button::isButtonPressed() {
    return isPressed;
}

void Button::reset() {
    isPressed = false;
    longPressDetected = false;
    pressStartTime = 0;
    lastRepeatTime = 0;
}

// ========== ButtonController Class Implementation ==========

ButtonController::ButtonController() {
    enterPin = 0;
    upPin = 0;
    downPin = 0;
    pcfExpander = nullptr;
    buttonsInitialized = false;
}

void ButtonController::init(uint8_t enterPinNum, uint8_t upPinNum, uint8_t downPinNum, PCF8575* pcf) {
    enterPin = enterPinNum;
    upPin = upPinNum;
    downPin = downPinNum;
    pcfExpander = pcf;
    
    // Initialize each button with PCF8575 expander
    enterButton.init(enterPin, pcfExpander);
    upButton.init(upPin, pcfExpander);
    downButton.init(downPin, pcfExpander);
    
    buttonsInitialized = true;
    
    Serial.println("ButtonController initialized");
    Serial.print("Enter Pin: "); Serial.println(enterPin);
    Serial.print("Up Pin: "); Serial.println(upPin);
    Serial.print("Down Pin: "); Serial.println(downPin);
    Serial.println("Using PCF8575 I2C Expander");
}

void ButtonController::update() {
    if (!buttonsInitialized) return;
    
    // Update all buttons
    enterButton.update();
    upButton.update();
    downButton.update();
}

ButtonState ButtonController::getEnterState() {
    return enterButton.update();
}

ButtonState ButtonController::getUpState() {
    return upButton.update();
}

ButtonState ButtonController::getDownState() {
    return downButton.update();
}

bool ButtonController::isEnterPressed() {
    return enterButton.isButtonPressed();
}

bool ButtonController::isUpPressed() {
    return upButton.isButtonPressed();
}

bool ButtonController::isDownPressed() {
    return downButton.isButtonPressed();
}

void ButtonController::resetAll() {
    enterButton.reset();
    upButton.reset();
    downButton.reset();
}
