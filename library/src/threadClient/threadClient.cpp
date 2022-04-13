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
    m_ThreadID(threadID), threadBase(threadID), tcpClient(ipAddr, socketID)
{
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);
}

threadClient::~threadClient()
{
}

void threadClient::RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes)
{

}
void threadClient::PeriodicFunction()
{

}
void threadClient::Notification(uint8_t notifId)
{

}

void threadClient::AddToTxBuffer(uint8_t* data, uint16_t numOfBytes)
{
    printf("Thread ID: 0x%X\n", m_ThreadID);
    tcpClient::AddToExternalTxBuffer(data, numOfBytes);
}