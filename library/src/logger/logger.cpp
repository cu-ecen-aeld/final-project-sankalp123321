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
    if(logger->mloggerlevel == lvl)
    {
        switch(lvl)
        {
            case LEVEL_FATAL:
                loglevel = "[FATAL] ";
            break;
            case LEVEL_ERROR:
                loglevel = "[ERROR] ";
            break;
            case LEVEL_WARN:
                loglevel = "[WARN] ";
            break;
            case LEVEL_INFO:
                loglevel = "[INFO] ";
            break;
            case LEVEL_DEBUG:
                loglevel = "[DEBUG] ";
            break;
            case LEVEL_TRACE:
                loglevel = "[TRACE] ";
            break;
            case LEVEL_NONE:
            default:
                loglevel = "[NONE] ";
            break;
        };
        va_start(args, logFormat);
        printf("%s%s ", loglevel.c_str(), __TIME__);
        vprintf(logFormat.c_str(), args);
        va_end(args);
    }
}