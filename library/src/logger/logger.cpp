#include <ctime>   
#include <string.h>
#include <stdarg.h>
#include "logger.h"

CPPLogger* CPPLogger::cpplogger = nullptr;
std::mutex CPPLogger::mutex_;

CPPLogger::CPPLogger()
{
    
}

CPPLogger::~CPPLogger()
{
    if (cpplogger != nullptr)
    {
        delete cpplogger;
    }
    
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void logger_setLevel(CPPLogger *logger, Level_Level lvl)
{
    if((lvl > LEVEL_TRACE) && (lvl < LEVEL_FATAL))
    {
        logger->mloggerlevel = LEVEL_INFO;
    }
    logger->mloggerlevel = lvl;
}
void logger_log(CPPLogger *logger, Level_Level lvl, std::string logFormat, ...)
{
    va_list args;
    std::string loglevel;
    if(lvl <= logger->mloggerlevel)
    {
        switch(lvl)
        {
            case LEVEL_FATAL:
                loglevel = "[FATAL]";
            break;
            case LEVEL_ERROR:
                loglevel = "[ERROR]";
            break;
            case LEVEL_WARN:
                loglevel = "[WARN]";
            break;
            case LEVEL_INFO:
                loglevel = "[INFO]";
            break;
            case LEVEL_DEBUG:
                loglevel = "[DEBUG]";
            break;
            case LEVEL_TRACE:
                loglevel = "[TRACE]";
            break;
            case LEVEL_NONE:
            default:
                loglevel = "[NONE]";
            break;
        };
        va_start(args, logFormat);
        printf("%7s %s ", loglevel.c_str(), currentDateTime().c_str());
        vprintf(logFormat.c_str(), args);
        va_end(args);
    }
}