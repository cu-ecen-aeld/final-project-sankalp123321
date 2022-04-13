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

threadBase::threadBase(uint32_t threadID)
{
    
}

threadBase::~threadBase()
{
}

void threadBase::RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes){}
void threadBase::PeriodicFunction(){}
void threadBase::Notification(uint8_t notifId){}
void threadBase::AddToTxBuffer(uint8_t* data, uint16_t numOfBytes){}
uint16_t threadBase::PopRxBuffer(uint8_t* data, uint16_t numOfBytes){}