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
#include <functional>
#include <map>
#include <packet/packet.h>

typedef std::function<int(packet*)> msgHandle;

class threadBase
{
private:
    /* data */
    // std::array<void(*)(packet*), 10> actions{};
public:
    std::map<uint16_t, std::function<int(packet*)>> methodHandler;
    template<typename T>
    msgHandle CastHadnler(int(T::*fp)(packet*), T *obj)
    {
        using namespace std::placeholders;
        return std::bind(fp, obj, _1);
    }
    void RegisterMethods(uint16_t msgID, std::function<int(packet*)>);
    // int (*GetMethodBasedOnMsgID (uint16_t msgID))(packet*);
    std::function<int(packet*)> GetMethodBasedOnMsgID (uint16_t msgID);
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes){};
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    virtual void AddToTxBuffer(uint8_t* data, uint16_t numOfBytes);
    virtual uint16_t PopRxBuffer(uint8_t* data, uint16_t numOfBytes);
    threadBase(uint32_t threadID);
    ~threadBase();
};