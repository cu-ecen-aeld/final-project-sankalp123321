/**
 * @file packet.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <iostream>
#include "logger/logger.h"

#define HEADER_ID (0xAAC0FFEE)
#define HEADER_ID_INV (0xEEFFC0AA)
#define HEADER_ID_SIZE (4) 
#define PACKET_HEADER_SIZE (5 + HEADER_ID_SIZE)
#define CKSUM_SIZE (1)
#define DATA_SIZE (254)
#define TOTAL_PACKET_SIZE_MAX (PACKET_HEADER_SIZE + DATA_SIZE + CKSUM_SIZE)


class packet
{
private:
    #pragma pack(1)
    struct CommsPacket
    {
        uint32_t m_header;
        uint16_t m_destThreadID;
        uint16_t m_srcThreadID;
        uint8_t m_payLoadSize;
        uint16_t m_msgID;
        uint8_t m_payload[254];
        uint8_t m_cksum;
    };
    
public:
    CommsPacket datagram;
    void Serialize(uint8_t* payload, uint8_t payloadQty);
    packet(uint32_t destThrdID, uint32_t srcThrdID, uint16_t msgID);
    static void SendMessage(packet &pkt);
    static void SendMessage(packet &pkt, uint32_t commdThread);
    void setDestID(uint16_t destID);
    void setSrcID(uint16_t srcID);
    packet();
    ~packet();
};
