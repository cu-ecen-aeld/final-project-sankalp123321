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

#include "threadServer.h"
#include "threadBase/threadBase.h"
#include "socket/socket.h"

class threadServer : public threadBase, tcpServer
{
private:
public:
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes);
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    threadServer(uint32_t threadID, std::string ipAddr, std::string socketID);
    virtual ~threadServer();
};
