/**
 * @file threadBase.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "threadBase/threadBase.h"
#include <logger/logger.h>
threadBase::threadBase(uint32_t threadID)
{
}

threadBase::~threadBase()
{
}

void threadBase::RegisterMethods(uint16_t msgID, std::function<int(packet*)> fp)
{
    CPPLogger *cpplogger = CPPLogger::getLoggerInst();
    methodHandler.emplace(msgID, fp);
}

std::function<int(packet*)> threadBase::GetMethodBasedOnMsgID (uint16_t msgID)
{
    return methodHandler.find(msgID)->second;
}
void threadBase::PeriodicFunction(){}
void threadBase::Notification(uint8_t notifId){}
void threadBase::AddToTxBuffer(uint8_t* data, uint16_t numOfBytes){}
uint16_t threadBase::PopRxBuffer(uint8_t* data, uint16_t numOfBytes){}