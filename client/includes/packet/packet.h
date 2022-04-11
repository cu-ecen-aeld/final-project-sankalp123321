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
class packet
{
private:
    #pragma pack(1)
    struct CommsPacket
    {
        uint16_t m_destThreadID;
        uint16_t m_srcThreadID;
        uint8_t m_payLoadSize;
        uint8_t m_payload[254];
        uint8_t m_cksum;
    };
    CommsPacket datagram;
public:
    void Serialize(uint8_t* payload, uint8_t payloadQty);
    packet(uint32_t destThrdID, uint32_t srcThrdID);
    static void SendMessage(packet &pkt);
    static void SendMessage(packet &pkt, uint32_t commdThread);
    ~packet();
};
