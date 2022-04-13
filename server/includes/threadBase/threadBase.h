/**
 * @file threadBase.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <iostream>

class threadBase
{
private:
    /* data */
public:
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes);
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    virtual void AddToTxBuffer(uint8_t* data, uint16_t numOfBytes);
    virtual uint16_t PopRxBuffer(uint8_t* data, uint16_t numOfBytes);
    threadBase(uint32_t threadID);
    ~threadBase();
};