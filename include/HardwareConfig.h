/*
 * Hardware Configuration
 * Pin definitions and I2C addresses for the Egg Tray Moulder Machine
 */

#ifndef HARDWARECONFIG_H
#define HARDWARECONFIG_H

#include <Arduino.h>
#include "PCF8575.h"

// ==================== I2C CONFIGURATION ====================

// ESP32-WROOM DevKit I2C pins (used via Wire.begin(21,22))
// SDA = 21, SCL = 22

// I2C Addresses
#define PCF8575_1_ADDRESS 0x25  // PCF8575 #1 - 16 Channel Relay
#define PCF8575_2_ADDRESS 0x22  // PCF8575 #2 - 8 Channel Relay + Buttons + Flow Sensor
#define LCD_I2C_ADDRESS 0x27    // LCD I2C Display
#define ADS1115_ADDRESS 0x48    // ADS1115 ADC

// ==================== DIRECT GPIO PINS (ESP32-WROOM DevKit) ====================

// Button pins (use GPIOs with internal pullups)
#define BTN_UP 25
#define BTN_ENTER 26
#define BTN_DOWN 27

// Sensor pins
#define SENSOR_WATER_FLOW 32
#define SENSOR_IR_TRAY 19

// Servo pin
#define SERVO_PIN 33

// HX711 Load Cell Amplifier
#define HX711_DOUT_PIN 16
#define HX711_SCK_PIN 17

// ==================== PCF8575 OBJECTS ====================

// Global PCF8575 I2C Expander objects
extern PCF8575 pcf8575_1;  // 16-channel relay board
extern PCF8575 pcf8575_2;  // 8-channel relay + buttons + flow sensor

#endif // HARDWARECONFIG_H
