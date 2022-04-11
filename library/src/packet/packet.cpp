/**
 * @file packet.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cstring>
#include "packet/packet.h"
#include "buffer/buffer.h"

void packet::SendMessage(packet &pkt)
{
    buffer* m_bufferInst = buffer::GetBufferInst();
    if(m_bufferInst != nullptr)
    {
        printf("it has memory\n");
        m_bufferInst->AddToInternalBuffer(pkt);
    }
}

void packet::SendMessage(packet &pkt, uint32_t commThread)
{
    buffer* m_bufferInst = buffer::GetBufferInst();
    uint8_t *buffptr = (uint8_t*)&pkt.datagram;
    printf("Header[%04X] SrcAddr[0x%04X] DestAddr[0x%04X] PayloadSize[%x] Data[ ", pkt.datagram.m_header, pkt.datagram.m_srcThreadID, \
    pkt.datagram.m_destThreadID, pkt.datagram.m_payLoadSize);   
    for (uint16_t i = 0; i < pkt.datagram.m_payLoadSize; i++)
    {
        printf("0x%02X ", buffptr[i + PACKET_HEADER_SIZE]);
    }
    printf("] cksum[%02X] \r\n", pkt.datagram.m_cksum);
    m_bufferInst->AddToExternalBuffer(buffptr, PACKET_HEADER_SIZE + CKSUM_SIZE + pkt.datagram.m_payLoadSize);
}

packet::packet(uint32_t destThrdID, uint32_t srcThrdID) 
{
    memset(&datagram, 0, sizeof (CommsPacket));
    datagram.m_header = HEADER_ID;
    datagram.m_destThreadID = destThrdID;
    datagram.m_srcThreadID = srcThrdID;

    for (uint8_t i = 0; i < 4; i++)
    {
        datagram.m_cksum ^= (srcThrdID & 0xFF);
        datagram.m_cksum ^= (destThrdID & 0xFF);
        
        srcThrdID = srcThrdID >> 8;
        destThrdID = destThrdID >> 8;
    }
    printf("size: %ld\r\n", sizeof(CommsPacket));
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