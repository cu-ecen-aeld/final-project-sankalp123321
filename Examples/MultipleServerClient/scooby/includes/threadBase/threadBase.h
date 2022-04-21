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
#include <map>
#include <packet/packet.h>

typedef int (*methodHandlerPtr)(packet*);

class threadBase
{
private:
    /* data */
    std::map<uint16_t, int (*)(packet*)> methodHandler;
public:
    void RegisterMethods(uint16_t msgID, int (*fp)(packet*));
    // int (*GetMethodBasedOnMsgID (uint16_t msgID))(packet*);
    methodHandlerPtr GetMethodBasedOnMsgID (uint16_t msgID);
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes){};
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    virtual void AddToTxBuffer(uint8_t* data, uint16_t numOfBytes);
    virtual uint16_t PopRxBuffer(uint8_t* data, uint16_t numOfBytes);
    threadBase(uint32_t threadID);
    ~threadBase();
};