/*
 * PCF8575 Pin Mapping Reference
 * =============================
 * 
 * PCF8575 #1 (Address 0x20) - 16 Channel Relay Board
 * ---------------------------------------------------
 * P0-P15: Connected to 16-channel relay board
 * 
 * Pin Layout:
 * P0  = Relay 1   (Water Valve)
 * P1  = Relay 2   (Paper Shredder Motor)
 * P2  = Relay 3   (Starch Screw Dispenser)
 * P3  = Relay 4   (Mixer Motor)
 * P4  = Relay 5   (Linear Motor - Mixer Open)
 * P5  = Relay 6   (Pump Motor - Pulp Cycle)
 * P6  = Relay 7   (Cylinder A Up)
 * P7  = Relay 8   (Cylinder A Down)
 * P8  = Relay 9   (Cylinder B Forward)
 * P9  = Relay 10  (Cylinder B Backward)
 * P10 = Relay 11  (Suction Valve)
 * P11 = Relay 12  (Blower Valve)
 * P12 = Relay 13  (Valve A - Suction/Blower Routing)
 * P13 = Relay 14  (Valve B - Suction/Blower Routing)
 * P14 = Relay 15  (Conveyor Motor)
 * P15 = Relay 16  (Heater Blower)
 * 
 * 
 * PCF8575 #2 (Address 0x21) - 8 Channel Relay + Buttons + Sensors
 * ----------------------------------------------------------------
 * P0-P7: Connected to 8-channel relay board
 * P8-P12: Buttons and sensors (inputs with pullup)
 * 
 * Pin Layout:
 * P0  = Relay 1   (Valve C - Suction/Blower Routing)
 * P1  = Relay 2   (Valve D - Suction/Blower Routing)
 * P2  = Relay 3   (Reserved)
 * P3  = Relay 4   (Reserved)
 * P4  = Relay 5   (Reserved)
 * P5  = Relay 6   (Reserved)
 * P6  = Relay 7   (Reserved)
 * P7  = Relay 8   (Reserved)
 * P8  = Input     (Enter Button)
 * P9  = Input     (Up Button)
 * P10 = Input     (Down Button)
 * P11 = Input     (Water Flow Sensor)
 * P12 = Input     (IR Sensor - Tray Detection)
 * P13-P15 = Reserved for future expansion
 * 
 * 
 * Usage Notes:
 * -----------
 * - All relay outputs are active LOW (LOW = relay ON, HIGH = relay OFF)
 * - All button inputs use internal pullup (pressed = LOW, released = HIGH)
 * - Flow sensor generates pulses (LOW pulse per rotation)
 * - IR sensor is active LOW (LOW = tray detected, HIGH = no tray)
 */

#ifndef PCF8575_PINMAP_H
#define PCF8575_PINMAP_H

// PCF8575 #1 - 16 Channel Relay Board Pins
#define RELAY_WATER_VALVE        0
#define RELAY_SHREDDER           1
#define RELAY_STARCH_SCREW       2
#define RELAY_MIXER              3
#define RELAY_MIXER_LINEAR       4
#define RELAY_PULP_PUMP          5
#define RELAY_CYL_A_UP           6
#define RELAY_CYL_A_DOWN         7
#define RELAY_CYL_B_FWD          8
#define RELAY_CYL_B_BACK         9
#define RELAY_SUCTION_VALVE      10
#define RELAY_BLOWER_VALVE       11
#define RELAY_VALVE_A            12
#define RELAY_VALVE_B            13
#define RELAY_CONVEYOR           14
#define RELAY_HEATER_BLOWER      15

// PCF8575 #2 - 8 Channel Relay Pins
#define RELAY_VALVE_C            0
#define RELAY_VALVE_D            1
#define RELAY_SPARE_1            2
#define RELAY_SPARE_2            3
#define RELAY_SPARE_3            4
#define RELAY_SPARE_4            5
#define RELAY_SPARE_5            6
#define RELAY_SPARE_6            7

// PCF8575 #2 - Input Pins
#define BTN_ENTER                8
#define BTN_UP                   9
#define BTN_DOWN                 10
#define SENSOR_FLOW              11
#define SENSOR_IR_TRAY           12

#endif // PCF8575_PINMAP_H
