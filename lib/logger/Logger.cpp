#include "./LogLevel.cpp"
#include <iostream>

using namespace std;

class Logger {
    private:

    static void log(LogLevel level, bool debug_mode, const string& message) {
        switch (level) {
            case INFO:
                cout << message << std::endl;
                break;
            case DEBUG:
                if(debug_mode)
                    cout << "[DEBUG] " << message << std::endl;
                break;
            case WARNING:
                cout << "[WARNING] " << message << std::endl;
                break;
            case ERROR:
                cout << "[ERROR] " << message << std::endl;
                break;
    
        }
    }

    public: 
    
    static void logInfo(const string& message) {
        log(LogLevel::INFO, false, message);
    }

    static void logDebug(bool debug_mode, const string& message) {
        log(LogLevel::DEBUG, debug_mode, message);
    }

    static void logWarning(const string& message) {
        log(LogLevel::WARNING, false, message);
    }

    static void logError(const string& message) {
        log(LogLevel::ERROR, false, message);
    }

    
};