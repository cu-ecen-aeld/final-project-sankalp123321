/**
 * @file threadServer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <string>
#include <deque>
#include "threadServer.h"
#include "threadBase/threadBase.h"
#include "socket/socket.h"
#include "logger/logger.h"

class threadServer : public threadBase, public tcpServer
{
private:
    /**
     * @brief 
     * 
     * @param inst 
     */
    static void packetProcessor(threadServer* inst);
    uint32_t m_ThreadID;
    std::thread* packetProcessorThread;
    CPPLogger* cpplogger;
public:
    /**
     * @brief 
     * 
     * @param buffer 
     * @param numOfBytes 
     */
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes);

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
     * @brief Construct a new thread Server object
     * 
     * @param threadID 
     * @param ipAddr 
     * @param socketID 
     */
    threadServer(uint32_t threadID, std::string ipAddr, std::string socketID);

    /**
     * @brief Destroy the thread Server object
     * 
     */
    virtual ~threadServer();
};
