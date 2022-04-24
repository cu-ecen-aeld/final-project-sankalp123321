/**
 * @file packet.c
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cstring>
#include "threadBase/threadBase.h"
#include "packet/packet.h"
#include "buffer/buffer.h"
#include "router/routingTbl.h"

packet::packet(uint32_t destThrdID, uint32_t srcThrdID, uint16_t msgID) 
{
    memset(&datagram, 0, sizeof (CommsPacket));
    datagram.m_header = HEADER_ID;
    datagram.m_destThreadID = destThrdID;
    datagram.m_srcThreadID = srcThrdID;
    datagram.m_msgID = msgID;
    
    for (uint8_t i = 0; i < 2; i++)
    {
        datagram.m_cksum ^= (srcThrdID & 0xFF);
        datagram.m_cksum ^= (destThrdID & 0xFF);
        datagram.m_cksum ^= (msgID & 0xFF);
        
        srcThrdID = srcThrdID >> 8;
        destThrdID = destThrdID >> 8;
        msgID = msgID >> 8;
    }
    // logger_log(cpplogger, LEVEL_INFO, "size: %ld\r\n", sizeof(CommsPacket));
}

packet::packet()
{
}

packet::~packet()
{
}

void packet::Serialize(uint8_t* payload, uint8_t payloadQty)
{
    datagram.m_payLoadSize = payloadQty;
    if(payloadQty > 254)
    {
        // report error
    }
    memcpy(datagram.m_payload, payload, payloadQty);
    for (uint8_t i = 0; i < payloadQty; i++)
    {
        datagram.m_cksum ^= datagram.m_payload[i];
    }

    datagram.m_cksum ^= payloadQty;
    datagram.m_payload[payloadQty] = datagram.m_cksum;
}

void packet::setDestID(uint16_t destID)
{
    datagram.m_destThreadID = destID;
}
void packet::setSrcID(uint16_t srcID)
{
    datagram.m_srcThreadID = srcID;
}