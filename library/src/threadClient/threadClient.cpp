/**
 * @file threadClient.cpp
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2022-03-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <thread>
#include "msgBase/baseMsg.h"
#include "ackMsg/ackMsg.h"
#include "packet/packet.h"
#include "threadClient.h"
#include "router/routingTbl.h"
#include "buffer/buffer.h"

threadClient::threadClient(uint32_t threadID, std::string ipAddr, std::string socketID):
    threadBase(threadID), tcpClient(ipAddr, socketID), m_ThreadID(threadID), cpplogger(nullptr)
{
    cpplogger = CPPLogger::getLoggerInst();
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);

    packetProcessorThread = new std::thread(packetProcessor, this);
}

threadClient::~threadClient()
{
    packetProcessorThread->join();

    if(packetProcessorThread != nullptr)
    {
        delete packetProcessorThread;
    }
}

void threadClient::packetProcessor(threadClient* inst)
{
    uint16_t cntr = 0;
    uint8_t bufferBytes[TOTAL_PACKET_SIZE_MAX];
    uint16_t actualBytesRead = 0;
    while (1)
    {
        // signal wake up
        actualBytesRead = inst->PopRxBuffer(bufferBytes, TOTAL_PACKET_SIZE_MAX);

        if(!actualBytesRead)
        {
            continue;
        }

        while ((cntr + 3) <= actualBytesRead)
        {
            uint32_t sync = (((uint32_t)bufferBytes[cntr] << 24) | ((uint32_t)bufferBytes[cntr + 1] << 16) | \
                                ((uint32_t)bufferBytes[cntr + 2] << 8) | ((uint32_t)bufferBytes[cntr + 3]));
            // logger_log(inst->cpplogger, LEVEL_DEBUG, "hdr: %08X\n", sync);
            if(sync == HEADER_ID_INV)
            {
                inst->packetDeserializer(bufferBytes, actualBytesRead);
                break;
            }
            else if(sync == ACK_HEADER_ID_INV)
            {
                inst->ackPacketDeserializer(bufferBytes, actualBytesRead);
                break;
            }
            cntr++;
        }
    }
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
    // logger_log(cpplogger, LEVEL_DEBUG, "AddToTxBuffer Thread ID: 0x%X\n", m_ThreadID);
    tcpClient::AddToExternalTxBuffer(data, numOfBytes);
}

uint16_t threadClient::PopRxBuffer(uint8_t* data, uint16_t numOfBytes)
{
    // logger_log(cpplogger, LEVEL_DEBUG, "PopRxBuffer Thread ID: 0x%X\n", m_ThreadID);
    return tcpClient::PopFromExternalRxBuffer(data, numOfBytes);
}