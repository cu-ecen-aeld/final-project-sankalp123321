/**
 * @file buffer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <deque>
#include <vector>
#include "packet/packet.h"

class buffer
{
private:
    /* data */
    
    static buffer* m_bufferInst;
    std::deque<uint8_t> externalPacketQueue;
    std::deque<packet> internalPacketQueue;
    buffer();
public:
    static buffer* GetBufferInst();
    uint8_t AddToExternalBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t AddToInternalBuffer(packet pack);
    ~buffer();
};
#endif