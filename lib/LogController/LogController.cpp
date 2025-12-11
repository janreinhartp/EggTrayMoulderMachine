#include "LogController.h"

// Global logger instance
LogController logger;

LogController::LogController() {
    currentLevel = LOG_INFO;
    timestampEnabled = true;
    initialized = false;
}

void LogController::init(LogLevel level, bool enableTimestamp) {
    // Use build flag if defined, otherwise use parameter
    #ifdef LOG_LEVEL
        currentLevel = (LogLevel)LOG_LEVEL;
    #else
        currentLevel = level;
    #endif
    
    timestampEnabled = enableTimestamp;
    initialized = true;
    
    Serial.println();
    separator();
    Serial.println("LogController Initialized");
    Serial.print("Log Level: ");
    Serial.println(getLevelString(currentLevel));
    #ifdef LOG_LEVEL
        Serial.println("(Set via build flag)");
    #endif
    separator();
    Serial.println();
}

void LogController::setLevel(LogLevel level) {
    currentLevel = level;
    info("LogController", "Log level changed to", getLevelString(level));
}

void LogController::enableTimestamp(bool enable) {
    timestampEnabled = enable;
}

const char* LogController::getLevelString(LogLevel level) {
    switch (level) {
        case LOG_NONE: return "NONE";
        case LOG_ERROR: return "ERROR";
        case LOG_WARNING: return "WARNING";
        case LOG_INFO: return "INFO";
        case LOG_DEBUG: return "DEBUG";
        case LOG_VERBOSE: return "VERBOSE";
        default: return "UNKNOWN";
    }
}

void LogController::printTimestamp() {
    if (timestampEnabled) {
        unsigned long ms = millis();
        unsigned long seconds = ms / 1000;
        unsigned long minutes = seconds / 60;
        unsigned long hours = minutes / 60;
        
        ms = ms % 1000;
        seconds = seconds % 60;
        minutes = minutes % 60;
        
        char buffer[16];
        sprintf(buffer, "[%02lu:%02lu:%02lu.%03lu] ", hours, minutes, seconds, ms);
        Serial.print(buffer);
    }
}

void LogController::printPrefix(LogLevel level, const char* tag) {
    printTimestamp();
    Serial.print("[");
    Serial.print(getLevelString(level));
    Serial.print("] [");
    Serial.print(tag);
    Serial.print("] ");
}

// Error methods
void LogController::error(const char* tag, const char* message) {
    if (currentLevel >= LOG_ERROR) {
        printPrefix(LOG_ERROR, tag);
        Serial.println(message);
    }
}

void LogController::error(const char* tag, const char* message, int value) {
    if (currentLevel >= LOG_ERROR) {
        printPrefix(LOG_ERROR, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

void LogController::error(const char* tag, const char* message, const char* value) {
    if (currentLevel >= LOG_ERROR) {
        printPrefix(LOG_ERROR, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

// Warning methods
void LogController::warning(const char* tag, const char* message) {
    if (currentLevel >= LOG_WARNING) {
        printPrefix(LOG_WARNING, tag);
        Serial.println(message);
    }
}

void LogController::warning(const char* tag, const char* message, int value) {
    if (currentLevel >= LOG_WARNING) {
        printPrefix(LOG_WARNING, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

void LogController::warning(const char* tag, const char* message, const char* value) {
    if (currentLevel >= LOG_WARNING) {
        printPrefix(LOG_WARNING, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

// Info methods
void LogController::info(const char* tag, const char* message) {
    if (currentLevel >= LOG_INFO) {
        printPrefix(LOG_INFO, tag);
        Serial.println(message);
    }
}

void LogController::info(const char* tag, const char* message, int value) {
    if (currentLevel >= LOG_INFO) {
        printPrefix(LOG_INFO, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

void LogController::info(const char* tag, const char* message, const char* value) {
    if (currentLevel >= LOG_INFO) {
        printPrefix(LOG_INFO, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

// Debug methods
void LogController::debug(const char* tag, const char* message) {
    if (currentLevel >= LOG_DEBUG) {
        printPrefix(LOG_DEBUG, tag);
        Serial.println(message);
    }
}

void LogController::debug(const char* tag, const char* message, int value) {
    if (currentLevel >= LOG_DEBUG) {
        printPrefix(LOG_DEBUG, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

void LogController::debug(const char* tag, const char* message, bool value) {
    if (currentLevel >= LOG_DEBUG) {
        printPrefix(LOG_DEBUG, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value ? "true" : "false");
    }
}

void LogController::debug(const char* tag, const char* message, const char* value) {
    if (currentLevel >= LOG_DEBUG) {
        printPrefix(LOG_DEBUG, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

void LogController::debug(const char* tag, const char* message, unsigned long value) {
    if (currentLevel >= LOG_DEBUG) {
        printPrefix(LOG_DEBUG, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

// Verbose methods
void LogController::verbose(const char* tag, const char* message) {
    if (currentLevel >= LOG_VERBOSE) {
        printPrefix(LOG_VERBOSE, tag);
        Serial.println(message);
    }
}

void LogController::verbose(const char* tag, const char* message, int value) {
    if (currentLevel >= LOG_VERBOSE) {
        printPrefix(LOG_VERBOSE, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

void LogController::verbose(const char* tag, const char* message, const char* value) {
    if (currentLevel >= LOG_VERBOSE) {
        printPrefix(LOG_VERBOSE, tag);
        Serial.print(message);
        Serial.print(": ");
        Serial.println(value);
    }
}

// Utility methods
void LogController::separator() {
    Serial.println("=====================================");
}

void LogController::printHex(const char* tag, const char* message, uint16_t value) {
    if (currentLevel >= LOG_DEBUG) {
        printPrefix(LOG_DEBUG, tag);
        Serial.print(message);
        Serial.print(": 0x");
        Serial.println(value, HEX);
    }
}

void LogController::printBinary(const char* tag, const char* message, uint16_t value) {
    if (currentLevel >= LOG_DEBUG) {
        printPrefix(LOG_DEBUG, tag);
        Serial.print(message);
        Serial.print(": 0b");
        Serial.println(value, BIN);
    }
}
