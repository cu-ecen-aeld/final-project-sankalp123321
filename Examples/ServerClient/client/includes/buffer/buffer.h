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
#include "msgBase/baseMsg.h"
#include "packet/packet.h"
#include "ackMsg/ackMsg.h"

class buffer
{
private:
    /* data */
    
    static buffer* m_bufferInst;
    std::deque<packet> internalPacketQueue;
    std::deque<ackMsg> internalAckMsgQueue;
    static std::mutex _mutex_singleton;
    std::mutex mutex;
    std::mutex ackMutex;
    buffer();
public:
    static buffer* GetBufferInst();
    uint8_t AddToInternalBuffer(packet pack);
    uint8_t PopFromInternalBuffer(packet*);
    uint8_t AddToInternalBuffer(ackMsg pack);
    uint8_t PopFromInternalBuffer(ackMsg*);
    ~buffer();
};
#endif