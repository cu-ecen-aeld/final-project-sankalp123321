/**
 * @file packet.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
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
#include "msgBase/baseMsg.h"

#define HEADER_ID (0xAAC0FFEE)
#define HEADER_ID_INV (0xEEFFC0AA)
#define HEADER_ID_SIZE (4) 
#define PACKET_HEADER_SIZE (9 + HEADER_ID_SIZE)
#define CKSUM_SIZE (1)
#define DATA_SIZE (254)
#define TOTAL_PACKET_SIZE_MAX (PACKET_HEADER_SIZE + DATA_SIZE + CKSUM_SIZE)


class packet : public baseMsg
{
private:
    #pragma pack(1)
    struct CommsPacket
    {
        uint32_t m_header;
        uint16_t m_pktID;
        uint16_t m_destThreadID;
        uint16_t m_srcThreadID;
        uint16_t m_msgID;
        uint8_t m_payLoadSize;
        uint8_t m_payload[254];
        uint8_t m_cksum;
    };
    
public:
    CommsPacket datagram;
    /**
     * @brief 
     * 
     * @param payload 
     * @param payloadQty 
     */
    void Serialize(uint8_t* payload, uint8_t payloadQty);

    /**
     * @brief Construct a new packet object
     * 
     * @param destThrdID 
     * @param srcThrdID 
     * @param msgID 
     */
    packet(uint32_t destThrdID, uint32_t srcThrdID, uint16_t msgID);

    /**
     * @brief Set the Dest I D object
     * 
     * @param destID 
     */
    void setDestID(uint16_t destID);

    /**
     * @brief Set the Src I D object
     * 
     * @param srcID 
     */
    void setSrcID(uint16_t srcID);

    /**
     * @brief Construct a new packet object
     * 
     */
    packet();

    /**
     * @brief Destroy the packet object
     * 
     */
    virtual ~packet();
};
