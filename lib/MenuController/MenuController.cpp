#include "MenuController.h"
#include <Preferences.h>
#include <LogController.h>

extern LogController logger;
Preferences preferences;

MenuController::MenuController() {
    layers = nullptr;
    layerCount = 0;
    currentLayerIndex = 0;
    currentItemIndex = 0;
    currentState = MENU_STATE_BROWSING;
    stackPointer = 0;
    editingItem = nullptr;
    tempIntValue = 0;
    tempFloatValue = 0.0;
    displayCallback = nullptr;
    displayCallback4Line = nullptr;
    prefsInitialized = false;
    eepromAddress = 0;
}

void MenuController::init(MenuLayer* menuLayers, uint8_t count) {
    layers = menuLayers;
    layerCount = count;
    currentLayerIndex = 0;
    currentItemIndex = 0;
    currentState = MENU_STATE_BROWSING;
    stackPointer = 0;
    
    // Initialize Preferences (ESP32 non-volatile storage)
    preferences.begin("eggTrayMachine", false);  // Open in read-write mode
    eepromAddress = 0;
    prefsInitialized = true;
    logger.debug("MenuCtrl", "Preferences ready");
    
    // Load all saved settings
    loadAllSettings();
}

void MenuController::setDisplayCallback(void (*callback)(const char*, const char*, bool)) {
    displayCallback = callback;
}

void MenuController::setDisplay4LineCallback(void (*callback)(const char*, const char*, const char*, const char*)) {
    displayCallback4Line = callback;
}

void MenuController::updateDisplay() {
    // Check if we're in TEST MACHINE menu and use 4-line display
    if (displayCallback4Line && strcmp(layers[currentLayerIndex].name, "TEST MACHINE") == 0) {
        char line1[21], line2[21], line3[21], line4[21];
        getCurrentDisplay4Line(line1, line2, line3, line4);
        displayCallback4Line(line1, line2, line3, line4);
    } else if (displayCallback) {
        char line1[21], line2[21];
        bool editing;
        getCurrentDisplay(line1, line2, editing);
        displayCallback(line1, line2, editing);
    }
}

void MenuController::pushNavigation(uint8_t layerIndex) {
    if (stackPointer < MAX_DEPTH) {
        navigationStack[stackPointer++] = currentLayerIndex;
    }
}

uint8_t MenuController::popNavigation() {
    if (stackPointer > 0) {
        return navigationStack[--stackPointer];
    }
    return 0; // Default to first layer
}

void MenuController::navigateUp() {
    if (currentState == MENU_STATE_BROWSING) {
        // Move up in menu
        if (currentItemIndex > 0) {
            currentItemIndex--;
        } else {
            // Wrap to bottom
            currentItemIndex = layers[currentLayerIndex].itemCount - 1;
        }
        
        // Update display
        updateDisplay();
    }
}

void MenuController::navigateDown() {
    if (currentState == MENU_STATE_BROWSING) {
        // Move down in menu
        if (currentItemIndex < layers[currentLayerIndex].itemCount - 1) {
            currentItemIndex++;
        } else {
            // Wrap to top
            currentItemIndex = 0;
        }
        
        // Update display
        updateDisplay();
    }
}

void MenuController::selectItem() {
    MenuItem* item = &layers[currentLayerIndex].items[currentItemIndex];
    
    if (currentState == MENU_STATE_BROWSING) {
        // Handle item selection based on type
        switch (item->type) {
            case MENU_ITEM_SUBMENU:
                // Navigate to submenu
                pushNavigation(currentLayerIndex);
                // Find layer index for this submenu
                for (uint8_t i = 0; i < layerCount; i++) {
                    if (layers[i].items == item->subMenu) {
                        currentLayerIndex = i;
                        currentItemIndex = 0;
                        break;
                    }
                }
                break;
                
            case MENU_ITEM_ACTION:
                // Execute action
                if (item->action) {
                    item->action();
                }
                break;
                
            case MENU_ITEM_VALUE_INT:
            case MENU_ITEM_VALUE_FLOAT:
                // Enter editing mode
                currentState = MENU_STATE_EDITING;
                editingItem = item;
                if (item->type == MENU_ITEM_VALUE_INT && item->intValue) {
                    tempIntValue = *item->intValue;
                } else if (item->type == MENU_ITEM_VALUE_FLOAT && item->floatValue) {
                    tempFloatValue = *item->floatValue;
                }
                break;
                
            case MENU_ITEM_BOOL:
                // Toggle boolean
                if (item->boolValue) {
                    *item->boolValue = !(*item->boolValue);
                    saveValueToPrefs(item);
                }
                break;
                
            case MENU_ITEM_BACK:
                goBack();
                break;
        }
        
        // Update display
        updateDisplay();
        
    } else if (currentState == MENU_STATE_EDITING) {
        // Save edited value and exit editing mode
        if (editingItem) {
            if (editingItem->type == MENU_ITEM_VALUE_INT && editingItem->intValue) {
                *editingItem->intValue = tempIntValue;
            } else if (editingItem->type == MENU_ITEM_VALUE_FLOAT && editingItem->floatValue) {
                *editingItem->floatValue = tempFloatValue;
            }
            saveValueToPrefs(editingItem);
        }
        currentState = MENU_STATE_BROWSING;
        editingItem = nullptr;
        
        // Update display
        updateDisplay();
    }
}

void MenuController::goBack() {
    if (currentState == MENU_STATE_EDITING) {
        // Cancel editing
        currentState = MENU_STATE_BROWSING;
        editingItem = nullptr;
    } else if (stackPointer > 0) {
        // Go back to previous menu
        currentLayerIndex = popNavigation();
        currentItemIndex = 0;
    }
    
    // Update display
    updateDisplay();
}

void MenuController::incrementValue(bool fast) {
    if (currentState == MENU_STATE_EDITING && editingItem) {
        float step = editingItem->step;
        if (fast) step *= 10; // 10x faster for fast scroll
        
        if (editingItem->type == MENU_ITEM_VALUE_INT) {
            tempIntValue += (int)step;
            if (tempIntValue > editingItem->maxValue) {
                tempIntValue = editingItem->maxValue;
            }
        } else if (editingItem->type == MENU_ITEM_VALUE_FLOAT) {
            tempFloatValue += step;
            if (tempFloatValue > (float)editingItem->maxValue) {
                tempFloatValue = (float)editingItem->maxValue;
            }
        }
        
        // Update display
        updateDisplay();
    }
}

void MenuController::decrementValue(bool fast) {
    if (currentState == MENU_STATE_EDITING && editingItem) {
        float step = editingItem->step;
        if (fast) step *= 10; // 10x faster for fast scroll
        
        if (editingItem->type == MENU_ITEM_VALUE_INT) {
            tempIntValue -= (int)step;
            if (tempIntValue < editingItem->minValue) {
                tempIntValue = editingItem->minValue;
            }
        } else if (editingItem->type == MENU_ITEM_VALUE_FLOAT) {
            tempFloatValue -= step;
            if (tempFloatValue < (float)editingItem->minValue) {
                tempFloatValue = (float)editingItem->minValue;
            }
        }
        
        // Update display
        updateDisplay();
    }
}

void MenuController::getCurrentDisplay(char* line1, char* line2, bool& isEditing) {
    isEditing = (currentState == MENU_STATE_EDITING);
    
    // Line 1: Layer name
    snprintf(line1, 21, "%s", layers[currentLayerIndex].name);
    
    // Line 2: Current item
    MenuItem* item = &layers[currentLayerIndex].items[currentItemIndex];
    
    if (currentState == MENU_STATE_EDITING && editingItem) {
        // Show editing value
        if (editingItem->type == MENU_ITEM_VALUE_INT) {
            snprintf(line2, 21, ">%s:%d%s", editingItem->label, tempIntValue, 
                     editingItem->unit ? editingItem->unit : "");
        } else if (editingItem->type == MENU_ITEM_VALUE_FLOAT) {
            snprintf(line2, 21, ">%s:%.1f%s", editingItem->label, tempFloatValue,
                     editingItem->unit ? editingItem->unit : "");
        }
    } else {
        // Show normal item
        switch (item->type) {
            case MENU_ITEM_SUBMENU:
            case MENU_ITEM_BACK:
                snprintf(line2, 21, ">%s", item->label);
                break;
                
            case MENU_ITEM_ACTION:
                // Check if this is a relay toggle action (in TEST MACHINE menu)
                if (strcmp(layers[currentLayerIndex].name, "TEST MACHINE") == 0 && 
                    strcmp(item->label, "Exit Test") != 0) {
                    // This is a relay item - show state
                    // Find which relay by matching the label
                    extern bool relayStates[24];
                    extern const char* relayNames[24];
                    
                    int relayIndex = -1;
                    for (int i = 0; i < 24; i++) {
                        if (strcmp(item->label, relayNames[i]) == 0) {
                            relayIndex = i;
                            break;
                        }
                    }
                    
                    if (relayIndex >= 0) {
                        snprintf(line2, 21, ">%s [%s]", item->label, 
                                 relayStates[relayIndex] ? "ON" : "OFF");
                    } else {
                        snprintf(line2, 21, ">%s", item->label);
                    }
                } else {
                    snprintf(line2, 21, ">%s", item->label);
                }
                break;
                
            case MENU_ITEM_VALUE_INT:
                if (item->intValue) {
                    snprintf(line2, 21, ">%s:%d%s", item->label, *item->intValue,
                             item->unit ? item->unit : "");
                }
                break;
                
            case MENU_ITEM_VALUE_FLOAT:
                if (item->floatValue) {
                    snprintf(line2, 21, ">%s:%.1f%s", item->label, *item->floatValue,
                             item->unit ? item->unit : "");
                }
                break;
                
            case MENU_ITEM_BOOL:
                if (item->boolValue) {
                    snprintf(line2, 21, ">%s:%s", item->label, *item->boolValue ? "ON" : "OFF");
                }
                break;
        }
    }
}

void MenuController::refresh() {
    // Force display update using the helper
    updateDisplay();
}

void MenuController::getCurrentDisplay4Line(char* line1, char* line2, char* line3, char* line4) {
    // Line 1: Menu name
    snprintf(line1, 21, "%s", layers[currentLayerIndex].name);
    
    MenuItem* item = &layers[currentLayerIndex].items[currentItemIndex];
    
    if (strcmp(item->label, "Exit Test") == 0) {
        // Exit Test item
        snprintf(line2, 21, "%s", item->label);
        snprintf(line3, 21, "");
        snprintf(line4, 21, "Press to Exit");
    } else if (strcmp(item->label, "Starch Servo") == 0) {
        // Servo item - check servo angle
        extern int servoAngle;
        snprintf(line2, 21, "%s", item->label);
        snprintf(line3, 21, "Angle: %d deg", servoAngle);
        snprintf(line4, 21, "Click to %s", servoAngle == 0 ? "Open" : "Close");
    } else {
        // Regular relay item
        extern bool relayStates[24];
        extern const char* relayNames[24];
        
        int relayIndex = -1;
        for (int i = 0; i < 24; i++) {
            if (strcmp(item->label, relayNames[i]) == 0) {
                relayIndex = i;
                break;
            }
        }
        
        // Line 2: Relay name
        snprintf(line2, 21, "%s", item->label);
        
        // Line 3: Status
        if (relayIndex >= 0) {
            snprintf(line3, 21, "Status: %s", relayStates[relayIndex] ? "ON" : "OFF");
            snprintf(line4, 21, "Click to %s", relayStates[relayIndex] ? "Turn OFF" : "Turn ON");
        } else {
            snprintf(line3, 21, "Status: Unknown");
            snprintf(line4, 21, "Click to Toggle");
        }
    }
}

void MenuController::loadValueFromPrefs(MenuItem* item) {
    if (!prefsInitialized || !item->prefKey) return;
    
    switch (item->type) {
        case MENU_ITEM_VALUE_INT:
            if (item->intValue) {
                *item->intValue = preferences.getInt(item->prefKey, *item->intValue);
            }
            break;
            
        case MENU_ITEM_VALUE_FLOAT:
            if (item->floatValue) {
                *item->floatValue = preferences.getFloat(item->prefKey, *item->floatValue);
            }
            break;
            
        case MENU_ITEM_BOOL:
            if (item->boolValue) {
                *item->boolValue = preferences.getBool(item->prefKey, *item->boolValue);
            }
            break;
            
        default:
            break;
    }
}

void MenuController::saveValueToPrefs(MenuItem* item) {
    if (!prefsInitialized || !item->prefKey) return;
    
    switch (item->type) {
        case MENU_ITEM_VALUE_INT:
            if (item->intValue) {
                preferences.putInt(item->prefKey, *item->intValue);
            }
            break;
            
        case MENU_ITEM_VALUE_FLOAT:
            if (item->floatValue) {
                preferences.putFloat(item->prefKey, *item->floatValue);
            }
            break;
            
        case MENU_ITEM_BOOL:
            if (item->boolValue) {
                preferences.putBool(item->prefKey, *item->boolValue);
            }
            break;
            
        default:
            break;
    }
}

void MenuController::saveAllSettings() {
    if (!prefsInitialized) return;
    
    logger.debug("Prefs", "Saving all settings...");
    
    // Iterate through all layers and items
    for (uint8_t l = 0; l < layerCount; l++) {
        for (uint8_t i = 0; i < layers[l].itemCount; i++) {
            saveValueToPrefs(&layers[l].items[i]);
        }
    }
    
    logger.debug("Prefs", "All settings saved");
}

void MenuController::loadAllSettings() {
    if (!prefsInitialized) return;
    
    logger.debug("Prefs", "Loading all settings...");
    
    // Iterate through all layers and items
    for (uint8_t l = 0; l < layerCount; l++) {
        for (uint8_t i = 0; i < layers[l].itemCount; i++) {
            loadValueFromPrefs(&layers[l].items[i]);
        }
    }
    
    logger.debug("Prefs", "All settings loaded");
}

void MenuController::reset() {
    currentLayerIndex = 0;
    currentItemIndex = 0;
    currentState = MENU_STATE_BROWSING;
    stackPointer = 0;
    editingItem = nullptr;
    
    // Update display
    if (displayCallback) {
        char line1[21], line2[21];
        bool editing;
        getCurrentDisplay(line1, line2, editing);
        displayCallback(line1, line2, editing);
    }
}
