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
void exitTestMode();
void saveSettings();
void resetToDefaults();

// Relay toggle functions
void toggleRelay0();
void toggleRelay1();
void toggleRelay2();
void toggleRelay3();
void toggleRelay4();
void toggleRelay5();
void toggleRelay6();
void toggleRelay7();
void toggleRelay8();
void toggleRelay9();
void toggleRelay10();
void toggleRelay11();
void toggleRelay12();
void toggleRelay13();
void toggleRelay14();
void toggleRelay15();
void toggleRelay16();
void toggleRelay17();
void toggleRelay18();
void toggleRelay19();
void toggleRelay20();
void toggleRelay21();
void toggleRelay22();
void toggleRelay23();

// Servo control functions
void toggleServo();

// Scale calibration functions
void startScaleCalibration();
void calibrateScaleZero();
void calibrateScaleWithWeight();
void saveScaleCalibration();

// External references to relay states
extern bool relayStates[24];
extern const char* relayNames[24];
extern int servoAngle;

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
extern MenuItem scaleCalMenuItems[];
extern MenuItem testMenuItems[];
extern MenuItem runningMenuItems[];

// Menu layers declaration
extern MenuLayer menuLayers[];

// Menu counts
#define MAIN_MENU_COUNT 3
#define SETTINGS_MENU_COUNT 9
#define WATER_MENU_COUNT 3
#define STARCH_MENU_COUNT 3
#define SHREDDER_MENU_COUNT 2
#define MIXER_MENU_COUNT 3
#define MOULDING_MENU_COUNT 4
#define DRYING_MENU_COUNT 3
#define SCALE_CAL_MENU_COUNT 4
#define TEST_MENU_COUNT 19
#define RUNNING_MENU_COUNT 1
#define TOTAL_LAYERS 11

// Function to link all submenus
void linkMenus();

#endif // MENUCONFIG_H
