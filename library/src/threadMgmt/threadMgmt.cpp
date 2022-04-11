/**
 * @file threadMgmt.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <threadMgmt.h>
#include "buffer/buffer.h"
#include "packet/packet.h"

threadMgmt *threadMgmt::m_threadMgmtInst = nullptr;

threadMgmt::threadMgmt() :
    m_rTbl(nullptr)
{
    m_rTbl = routingTbl::GetRoutingTableInst();
}

threadMgmt::~threadMgmt()
{
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

void threadMgmt::packetProcessor()
{
    buffer* m_bufferInst = buffer::GetBufferInst();
    uint8_t packetFinderState = FIND_HDR;
    uint16_t cntr = 0;
    packet foundPacket (0, 0);
    uint16_t payloadSize = 0, payloadSizeCntr = 0;
    uint8_t cksum = 0;
    uint8_t buffer[TOTAL_PACKET_SIZE_MAX];
    while (1)
    {
        // signal wake up
        uint16_t actualBytesRead = m_bufferInst->PopFromExternalBuffer(buffer, TOTAL_PACKET_SIZE_MAX);

        while (cntr <= actualBytesRead)
        {
            switch (packetFinderState)
            {
                case FIND_HDR:
                {
                    uint32_t sync = (((uint32_t)buffer[cntr ] << 24) | ((uint32_t)buffer[cntr + 1] << 16) | \
                                ((uint32_t)buffer[cntr + 2] << 8) | ((uint32_t)buffer[cntr + 3]));
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
                    uint16_t destAddr = ((uint16_t)buffer[cntr] << 8) | ((uint16_t)buffer[cntr + 1]);
                    foundPacket.datagram.m_destThreadID = destAddr;
                    cntr += sizeof(foundPacket.datagram.m_destThreadID);
                    packetFinderState++;
                }
                break;
                case FIND_SRC_ADDR:
                {
                    uint16_t srcAddr = ((uint16_t)buffer[cntr] << 8) | ((uint16_t)buffer[cntr + 1]);
                    foundPacket.datagram.m_srcThreadID = srcAddr;
                    cntr += sizeof(foundPacket.datagram.m_srcThreadID);
                    packetFinderState++;
                }
                break;
                case FIND_PAYLOAD_SIZE:
                {
                    foundPacket.datagram.m_payLoadSize = buffer[cntr];
                    payloadSize = buffer[cntr];
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
                        foundPacket.datagram.m_payload[payloadSizeCntr++] = buffer[cntr++];
                    }
                    payloadSize--;
                }
                break;
                case FIND_CKSUM:
                {
                    foundPacket.datagram.m_cksum = buffer[cntr++];
                    for (int i = 4; i < (0 + (PACKET_HEADER_SIZE + foundPacket.datagram.m_payLoadSize + CKSUM_SIZE)); i++)
                    {
                        printf("%02X ", buffer[i]);
                        cksum ^= buffer[i];
                    }
                    printf("\n");
                    if(!cksum)
                    {
                        printf("Correct checksum.\n");
                    }
                    else
                    {
                        printf("Invalid checksum: %02X.\n", cksum);
                    }
                    return;
                }
                break;
                default:
                    break;
            }
            // printf("Byte[0x%02X] stage[0x%02X] cntr[%d]\n", buffer[cntr], packetFinderState, cntr);
        }
    }
}


void threadMgmt::managerThread()
{
    packetProcessor();
}

threadMgmt *threadMgmt::OverWatch()
{ 
    if(m_threadMgmtInst == nullptr)
    {
        m_threadMgmtInst = new threadMgmt;
    }

    return m_threadMgmtInst;
}