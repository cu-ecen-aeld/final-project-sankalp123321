/**
 * @file threadServer.cpp
 * @author Sankalp Agrawal (saag2511@colorado.edu)
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
#include "packet/packet.h"
#include "buffer/buffer.h"

threadServer::threadServer(uint32_t threadID, std::string ipAddr, std::string socketID):
    threadBase(threadID), tcpServer(ipAddr, socketID), m_ThreadID(threadID), cpplogger(nullptr) 
{
    cpplogger = CPPLogger::getLoggerInst();
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);

    packetProcessorThread = new std::thread(packetProcessor, this);
}

threadServer::~threadServer()
{
    packetProcessorThread->join();

    if(packetProcessorThread != nullptr)
    {
        delete packetProcessorThread;
    }
}

void threadServer::packetProcessor(threadServer* inst)
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

            uint32_t sync = (((uint32_t)bufferBytes[cntr ] << 24) | ((uint32_t)bufferBytes[cntr + 1] << 16) | \
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
    // printf("AddToTxBuffer Thread ID: 0x%X\n", m_ThreadID);
    tcpServer::AddToExternalTxBuffer(data, numOfBytes);
}

uint16_t threadServer::PopRxBuffer(uint8_t* data, uint16_t numOfBytes)
{
    // printf("PopRxBuffer Thread ID: 0x%X\n", m_ThreadID);
    return tcpServer::PopFromExternalRxBuffer(data, numOfBytes);
}