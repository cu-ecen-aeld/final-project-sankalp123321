#pragma once

#include <iostream>
#include <mutex>
#include <string>

#define FDATE (char const[]){ __DATE__[7], __DATE__[8], ..., ' ', ... , '\0' }

typedef enum{
    LEVEL_NONE,
    LEVEL_FATAL,
    LEVEL_ERROR,
    LEVEL_WARN,
    LEVEL_INFO,
    LEVEL_DEBUG,
    LEVEL_TRACE
}Level_Level;
class CPPLogger
{
    public:
        friend void logger_log(CPPLogger *logger, Level_Level lvl, std::string logFormat, ...);
        friend void logger_setLevel(CPPLogger *logger, Level_Level lvl);
        static CPPLogger* getLoggerInst()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if(cpplogger == nullptr)
            {
                cpplogger = new CPPLogger();
            }
            return cpplogger;
        }
        Level_Level mloggerlevel;
    private:
        CPPLogger();
        ~CPPLogger();
        void logger_print(Level_Level level, char* logFormat, ...);

        static CPPLogger* cpplogger;
        
        static std::mutex mutex_;
};