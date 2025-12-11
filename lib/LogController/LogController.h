#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include <Arduino.h>

// For ESP32-C3 with CDC on boot
#if defined(ARDUINO_USB_CDC_ON_BOOT) && ARDUINO_USB_CDC_ON_BOOT
#define HWSerial Serial
#else
#define HWSerial Serial
#endif

// Log levels
enum LogLevel {
    LOG_NONE = 0,
    LOG_ERROR = 1,
    LOG_WARNING = 2,
    LOG_INFO = 3,
    LOG_DEBUG = 4,
    LOG_VERBOSE = 5
};

class LogController {
private:
    LogLevel currentLevel;
    bool timestampEnabled;
    bool initialized;
    
    const char* getLevelString(LogLevel level);
    void printTimestamp();
    void printPrefix(LogLevel level, const char* tag);

public:
    LogController();
    
    // Initialize logger
    void init(LogLevel level = LOG_INFO, bool enableTimestamp = true);
    
    // Set log level
    void setLevel(LogLevel level);
    
    // Enable/disable timestamps
    void enableTimestamp(bool enable);
    
    // Logging methods with tag
    void error(const char* tag, const char* message);
    void error(const char* tag, const char* message, int value);
    void error(const char* tag, const char* message, const char* value);
    
    void warning(const char* tag, const char* message);
    void warning(const char* tag, const char* message, int value);
    void warning(const char* tag, const char* message, const char* value);
    
    void info(const char* tag, const char* message);
    void info(const char* tag, const char* message, int value);
    void info(const char* tag, const char* message, const char* value);
    
    void debug(const char* tag, const char* message);
    void debug(const char* tag, const char* message, int value);
    void debug(const char* tag, const char* message, bool value);
    void debug(const char* tag, const char* message, const char* value);
    void debug(const char* tag, const char* message, unsigned long value);
    
    void verbose(const char* tag, const char* message);
    void verbose(const char* tag, const char* message, int value);
    void verbose(const char* tag, const char* message, const char* value);
    
    // Print separator line
    void separator();
    
    // Print formatted data
    void printHex(const char* tag, const char* message, uint16_t value);
    void printBinary(const char* tag, const char* message, uint16_t value);
};

// Global logger instance
extern LogController logger;

#endif // LOGCONTROLLER_H
