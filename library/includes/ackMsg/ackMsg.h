/**
 * @file ackMsg.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <iostream>
#include "packet/packet.h"
#include "msgBase/baseMsg.h"

#define ACK_HEADER_ID (0x01C0FFEE)
#define ACK_HEADER_ID_INV (0xEEFFC001)
#define ACK_HEADER_ID_SIZE (4) 
#define ACK_PACKET_HEADER_SIZE (11)
#define ACK_CKSUM_SIZE (1)
#define ACK_TOTAL_PACKET_SIZE_MAX (ACK_PACKET_HEADER_SIZE + ACK_CKSUM_SIZE + ACK_HEADER_ID_SIZE)

class ackMsg : public baseMsg
{
private:
    #pragma pack(1)
    struct AckMsg
    {
        uint32_t m_header;
        uint16_t m_pktID;
        uint16_t m_destThreadID;
        uint16_t m_srcThreadID;
        uint16_t m_msgID;
        uint8_t m_isAck;
        uint16_t m_errorCode;
        uint8_t m_cksum;
    };

public:
    AckMsg ackBackMsg;
    void setPktID(uint16_t pktID) { ackBackMsg.m_pktID = pktID; }
    void setAckStatus(uint8_t isAck) { ackBackMsg.m_isAck = isAck; }
    void setErrorCode(uint16_t errorCode) { ackBackMsg.m_errorCode = errorCode; }

    void calCulateChecksum()
    {
        ackBackMsg.m_cksum = 0;
        uint8_t *buffptr = (uint8_t*)&ackBackMsg;
        for (int i = 4; i < (ACK_PACKET_HEADER_SIZE + ACK_HEADER_ID_SIZE - 1); i++)
        {
            ackBackMsg.m_cksum ^= buffptr[i];
        }
    }

    void formAckBackMessage(packet *incomingPacket)
    {
        ackBackMsg.m_header = ACK_HEADER_ID;
        ackBackMsg.m_msgID = incomingPacket->datagram.m_msgID;
        ackBackMsg.m_destThreadID = incomingPacket->datagram.m_destThreadID;
        ackBackMsg.m_srcThreadID = incomingPacket->datagram.m_srcThreadID;
    }

    ackMsg(/* args */){}
    ~ackMsg(){}
};