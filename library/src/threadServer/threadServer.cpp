/**
 * @file threadServer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "threadServer.h"
#include "threadBase/threadBase.h"
#include "router/routingTbl.h"

threadServer::threadServer(uint32_t threadID, std::string ipAddr, std::string socketID):
    m_ThreadID(threadID), threadBase(threadID), tcpServer(ipAddr, socketID)
{
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);
}

threadServer::~threadServer()
{

}

void threadServer::RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes)
{

}
void threadServer::PeriodicFunction()
{

}
void threadServer::Notification(uint8_t notifId)
{

}

void threadServer::AddToTxBuffer(uint8_t* data, uint16_t numOfBytes)
{
    printf("Thread ID: 0x%X\n", m_ThreadID);
    tcpServer::AddToExternalTxBuffer(data, numOfBytes);
}