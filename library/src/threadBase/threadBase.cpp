/**
 * @file threadBase.cpp
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "threadBase/threadBase.h"
#include <logger/logger.h>
#include "buffer/buffer.h"
#include "ackMsg/ackMsg.h"

enum
{
    FIND_HDR = 0,
    FIND_PKT_ID,
    FIND_DEST_ADDR,
    FIND_SRC_ADDR,
    FIND_MSG_ID,
    FIND_PAYLOAD_SIZE,
    FIND_PAYLOAD,
    FIND_CKSUM,
};


enum
{
    FIND_ACK_HDR = 0,
    FIND_ACK_PKT_ID,
    FIND_ACK_DEST_ADDR,
    FIND_ACK_SRC_ADDR,
    FIND_ACK_MSG_ID,
    FIND_ACK_IS_ACK,
    FIND_ACK_ERROR_CODE,
    FIND_ACK_CKSUM,
};


threadBase::threadBase(uint32_t threadID)
{
}

threadBase::~threadBase()
{
}

void threadBase::RegisterMethods(uint16_t msgID, msgHandle fp)
{
    methodHandler.emplace(msgID, fp);
}

msgHandle threadBase::GetMethodBasedOnMsgID (uint16_t msgID)
{
    return methodHandler.find(msgID)->second;
}

void threadBase::RegisterMethods(uint16_t msgID, ackMsgHandle fp)
{
    ackMethodHandler.emplace(msgID, fp);
}

ackMsgHandle threadBase::GetAckMethodBasedOnMsgID (uint16_t msgID)
{
    return ackMethodHandler.find(msgID)->second;
}

void threadBase::PeriodicFunction(){}
void threadBase::Notification(uint8_t notifId){}
void threadBase::AddToTxBuffer(uint8_t* data, uint16_t numOfBytes){}
uint16_t threadBase::PopRxBuffer(uint8_t* data, uint16_t numOfBytes){ return 0; }

void threadBase::packetDeserializer(uint8_t* bufferBytes, uint16_t numOfBytes)
{
    uint8_t packetFinderState = FIND_HDR;
    uint16_t cntr = 0;
    CPPLogger *cpplogger = CPPLogger::getLoggerInst();
    // packet *foundPacket;
    packet foundPacket(0,0,0);
    uint16_t payloadSize = 0, payloadSizeCntr = 0;
    uint8_t cksum = 0;
    uint16_t actualBytesRead = numOfBytes;
    buffer* buffInst = buffer::GetBufferInst();

    while (cntr <= actualBytesRead)
    {
        switch (packetFinderState)
        {
            case FIND_HDR:
            {
                uint32_t sync = (((uint32_t)bufferBytes[cntr ] << 24) | ((uint32_t)bufferBytes[cntr + 1] << 16) | \
                            ((uint32_t)bufferBytes[cntr + 2] << 8) | ((uint32_t)bufferBytes[cntr + 3]));
                // logger_log(cpplogger, LEVEL_DEBUG, "hdr: %08X\n", sync);
                if(sync == HEADER_ID_INV)
                {
                    packetFinderState++;
                    foundPacket.datagram.m_header = HEADER_ID;
                    foundPacket.setIsMessagePacket(true);
                }
                cntr += HEADER_ID_SIZE;
            }
            break;
            case FIND_PKT_ID:
            {
                foundPacket.datagram.m_pktID = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                cntr += sizeof(foundPacket.datagram.m_pktID);
                packetFinderState++;
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
            case FIND_MSG_ID:
            {
                foundPacket.datagram.m_msgID = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                logger_log(cpplogger, LEVEL_DEBUG, "msgID: 0x%X\n",  foundPacket.datagram.m_msgID);
                cntr += sizeof(foundPacket.datagram.m_msgID);
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
                    // logger_log(cpplogger, LEVEL_DEBUG, "%02X ", bufferBytes[i]);
                    cksum ^= bufferBytes[i];
                }
                logger_log(cpplogger, LEVEL_DEBUG, "\n");
                if(!cksum)
                {
                    logger_log(cpplogger, LEVEL_DEBUG, "Correct checksum.\n");
                    buffInst->AddToInternalBuffer(foundPacket);
                }
                else
                {
                    logger_log(cpplogger, LEVEL_DEBUG, "Invalid checksum: %02X.\n", cksum);
                }
                packetFinderState = FIND_HDR;
                break;
            }
            break;
            default:
                break;
        }
        // logger_log(cpplogger, LEVEL_DEBUG, "Byte[0x%02X] stage[0x%02X] cntr[%d]\n", bufferBytes[cntr], packetFinderState, cntr);
    }
    cntr = 0;
}

void threadBase::ackPacketDeserializer(uint8_t* bufferBytes, uint16_t numOfBytes)
{
    uint8_t packetFinderState = FIND_HDR;
    uint16_t cntr = 0;
    CPPLogger *cpplogger = CPPLogger::getLoggerInst();
    // packet *foundPacket;
    ackMsg foundPacket;
    uint8_t cksum = 0;
    uint16_t actualBytesRead = numOfBytes;
    buffer* buffInst = buffer::GetBufferInst();

    while (cntr <= actualBytesRead)
    {
        switch (packetFinderState)
        {
            case FIND_ACK_HDR:
            {
                uint32_t sync = (((uint32_t)bufferBytes[cntr ] << 24) | ((uint32_t)bufferBytes[cntr + 1] << 16) | \
                            ((uint32_t)bufferBytes[cntr + 2] << 8) | ((uint32_t)bufferBytes[cntr + 3]));
                logger_log(cpplogger, LEVEL_DEBUG, "hdr: %08X\n", sync);
                if(sync == ACK_HEADER_ID_INV)
                {
                    packetFinderState++;
                    foundPacket.ackBackMsg.m_header = ACK_HEADER_ID;
                    foundPacket.setIsMessagePacket(false);
                }
                cntr += ACK_HEADER_ID_SIZE;
            }
            break;
            case FIND_ACK_PKT_ID:
            {
                foundPacket.ackBackMsg.m_pktID = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                cntr += sizeof(foundPacket.ackBackMsg.m_pktID);
                packetFinderState++;
            }
            break;
            case FIND_ACK_DEST_ADDR:
            {
                uint16_t destAddr = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                foundPacket.ackBackMsg.m_destThreadID = destAddr;
                cntr += sizeof(foundPacket.ackBackMsg.m_destThreadID);
                packetFinderState++;
            }
            break;
            case FIND_ACK_SRC_ADDR:
            {
                uint16_t srcAddr = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                foundPacket.ackBackMsg.m_srcThreadID = srcAddr;
                cntr += sizeof(foundPacket.ackBackMsg.m_srcThreadID);
                packetFinderState++;
            }
            break;
            case FIND_ACK_MSG_ID:
            {
                foundPacket.ackBackMsg.m_msgID = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                logger_log(cpplogger, LEVEL_DEBUG, "Ack msgID: 0x%X\n",  foundPacket.ackBackMsg.m_msgID);
                cntr += sizeof(foundPacket.ackBackMsg.m_msgID);
                packetFinderState++;
            }
            break;
            case FIND_ACK_IS_ACK:
            {
                foundPacket.ackBackMsg.m_isAck = bufferBytes[cntr];
                cntr += sizeof(foundPacket.ackBackMsg.m_isAck);
                packetFinderState++;
            }
            break;
            case FIND_ACK_ERROR_CODE:
            {
                foundPacket.ackBackMsg.m_errorCode = ((uint16_t)bufferBytes[cntr + 1] << 8) | ((uint16_t)bufferBytes[cntr]);
                cntr += sizeof(foundPacket.ackBackMsg.m_errorCode);
                packetFinderState++;
            }
            break;
            case FIND_ACK_CKSUM:
            {
                foundPacket.ackBackMsg.m_cksum = bufferBytes[cntr++];
                // logger_log(cpplogger, LEVEL_DEBUG, "[");
                // for (int i = 4; i < ACK_TOTAL_PACKET_SIZE_MAX; i++)
                // {
                //     printf("%02X ", bufferBytes[i]);
                //     cksum ^= bufferBytes[i];
                // }
                // printf("]\n");
                if(!cksum)
                {
                    logger_log(cpplogger, LEVEL_DEBUG, "Ack Correct checksum.\n");
                    buffInst->AddToInternalBuffer(foundPacket);
                }
                else
                {
                    logger_log(cpplogger, LEVEL_DEBUG, "Ack Invalid checksum: %02X.\n", cksum);
                }
                packetFinderState = FIND_HDR;
                break;
            }
            break;
            default:
                break;
        }
        // logger_log(cpplogger, LEVEL_DEBUG, "Byte[0x%02X] stage[0x%02X] cntr[%d]\n", bufferBytes[cntr], packetFinderState, cntr);
    }
    cntr = 0;
}