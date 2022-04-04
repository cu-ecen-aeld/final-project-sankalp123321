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

class threadClient : public threadBase, tcpClient
{
private:
    /* data */
public:
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes){};
    virtual void PeriodicFunction(){};
    virtual void Notification(uint8_t notifId){};
    threadClient(uint32_t threadID, std::string ipAddr, std::string socketID);
    ~threadClient();
};
