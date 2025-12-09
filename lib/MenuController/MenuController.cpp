#include "MenuController.h"

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
    prefsInitialized = false;
}

void MenuController::init(MenuLayer* menuLayers, uint8_t count) {
    layers = menuLayers;
    layerCount = count;
    currentLayerIndex = 0;
    currentItemIndex = 0;
    currentState = MENU_STATE_BROWSING;
    stackPointer = 0;
    
    // Initialize preferences
    if (preferences.begin("egg-tray", false)) {
        prefsInitialized = true;
        Serial.println("MenuController: Preferences initialized");
    } else {
        Serial.println("MenuController: Failed to initialize preferences");
    }
    
    // Load all saved settings
    loadAllSettings();
    
    Serial.println("MenuController initialized");
    Serial.print("Number of layers: "); Serial.println(layerCount);
}

void MenuController::setDisplayCallback(void (*callback)(const char*, const char*, bool)) {
    displayCallback = callback;
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
        
        Serial.print("Navigate Up - Item: "); Serial.println(currentItemIndex);
        
        // Update display if callback is set
        if (displayCallback) {
            char line1[21], line2[21];
            bool editing;
            getCurrentDisplay(line1, line2, editing);
            displayCallback(line1, line2, editing);
        }
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
        
        Serial.print("Navigate Down - Item: "); Serial.println(currentItemIndex);
        
        // Update display if callback is set
        if (displayCallback) {
            char line1[21], line2[21];
            bool editing;
            getCurrentDisplay(line1, line2, editing);
            displayCallback(line1, line2, editing);
        }
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
                        Serial.print("Enter submenu: "); Serial.println(layers[i].name);
                        break;
                    }
                }
                break;
                
            case MENU_ITEM_ACTION:
                // Execute action
                if (item->action) {
                    Serial.print("Execute action: "); Serial.println(item->label);
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
                Serial.print("Start editing: "); Serial.println(item->label);
                break;
                
            case MENU_ITEM_BOOL:
                // Toggle boolean
                if (item->boolValue) {
                    *item->boolValue = !(*item->boolValue);
                    saveValueToPrefs(item);
                    Serial.print("Toggle bool: "); Serial.print(item->label);
                    Serial.print(" = "); Serial.println(*item->boolValue);
                }
                break;
                
            case MENU_ITEM_BACK:
                goBack();
                break;
        }
        
        // Update display
        if (displayCallback) {
            char line1[21], line2[21];
            bool editing;
            getCurrentDisplay(line1, line2, editing);
            displayCallback(line1, line2, editing);
        }
        
    } else if (currentState == MENU_STATE_EDITING) {
        // Save edited value and exit editing mode
        if (editingItem) {
            if (editingItem->type == MENU_ITEM_VALUE_INT && editingItem->intValue) {
                *editingItem->intValue = tempIntValue;
            } else if (editingItem->type == MENU_ITEM_VALUE_FLOAT && editingItem->floatValue) {
                *editingItem->floatValue = tempFloatValue;
            }
            saveValueToPrefs(editingItem);
            Serial.print("Save value: "); Serial.println(editingItem->label);
        }
        currentState = MENU_STATE_BROWSING;
        editingItem = nullptr;
        
        // Update display
        if (displayCallback) {
            char line1[21], line2[21];
            bool editing;
            getCurrentDisplay(line1, line2, editing);
            displayCallback(line1, line2, editing);
        }
    }
}

void MenuController::goBack() {
    if (currentState == MENU_STATE_EDITING) {
        // Cancel editing
        currentState = MENU_STATE_BROWSING;
        editingItem = nullptr;
        Serial.println("Cancel editing");
    } else if (stackPointer > 0) {
        // Go back to previous menu
        currentLayerIndex = popNavigation();
        currentItemIndex = 0;
        Serial.print("Go back to layer: "); Serial.println(layers[currentLayerIndex].name);
    }
    
    // Update display
    if (displayCallback) {
        char line1[21], line2[21];
        bool editing;
        getCurrentDisplay(line1, line2, editing);
        displayCallback(line1, line2, editing);
    }
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
        if (displayCallback) {
            char line1[21], line2[21];
            bool editing;
            getCurrentDisplay(line1, line2, editing);
            displayCallback(line1, line2, editing);
        }
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
        if (displayCallback) {
            char line1[21], line2[21];
            bool editing;
            getCurrentDisplay(line1, line2, editing);
            displayCallback(line1, line2, editing);
        }
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
            case MENU_ITEM_ACTION:
            case MENU_ITEM_BACK:
                snprintf(line2, 21, ">%s", item->label);
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

void MenuController::loadValueFromPrefs(MenuItem* item) {
    if (!prefsInitialized || !item->prefKey) return;
    
    switch (item->type) {
        case MENU_ITEM_VALUE_INT:
            if (item->intValue) {
                *item->intValue = preferences.getInt(item->prefKey, *item->intValue);
                Serial.print("Load "); Serial.print(item->prefKey);
                Serial.print(" = "); Serial.println(*item->intValue);
            }
            break;
            
        case MENU_ITEM_VALUE_FLOAT:
            if (item->floatValue) {
                *item->floatValue = preferences.getFloat(item->prefKey, *item->floatValue);
                Serial.print("Load "); Serial.print(item->prefKey);
                Serial.print(" = "); Serial.println(*item->floatValue);
            }
            break;
            
        case MENU_ITEM_BOOL:
            if (item->boolValue) {
                *item->boolValue = preferences.getBool(item->prefKey, *item->boolValue);
                Serial.print("Load "); Serial.print(item->prefKey);
                Serial.print(" = "); Serial.println(*item->boolValue);
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
                Serial.print("Save "); Serial.print(item->prefKey);
                Serial.print(" = "); Serial.println(*item->intValue);
            }
            break;
            
        case MENU_ITEM_VALUE_FLOAT:
            if (item->floatValue) {
                preferences.putFloat(item->prefKey, *item->floatValue);
                Serial.print("Save "); Serial.print(item->prefKey);
                Serial.print(" = "); Serial.println(*item->floatValue);
            }
            break;
            
        case MENU_ITEM_BOOL:
            if (item->boolValue) {
                preferences.putBool(item->prefKey, *item->boolValue);
                Serial.print("Save "); Serial.print(item->prefKey);
                Serial.print(" = "); Serial.println(*item->boolValue);
            }
            break;
            
        default:
            break;
    }
}

void MenuController::saveAllSettings() {
    if (!prefsInitialized) return;
    
    Serial.println("Saving all settings...");
    
    // Iterate through all layers and items
    for (uint8_t l = 0; l < layerCount; l++) {
        for (uint8_t i = 0; i < layers[l].itemCount; i++) {
            saveValueToPrefs(&layers[l].items[i]);
        }
    }
    
    Serial.println("All settings saved");
}

void MenuController::loadAllSettings() {
    if (!prefsInitialized) return;
    
    Serial.println("Loading all settings...");
    
    // Iterate through all layers and items
    for (uint8_t l = 0; l < layerCount; l++) {
        for (uint8_t i = 0; i < layers[l].itemCount; i++) {
            loadValueFromPrefs(&layers[l].items[i]);
        }
    }
    
    Serial.println("All settings loaded");
}

void MenuController::reset() {
    currentLayerIndex = 0;
    currentItemIndex = 0;
    currentState = MENU_STATE_BROWSING;
    stackPointer = 0;
    editingItem = nullptr;
    
    Serial.println("MenuController reset");
    
    // Update display
    if (displayCallback) {
        char line1[21], line2[21];
        bool editing;
        getCurrentDisplay(line1, line2, editing);
        displayCallback(line1, line2, editing);
    }
}
