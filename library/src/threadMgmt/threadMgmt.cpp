/**
 * @file threadMgmt.cpp
 * @author Sankalp Agrawal (saag2511@colorado.edu)
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

uint16_t threadMgmt::pktIDCntr = 0;
threadMgmt *threadMgmt::m_threadMgmtInst = nullptr;

threadMgmt::threadMgmt() :
    m_rTbl(nullptr)
{
    m_rTbl = routingTbl::GetRoutingTableInst();
}

threadMgmt::~threadMgmt()
{
    messageRoutingThread->join();

    if(messageRoutingThread != nullptr)
    {
        delete messageRoutingThread;
    }

    if(m_threadMgmtInst != nullptr)
    {
        delete m_threadMgmtInst;
    }
}

void threadMgmt::SendMessage(packet &pkt)
{
    buffer* m_bufferInst = buffer::GetBufferInst();
    if(m_bufferInst != nullptr)
    {
        m_bufferInst->AddToInternalBuffer(pkt);
    }
}

void threadMgmt::SendMessage(packet &pkt, uint32_t commThread)
{
    routingTbl* m_routerInst =  routingTbl::GetRoutingTableInst();
    uint8_t *buffptr = (uint8_t*)&pkt.datagram;
    CPPLogger* cpplog = CPPLogger::getLoggerInst();
    pkt.datagram.m_pktID = pktIDCntr++;
    pkt.datagram.m_cksum ^= (pktIDCntr >> 8) & 0xFF;
    pkt.datagram.m_cksum ^= pktIDCntr & 0xFF;
    logger_log(cpplog, LEVEL_INFO, "Header[%04X] SrcAddr[0x%04X] DestAddr[0x%04X] PayloadSize[%x] msgID[%x]  Data[ ", pkt.datagram.m_header, pkt.datagram.m_srcThreadID, \
    pkt.datagram.m_destThreadID, pkt.datagram.m_payLoadSize, pkt.datagram.m_msgID);   
    for (uint16_t i = 0; i < pkt.datagram.m_payLoadSize; i++)
    {
        printf("0x%02X ", buffptr[i + PACKET_HEADER_SIZE]);
    }
    printf("] cksum[%02X] \r\n", pkt.datagram.m_cksum);

    logger_log(cpplog, LEVEL_INFO, "SendMessage %p\n", m_routerInst->GetThreadInstanceFromID(commThread));
    threadBase* baseThreadPtr = m_routerInst->GetThreadInstanceFromID(commThread);
    if(baseThreadPtr == nullptr)
    {
        logger_log(cpplog, LEVEL_INFO, "No Registered thread 0x%X\n", commThread);
    }
    else
    {
        baseThreadPtr->AddToTxBuffer(buffptr, PACKET_HEADER_SIZE + CKSUM_SIZE + pkt.datagram.m_payLoadSize);
    }
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
        if(retVal)
        {
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

        ackMsg ackMsgHandler;
        retVal = m_bufferInst->PopFromInternalBuffer(&ackMsgHandler);
        if(retVal)
        {
            threadBase* tBase = rTbl->GetThreadInstanceFromID(ackMsgHandler.ackBackMsg.m_destThreadID);
            if(tBase != nullptr)
            {
                logger_log(cpplog, LEVEL_DEBUG, "Calling thread with ID: %x, ptr: %p \n", ackMsgHandler.ackBackMsg.m_destThreadID, tBase);
                // tBase->RecvMessageAsync(classToBeCalled.datagram.m_payload, classToBeCalled.datagram.m_payLoadSize);
                tBase->GetAckMethodBasedOnMsgID(ackMsgHandler.ackBackMsg.m_msgID)(&ackMsgHandler);
            }
            else
            {
                logger_log(cpplog, LEVEL_DEBUG, "No thread found with ID: %x\n", ackMsgHandler.ackBackMsg.m_destThreadID);
            }
        }
    }
}

void threadMgmt::SendAckMessage(ackMsg &pkt)
{

}
void threadMgmt::SendAckMessage(ackMsg &pkt, uint32_t commThread)
{
    routingTbl* m_routerInst =  routingTbl::GetRoutingTableInst();
    uint8_t *buffptr = (uint8_t*)&pkt.ackBackMsg;
    CPPLogger* cpplog = CPPLogger::getLoggerInst();
    pkt.ackBackMsg.m_pktID = pktIDCntr++;
    pkt.ackBackMsg.m_cksum ^= (pktIDCntr >> 8) & 0xFF;
    pkt.ackBackMsg.m_cksum ^= pktIDCntr & 0xFF;
    pkt.calCulateChecksum();

    logger_log(cpplog, LEVEL_DEBUG, "[");
    for (int i = 4; i < ACK_TOTAL_PACKET_SIZE_MAX; i++)
    {
        printf("%02X ", buffptr[i]);
    }
    printf("]\n");

    logger_log(cpplog, LEVEL_INFO, "Header[%04X] SrcAddr[0x%04X] DestAddr[0x%04X] isAck[%x] msgID[%x] errorcode[%d] cksum[%02X] \r\n", pkt.ackBackMsg.m_header, pkt.ackBackMsg.m_srcThreadID, \
    pkt.ackBackMsg.m_destThreadID, pkt.ackBackMsg.m_isAck, pkt.ackBackMsg.m_msgID, pkt.ackBackMsg.m_errorCode, pkt.ackBackMsg.m_cksum);   

    threadBase* baseThreadPtr = m_routerInst->GetThreadInstanceFromID(commThread);
    if(baseThreadPtr == nullptr)
    {
        logger_log(cpplog, LEVEL_INFO, "No Registered thread 0x%X\n", commThread);
    }
    else
    {
        baseThreadPtr->AddToTxBuffer(buffptr, ACK_TOTAL_PACKET_SIZE_MAX);
    }
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