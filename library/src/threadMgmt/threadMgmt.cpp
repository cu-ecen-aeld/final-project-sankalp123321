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
    messageRoutingThread->join();
    delete messageRoutingThread;
}

void threadMgmt::messageRouter()
{
    buffer* m_bufferInst = buffer::GetBufferInst();
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    CPPLogger* cpplog = CPPLogger::getLoggerInst();
    while (1)
    {
        packet classToBeCalled;
        uint8_t retVal = m_bufferInst->PopFromInternalBuffer(&classToBeCalled);
        if(!retVal)
        {
            continue;
        }
         
        threadBase* tBase = rTbl->GetThreadInstanceFromID(classToBeCalled.datagram.m_destThreadID);
        if(tBase != nullptr)
        {
            logger_log(cpplog, LEVEL_DEBUG, "Calling thread with ID: %x, ptr: %p \n", classToBeCalled.datagram.m_destThreadID, tBase);
            // tBase->RecvMessageAsync(classToBeCalled.datagram.m_payload, classToBeCalled.datagram.m_payLoadSize);
            tBase->GetMethodBasedOnMsgID(classToBeCalled.datagram.m_msgID)(&classToBeCalled);
        }
        else
        {
            logger_log(cpplog, LEVEL_DEBUG, "No thread found with ID: %x\n", classToBeCalled.datagram.m_destThreadID);
        }
    }
}

void threadMgmt::SendAckMessage(ackMsg &pkt)
{

}
void threadMgmt::SendAckMessage(ackMsg &pkt, uint32_t commdThread)
{

}

void threadMgmt::managerThread()
{
    messageRoutingThread = new std::thread(messageRouter);
}

threadMgmt *threadMgmt::OverWatch()
{ 
    if(m_threadMgmtInst == nullptr)
    {
        m_threadMgmtInst = new threadMgmt;
    }

    return m_threadMgmtInst;
}