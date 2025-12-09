/*
 * Hardware Configuration
 * Pin definitions and I2C addresses for the Egg Tray Moulder Machine
 */

#ifndef HARDWARECONFIG_H
#define HARDWARECONFIG_H

#include <Arduino.h>
#include "PCF8575.h"

// ==================== I2C CONFIGURATION ====================

// I2C Pins (ESP32-C3)
#define SDA_PIN 8
#define SCL_PIN 9

// I2C Addresses
#define PCF8575_1_ADDRESS 0x20  // PCF8575 #1 - 16 Channel Relay
#define PCF8575_2_ADDRESS 0x21  // PCF8575 #2 - 8 Channel Relay + Buttons + Flow Sensor
#define LCD_I2C_ADDRESS 0x27    // LCD I2C Display
#define ADS1115_ADDRESS 0x48    // ADS1115 ADC

// ==================== DIRECT GPIO PINS ====================

// Servo pin (direct GPIO)
#define SERVO_PIN 10

// HX711 Load Cell Amplifier
#define HX711_DOUT_PIN 2
#define HX711_SCK_PIN 3

// ==================== PCF8575 OBJECTS ====================

// Global PCF8575 I2C Expander objects
extern PCF8575 pcf8575_1;  // 16-channel relay board
extern PCF8575 pcf8575_2;  // 8-channel relay + buttons + flow sensor

#endif // HARDWARECONFIG_H
