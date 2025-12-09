#include "HardwareConfig.h"

// ==================== PCF8575 OBJECTS ====================

// PCF8575 I2C Expander objects
PCF8575 pcf8575_1(PCF8575_1_ADDRESS);  // 16-channel relay board
PCF8575 pcf8575_2(PCF8575_2_ADDRESS);  // 8-channel relay + buttons + flow sensor
