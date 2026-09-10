#include "./LogLevel.cpp"
#include <iostream>

class Logger {
    private:

    static void log(LogLevel level, bool debug_mode, const std::string& message) {
        switch (level) {
            case INFO:
                std::cout << message << std::endl;
                break;
            case DEBUG:
                if(debug_mode)
                    std::cout << "[DEBUG] " << message << std::endl;
                break;
            case WARNING:
                std::cout << "[WARNING] " << message << std::endl;
                break;
            case ERROR:
                std::cout << "[ERROR] " << message << std::endl;
                break;
    
        }
    }

    public: 
    
    static void logInfo(const std::string& message) {
        log(LogLevel::INFO, false, message);
    }

    static void logDebug(bool debug_mode, const std::string& message) {
        log(LogLevel::DEBUG, debug_mode, message);
    }

    static void logWarning(const std::string& message) {
        log(LogLevel::WARNING, false, message);
    }

    static void logError(const std::string& message) {
        log(LogLevel::ERROR, false, message);
    }

    
};
