/**
 * @file threadClient.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "threadClient.h"
#include "router/routingTbl.h"

threadClient::threadClient(uint32_t threadID, std::string ipAddr, std::string socketID):
    threadBase(threadID), tcpClient(ipAddr, socketID)
{
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);
}

threadClient::~threadClient()
{
}