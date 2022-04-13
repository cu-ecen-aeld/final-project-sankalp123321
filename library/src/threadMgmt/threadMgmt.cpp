/**
 * @file threadMgmt.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <thread>
#include "threadMgmt/threadMgmt.h"
#include "buffer/buffer.h"
#include "packet/packet.h"

threadMgmt *threadMgmt::m_threadMgmtInst = nullptr;

threadMgmt::threadMgmt() :
    m_rTbl(nullptr)
{
    m_rTbl = routingTbl::GetRoutingTableInst();
}

threadMgmt::~threadMgmt()
{
}

void threadMgmt::messageRouter()
{
    buffer* m_bufferInst = buffer::GetBufferInst();
    uint8_t buffer[TOTAL_PACKET_SIZE_MAX];
    while (1)
    {
        uint16_t actualBytesRead = m_bufferInst->PopFromExternalTxBuffer(buffer, TOTAL_PACKET_SIZE_MAX);
    }
}

void threadMgmt::managerThread()
{
    std::thread messageRoutingThread(messageRouter);
    messageRoutingThread.join();
}

threadMgmt *threadMgmt::OverWatch()
{ 
    if(m_threadMgmtInst == nullptr)
    {
        m_threadMgmtInst = new threadMgmt;
    }

    return m_threadMgmtInst;
}