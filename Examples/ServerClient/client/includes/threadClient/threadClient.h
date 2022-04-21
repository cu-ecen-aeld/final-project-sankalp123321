/**
 * @file threadClient.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <string>
#include "threadBase/threadBase.h"
#include "socket/socket.h"
#include "logger/logger.h"

class threadClient : public threadBase, tcpClient
{
private:
    /* data */
    static void packetProcessor(threadClient* inst);
    uint32_t m_ThreadID;
    std::thread* packetProcessorThread;
    CPPLogger* cpplogger;
protected:
    
public:
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes);
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    virtual void AddToTxBuffer(uint8_t* data, uint16_t numOfBytes);
    virtual uint16_t PopRxBuffer(uint8_t* data, uint16_t numOfBytes);
    threadClient(uint32_t threadID, std::string ipAddr, std::string socketID);
    virtual ~threadClient();
};
