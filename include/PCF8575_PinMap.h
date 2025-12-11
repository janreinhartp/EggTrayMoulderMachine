/*
 * PCF8575 Pin Mapping Reference
 * =============================
 * 
 * PCF8575 #1 (Address 0x20) - 16 Channel Relay Board
 * ---------------------------------------------------
 * P0-P15: Connected to 16-channel relay board
 * 
 * Pin Layout:
 * P0  = Relay 1   (Spare 1)
 * P1  = Relay 2   (Heater)
 * P2  = Relay 3   (Spare 2)
 * P3  = Relay 4   (Valve)
 * P4  = Relay 5   (Shredder Power)
 * P5  = Relay 6   (Mixer)
 * P6  = Relay 7   (Pump)
 * P7  = Relay 8   (Spare 3)
 * P8  = Relay 9   (Defective)
 * P9  = Relay 10  (Screw)
 * P10 = Relay 11  (Conveyor)
 * P11 = Relay 12  (Defective)
 * P12 = Relay 13  (Shredder Trigger On)
 * P13 = Relay 14  (Linear Door)
 * P14 = Relay 15  (Spare 5)
 * P15 = Relay 16  (Spare 6)
 * 
 * 
 * PCF8575 #2 (Address 0x21) - 8 Channel Relay + Buttons + Sensors
 * ----------------------------------------------------------------
 * P0-P7: Connected to 8-channel relay board
 * P8-P12: Buttons and sensors (inputs with pullup)
 * 
 * Pin Layout:
 * P0  = Relay 1   (Vacuum)
 * P1  = Relay 2   (Blower)
 * P2  = Relay 3   (Mould A - Vacuum/Blower)
 * P3  = Relay 4   (Vacuum - A/B)
 * P4  = Relay 5   (Blower - A/B)
 * P5  = Relay 6   (Mould B - Vacuum/Blower)
 * P6  = Relay 7   (Forward/Reverse)
 * P7  = Relay 8   (Up/Down)
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
#define RELAY_SPARE_1            P0
#define RELAY_HEATER             P1
#define RELAY_SPARE_2            P2
#define RELAY_VALVE              P3
#define RELAY_SHREDDER_POWER     P4
#define RELAY_MIXER              P5
#define RELAY_PUMP               P6
#define RELAY_SPARE_3            P7
#define RELAY_DEFECTIVE_1        P8
#define RELAY_SCREW              P9
#define RELAY_CONVEYOR           P10
#define RELAY_DEFECTIVE_2        P11
#define RELAY_SHREDDER_TRIGGER   P12
#define RELAY_LINEAR_DOOR        P13
#define RELAY_SPARE_5            P14
#define RELAY_SPARE_6            P15
// PCF8575 #2 - 8 Channel Relay Pins
#define RELAY_VACUUM             P0
#define RELAY_BLOWER             P1
#define RELAY_MOULD_A_VAC_BLOW   P2
#define RELAY_VACUUM_AB          P3
#define RELAY_BLOWER_AB          P4
#define RELAY_MOULD_B_VAC_BLOW   P5
#define RELAY_FORWARD_REVERSE    P6
#define RELAY_UP_DOWN            P7

// Note: Buttons and sensors moved to direct Arduino GPIO pins
// See HardwareConfig.h for:
// - BTN_UP (Pin 2)
// - BTN_ENTER (Pin 3)
// - BTN_DOWN (Pin 4)
// - SENSOR_WATER_FLOW (Pin 5)
// - SERVO_PIN (Pin 6)
// - SENSOR_IR_TRAY (Pin 7)

#endif // PCF8575_PINMAP_H
