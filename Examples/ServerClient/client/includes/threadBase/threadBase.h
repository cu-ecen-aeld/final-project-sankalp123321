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
#include <ackMsg/ackMsg.h>
#include <packet/packet.h>

typedef std::function<int(packet*)> msgHandle;
typedef std::function<int(ackMsg*)> ackMsgHandle;
class threadBase
{
private:
public:
    /**
     * @brief 
     * 
     */
    std::map<uint16_t, msgHandle> methodHandler;

    /**
     * @brief 
     * 
     */
    std::map<uint16_t, ackMsgHandle> ackMethodHandler;

    /**
     * @brief 
     * 
     * @tparam T 
     * @param fp 
     * @param obj 
     * @return msgHandle 
     */
    template<typename T>
    msgHandle CastHadnler(int(T::*fp)(packet*), T *obj)
    {
        using namespace std::placeholders;
        return std::bind(fp, obj, _1);
    }

    /**
     * @brief 
     * 
     * @tparam T 
     * @param fp 
     * @param obj 
     * @return ackMsgHandle 
     */
    template<typename T>
    ackMsgHandle CastHadnler(int(T::*fp)(ackMsg*), T *obj)
    {
        using namespace std::placeholders;
        return std::bind(fp, obj, _1);
    }

    /**
     * @brief 
     * 
     * @param msgID 
     */
    void RegisterMethods(uint16_t msgID, msgHandle);

    /**
     * @brief Get the Method Based On Msg I D object
     * 
     * @param msgID 
     * @return std::function<int(packet*)> 
     */
    msgHandle GetMethodBasedOnMsgID (uint16_t msgID);

    /**
     * @brief 
     * 
     * @param msgID 
     */
    void RegisterMethods(uint16_t msgID, ackMsgHandle);

    /**
     * @brief Get the Method Based On Msg I D object
     * 
     * @param msgID 
     * @return std::function<int(packet*)> 
     */
    ackMsgHandle GetAckMethodBasedOnMsgID (uint16_t msgID);

    /**
     * @brief 
     * 
     * @param buffer 
     * @param numOfBytes 
     */
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes){};

    /**
     * @brief 
     * 
     */
    virtual void PeriodicFunction();

    /**
     * @brief 
     * 
     * @param notifId 
     */
    virtual void Notification(uint8_t notifId);

    /**
     * @brief 
     * 
     * @param data 
     * @param numOfBytes 
     */
    virtual void AddToTxBuffer(uint8_t* data, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param data 
     * @param numOfBytes 
     * @return uint16_t 
     */
    virtual uint16_t PopRxBuffer(uint8_t* data, uint16_t numOfBytes);
    
    /**
     * @brief 
     * 
     * @param data 
     * @param numOfBytes 
     */
    void packetDeserializer(uint8_t* data, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param data 
     * @param numOfBytes 
     */
    void ackPacketDeserializer(uint8_t* data, uint16_t numOfBytes);

    /**
     * @brief Construct a new thread Base object
     * 
     * @param threadID 
     */
    threadBase(uint32_t threadID);

    /**
     * @brief Destroy the thread Base object
     * 
     */
    ~threadBase();
};