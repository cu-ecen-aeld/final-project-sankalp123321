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
#include <mutex>
#include "packet/packet.h"

class buffer
{
private:
    /* data */
    
    static buffer* m_bufferInst;
    std::deque<uint8_t> externalRxPacketQueue;
    std::deque<uint8_t> externalTxPacketQueue;
    std::deque<packet> internalPacketQueue;
    static std::mutex _mutex_singleton;
    std::mutex mutex;
    buffer();
public:
    static buffer* GetBufferInst();
    uint8_t AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t AddToInternalBuffer(packet pack);
    uint8_t PopFromInternalBuffer(packet*);
    ~buffer();
};
#endif