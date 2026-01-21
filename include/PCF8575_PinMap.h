/*
 * PCF8575 Pin Mapping Reference
 * =============================
 * 
 * PCF8575 #1 (Address 0x25) - 16 Channel Relay Board
 * ---------------------------------------------------
 * P0-P15: Connected to 16-channel relay board
 * 
 * Pin Layout:
 * P0  = Relay 1   (Spare 1)
 * P1  = Relay 2   (Spare 2)
 * P2  = Relay 3   (Linear Door)
 * P3  = Relay 4   (Spare 3)
 * P4  = Relay 5   (Defective 1)
 * P5  = Relay 6   (Conveyor)
 * P6  = Relay 7   (Screw)
 * P7  = Relay 8   (Defective 2)
 * P8  = Relay 9   (Shredder Power)
 * P9  = Relay 10  (Pump)
 * P10 = Relay 11  (Mixer)
 * P11 = Relay 12  (Shredder Main Power)
 * P12 = Relay 13  (Valve)
 * P13 = Relay 14  (Spare 4)
 * P14 = Relay 15  (Heater)
 * P15 = Relay 16  (Spare 5)
 * 
 * 
 * PCF8575 #2 (Address 0x22) - 8 Channel Relay + Buttons + Sensors
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

// PCF8575 #1 - 16 Channel Relay Board Pins (P0..P15)
#define RELAY_SPARE_1              P0
#define RELAY_SPARE_2              P1
#define RELAY_LINEAR               P2
#define RELAY_SPARE_3              P3
#define RELAY_DEFECTIVE_1          P4
#define RELAY_CONVEYOR             P5
#define RELAY_SCREW                P6
#define RELAY_DEFECTIVE_2          P7
#define RELAY_SHREDDER_POWER       P8   // ON/OFF
#define RELAY_PUMP                 P9
#define RELAY_MIXER                P10
#define RELAY_SHREDDER_MAIN_POWER  P11
#define RELAY_VALVE                P12
#define RELAY_SPARE_4              P13
#define RELAY_HEATER               P14
#define RELAY_SPARE_5              P15
// Note: PCF8575 has pins P0..P15 (16 total). "Spare 6" is not mappable on this expander.

// PCF8575 #2 - 8 Channel Relay Pins (P0..P7)
#define RELAY_VACUUM               P0
#define RELAY_BLOWER               P1
#define RELAY_MOULD_A_VAC_BLOW     P2  // Mould A: Vacuum & Blower
#define RELAY_VACUUM_AB            P3  // Vacuum: A & B
#define RELAY_BLOWER_AB            P4  // Blower: A & B
#define RELAY_MOULD_B_VAC_BLOW     P5  // Mould B: Vacuum & Blower
#define RELAY_FORWARD_REVERSE      P6
#define RELAY_UP_DOWN              P7

// Note: Buttons and sensors use direct ESP32 GPIO pins
// See HardwareConfig.h for:
// - BTN_UP (GPIO 25)
// - BTN_ENTER (GPIO 26)
// - BTN_DOWN (GPIO 27)
// - SENSOR_WATER_FLOW (GPIO 23)
// - SERVO_PIN (GPIO 14)
// - SENSOR_IR_TRAY (GPIO 19)

#endif // PCF8575_PINMAP_H
