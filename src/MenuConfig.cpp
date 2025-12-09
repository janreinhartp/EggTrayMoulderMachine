#include "MenuConfig.h"
#include "SettingsConfig.h"

// ==================== MENU ITEMS DEFINITIONS ====================

// Main menu items
MenuItem mainMenuItems[MAIN_MENU_COUNT] = {
    {"Settings", MENU_ITEM_SUBMENU, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Run Auto", MENU_ITEM_ACTION, startAutoRun, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Test Machine", MENU_ITEM_ACTION, enterTestMode, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Settings submenu items
MenuItem settingsMenuItems[SETTINGS_MENU_COUNT] = {
    {"Water", MENU_ITEM_SUBMENU, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Starch", MENU_ITEM_SUBMENU, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Shredder", MENU_ITEM_SUBMENU, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Mixer", MENU_ITEM_SUBMENU, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Moulding", MENU_ITEM_SUBMENU, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Drying", MENU_ITEM_SUBMENU, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Save All", MENU_ITEM_ACTION, saveSettings, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr},
    {"Back", MENU_ITEM_BACK, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Water settings submenu
MenuItem waterMenuItems[WATER_MENU_COUNT] = {
    {"Amount", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &waterAmount, nullptr, nullptr, 100, 5000, 50, "ml", "waterAmt"},
    {"Timeout", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &waterFlowTimeout, nullptr, nullptr, 10, 300, 5, "sec", "waterTmo"},
    {"Back", MENU_ITEM_BACK, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Starch settings submenu
MenuItem starchMenuItems[STARCH_MENU_COUNT] = {
    {"Weight", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &starchWeight, nullptr, nullptr, 100, 2000, 50, "g", "starchWt"},
    {"Dispense Time", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &starchDispenseTime, nullptr, nullptr, 5, 60, 1, "sec", "starchTm"},
    {"Back", MENU_ITEM_BACK, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Shredder settings submenu
MenuItem shredderMenuItems[SHREDDER_MENU_COUNT] = {
    {"Run Time", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &shredderTime, nullptr, nullptr, 10, 120, 5, "sec", "shredTm"},
    {"Back", MENU_ITEM_BACK, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Mixer settings submenu
MenuItem mixerMenuItems[MIXER_MENU_COUNT] = {
    {"Mix Time", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &mixTime, nullptr, nullptr, 30, 600, 10, "sec", "mixTm"},
    {"Mix Speed", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &mixSpeed, nullptr, nullptr, 0, 100, 5, "%", "mixSpd"},
    {"Back", MENU_ITEM_BACK, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Moulding settings submenu
MenuItem mouldingMenuItems[MOULDING_MENU_COUNT] = {
    {"Suction Time", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &mouldSuctionTime, nullptr, nullptr, 3, 30, 1, "sec", "mldSucTm"},
    {"Blower Time", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &mouldBlowerTime, nullptr, nullptr, 2, 20, 1, "sec", "mldBlwTm"},
    {"Cycle Delay", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &mouldCycleDelay, nullptr, nullptr, 1, 10, 1, "sec", "mldDly"},
    {"Back", MENU_ITEM_BACK, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Drying settings submenu
MenuItem dryingMenuItems[DRYING_MENU_COUNT] = {
    {"Dry Time", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &dryingTime, nullptr, nullptr, 60, 600, 30, "sec", "dryTm"},
    {"Temperature", MENU_ITEM_VALUE_INT, nullptr, nullptr, 0, &dryingTemp, nullptr, nullptr, 50, 120, 5, "C", "dryTemp"},
    {"Back", MENU_ITEM_BACK, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Test mode menu items
MenuItem testMenuItems[TEST_MENU_COUNT] = {
    {"Exit Test", MENU_ITEM_ACTION, exitTestMode, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// Running status items
MenuItem runningMenuItems[RUNNING_MENU_COUNT] = {
    {"Stop", MENU_ITEM_ACTION, stopAutoRun, nullptr, 0, nullptr, nullptr, nullptr, 0, 0, 0, nullptr, nullptr}
};

// ==================== MENU LAYERS ====================

MenuLayer menuLayers[TOTAL_LAYERS] = {
    {"MAIN MENU", mainMenuItems, MAIN_MENU_COUNT},
    {"SETTINGS", settingsMenuItems, SETTINGS_MENU_COUNT},
    {"WATER", waterMenuItems, WATER_MENU_COUNT},
    {"STARCH", starchMenuItems, STARCH_MENU_COUNT},
    {"SHREDDER", shredderMenuItems, SHREDDER_MENU_COUNT},
    {"MIXER", mixerMenuItems, MIXER_MENU_COUNT},
    {"MOULDING", mouldingMenuItems, MOULDING_MENU_COUNT},
    {"DRYING", dryingMenuItems, DRYING_MENU_COUNT},
    {"TEST MACHINE", testMenuItems, TEST_MENU_COUNT},
    {"RUNNING", runningMenuItems, RUNNING_MENU_COUNT}
};

// ==================== LINK SUBMENUS ====================

void linkMenus() {
    // Link main menu submenus
    mainMenuItems[0].subMenu = settingsMenuItems;
    mainMenuItems[0].subMenuSize = SETTINGS_MENU_COUNT;
    
    // Link settings submenus
    settingsMenuItems[0].subMenu = waterMenuItems;
    settingsMenuItems[0].subMenuSize = WATER_MENU_COUNT;
    
    settingsMenuItems[1].subMenu = starchMenuItems;
    settingsMenuItems[1].subMenuSize = STARCH_MENU_COUNT;
    
    settingsMenuItems[2].subMenu = shredderMenuItems;
    settingsMenuItems[2].subMenuSize = SHREDDER_MENU_COUNT;
    
    settingsMenuItems[3].subMenu = mixerMenuItems;
    settingsMenuItems[3].subMenuSize = MIXER_MENU_COUNT;
    
    settingsMenuItems[4].subMenu = mouldingMenuItems;
    settingsMenuItems[4].subMenuSize = MOULDING_MENU_COUNT;
    
    settingsMenuItems[5].subMenu = dryingMenuItems;
    settingsMenuItems[5].subMenuSize = DRYING_MENU_COUNT;
}
