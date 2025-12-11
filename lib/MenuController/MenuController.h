#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <Arduino.h>
#include <EEPROM.h>

// Menu item types
enum MenuItemType {
    MENU_ITEM_SUBMENU,      // Has sub-items
    MENU_ITEM_ACTION,       // Executes an action
    MENU_ITEM_VALUE_INT,    // Editable integer value
    MENU_ITEM_VALUE_FLOAT,  // Editable float value
    MENU_ITEM_BOOL,         // Toggle boolean
    MENU_ITEM_BACK          // Return to previous menu
};

// Forward declaration
class MenuController;

// Menu item structure
struct MenuItem {
    const char* label;          // Display text
    MenuItemType type;          // Item type
    void (*action)();          // Function pointer for actions
    MenuItem* subMenu;         // Pointer to submenu items
    uint8_t subMenuSize;       // Number of submenu items
    
    // For editable values
    int* intValue;             // Pointer to integer value
    float* floatValue;         // Pointer to float value
    bool* boolValue;           // Pointer to boolean value
    int minValue;              // Minimum value (for int/float)
    int maxValue;              // Maximum value (for int/float)
    float step;                // Step size for adjustments
    const char* unit;          // Unit string (e.g., "sec", "ml")
    const char* prefKey;       // Preferences key for saving
};

// Menu layer structure
struct MenuLayer {
    const char* name;          // Layer name
    MenuItem* items;           // Array of menu items
    uint8_t itemCount;         // Number of items
};

// Menu state
enum MenuState {
    MENU_STATE_BROWSING,       // Browsing through menu items
    MENU_STATE_EDITING         // Editing a value
};

class MenuController {
private:
    MenuLayer* layers;
    uint8_t layerCount;
    uint8_t currentLayerIndex;
    uint8_t currentItemIndex;
    MenuState currentState;
    
    // Navigation history stack
    static const uint8_t MAX_DEPTH = 10;
    uint8_t navigationStack[MAX_DEPTH];
    uint8_t stackPointer;
    
    // Currently editing item
    MenuItem* editingItem;
    int tempIntValue;
    float tempFloatValue;
    
    // EEPROM base address for settings storage
    int eepromAddress;
    bool prefsInitialized;
    
    // Display callback
    void (*displayCallback)(const char* line1, const char* line2, bool editing);
    void (*displayCallback4Line)(const char* line1, const char* line2, const char* line3, const char* line4);
    
    // Helper functions
    void pushNavigation(uint8_t layerIndex);
    uint8_t popNavigation();
    void loadValueFromPrefs(MenuItem* item);
    void saveValueToPrefs(MenuItem* item);
    void updateDisplay(); // Helper to call appropriate display callback
    
public:
    MenuController();
    
    // Initialize with layers
    void init(MenuLayer* menuLayers, uint8_t count);
    
    // Set display callback function
    void setDisplayCallback(void (*callback)(const char*, const char*, bool));
    void setDisplay4LineCallback(void (*callback)(const char*, const char*, const char*, const char*));
    
    // Navigation functions
    void navigateUp();
    void navigateDown();
    void selectItem();
    void goBack();
    
    // Get current display text
    void getCurrentDisplay(char* line1, char* line2, bool& isEditing);
    void getCurrentDisplay4Line(char* line1, char* line2, char* line3, char* line4);
    
    // Force display refresh
    void refresh();
    
    // Update value while editing
    void incrementValue(bool fast);
    void decrementValue(bool fast);
    
    // Get current state
    MenuState getState() { return currentState; }
    uint8_t getCurrentLayer() { return currentLayerIndex; }
    uint8_t getCurrentItem() { return currentItemIndex; }
    
    // Save/Load all settings
    void saveAllSettings();
    void loadAllSettings();
    
    // Reset to main menu
    void reset();
};

#endif // MENUCONTROLLER_H
