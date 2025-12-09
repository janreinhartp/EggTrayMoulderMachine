/*
 * Menu Configuration
 * Defines the complete menu structure for the Egg Tray Moulder Machine
 */

#ifndef MENUCONFIG_H
#define MENUCONFIG_H

#include <MenuController.h>

// Forward declarations of menu action functions
void startAutoRun();
void stopAutoRun();
void enterTestMode();
void exitTestMode();
void saveSettings();
void resetToDefaults();

// External references to settings variables
extern int waterAmount;
extern int waterFlowTimeout;
extern int starchWeight;
extern int starchDispenseTime;
extern int shredderTime;
extern int mixTime;
extern int mixSpeed;
extern int pumpTime;
extern int mouldSuctionTime;
extern int mouldBlowerTime;
extern int mouldCycleDelay;
extern int dryingTime;
extern int dryingTemp;
extern int conveyorSpeed;
extern bool testMode;

// Menu items declarations
extern MenuItem mainMenuItems[];
extern MenuItem settingsMenuItems[];
extern MenuItem waterMenuItems[];
extern MenuItem starchMenuItems[];
extern MenuItem shredderMenuItems[];
extern MenuItem mixerMenuItems[];
extern MenuItem mouldingMenuItems[];
extern MenuItem dryingMenuItems[];
extern MenuItem testMenuItems[];
extern MenuItem runningMenuItems[];

// Menu layers declaration
extern MenuLayer menuLayers[];

// Menu counts
#define MAIN_MENU_COUNT 3
#define SETTINGS_MENU_COUNT 8
#define WATER_MENU_COUNT 3
#define STARCH_MENU_COUNT 3
#define SHREDDER_MENU_COUNT 2
#define MIXER_MENU_COUNT 3
#define MOULDING_MENU_COUNT 4
#define DRYING_MENU_COUNT 3
#define TEST_MENU_COUNT 1
#define RUNNING_MENU_COUNT 1
#define TOTAL_LAYERS 10

// Function to link all submenus
void linkMenus();

#endif // MENUCONFIG_H
