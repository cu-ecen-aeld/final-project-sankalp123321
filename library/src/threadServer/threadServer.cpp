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
#include "packet/packet.h"
#include "buffer/buffer.h"

threadServer::threadServer(uint32_t threadID, std::string ipAddr, std::string socketID):
    m_ThreadID(threadID), cpplogger(nullptr), threadBase(threadID), tcpServer(ipAddr, socketID)
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
}

enum
{
    FIND_HDR = 0,
    FIND_DEST_ADDR,
    FIND_SRC_ADDR,
    FIND_PAYLOAD_SIZE,
    FIND_PAYLOAD,
    FIND_CKSUM,
};

void threadServer::packetProcessor(threadServer* inst)
{
    uint8_t packetFinderState = FIND_HDR;
    uint16_t cntr = 0;
    packet foundPacket (0, 0);
    uint16_t payloadSize = 0, payloadSizeCntr = 0;
    uint8_t cksum = 0;
    uint8_t bufferBytes[TOTAL_PACKET_SIZE_MAX];
    uint16_t actualBytesRead = 0;
    buffer* buffInst = buffer::GetBufferInst();
    while (1)
    {
        // signal wake up
        actualBytesRead = inst->PopRxBuffer(&bufferBytes[actualBytesRead], TOTAL_PACKET_SIZE_MAX);
        if(!actualBytesRead)
        {
            continue;
        }

        while (cntr <= actualBytesRead)
        {
            switch (packetFinderState)
            {
                case FIND_HDR:
                {
                    uint32_t sync = (((uint32_t)bufferBytes[cntr ] << 24) | ((uint32_t)bufferBytes[cntr + 1] << 16) | \
                                ((uint32_t)bufferBytes[cntr + 2] << 8) | ((uint32_t)bufferBytes[cntr + 3]));
                    if(sync == HEADER_ID_INV)
                    {
                        packetFinderState++;
                        foundPacket.datagram.m_header = HEADER_ID;
                    }
                    cntr += HEADER_ID_SIZE;
                }
                break;
                case FIND_DEST_ADDR:
                {
                    uint16_t destAddr = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                    foundPacket.datagram.m_destThreadID = destAddr;
                    cntr += sizeof(foundPacket.datagram.m_destThreadID);
                    packetFinderState++;
                }
                break;
                case FIND_SRC_ADDR:
                {
                    uint16_t srcAddr = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                    foundPacket.datagram.m_srcThreadID = srcAddr;
                    cntr += sizeof(foundPacket.datagram.m_srcThreadID);
                    packetFinderState++;
                }
                break;
                case FIND_PAYLOAD_SIZE:
                {
                    foundPacket.datagram.m_payLoadSize = bufferBytes[cntr];
                    payloadSize = bufferBytes[cntr];
                    cntr += sizeof(foundPacket.datagram.m_payLoadSize);
                    packetFinderState++;
                }
                break;
                case FIND_PAYLOAD:
                {
                    if(!payloadSize)
                    {
                        packetFinderState++;
                    }
                    else
                    {
                        foundPacket.datagram.m_payload[payloadSizeCntr++] = bufferBytes[cntr++];
                    }
                    payloadSize--;
                }
                break;
                case FIND_CKSUM:
                {
                    foundPacket.datagram.m_cksum = bufferBytes[cntr++];
                    for (int i = 4; i < (0 + (PACKET_HEADER_SIZE + foundPacket.datagram.m_payLoadSize + CKSUM_SIZE)); i++)
                    {
                        logger_log(inst->cpplogger, LEVEL_DEBUG, "%02X ", bufferBytes[i]);
                        cksum ^= bufferBytes[i];
                    }
                    logger_log(inst->cpplogger, LEVEL_DEBUG, "\n");
                    if(!cksum)
                    {
                        logger_log(inst->cpplogger, LEVEL_DEBUG, "Correct checksum.\n");
                    }
                    else
                    {
                        logger_log(inst->cpplogger, LEVEL_DEBUG, "Invalid checksum: %02X.\n", cksum);
                    }
                    packetFinderState = FIND_HDR;
                    break;
                }
                break;
                default:
                    break;
            }
            // logger_log(inst->cpplogger, LEVEL_DEBUG, "Byte[0x%02X] stage[0x%02X] cntr[%d]\n", bufferBytes[cntr], packetFinderState, cntr);
        }
        cntr = 0;
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