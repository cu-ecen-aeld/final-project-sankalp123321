/**
 * @file buffer.cpp
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "buffer.h"

buffer* buffer::m_bufferInst = nullptr;
std::mutex buffer::_mutex_singleton;

buffer* buffer::GetBufferInst()
{
    std::lock_guard<std::mutex> lock(_mutex_singleton);
    if(m_bufferInst == nullptr)
    {
        m_bufferInst = new buffer();
    }
    return m_bufferInst;
}

buffer::buffer(/* args */)
{
}

buffer::~buffer()
{
    if(m_bufferInst != nullptr)
    {
        delete m_bufferInst;
    }
}

uint8_t buffer::AddToInternalBuffer(ackMsg pack)
{
    std::lock_guard<std::mutex> lock (ackMutex);
    internalAckMsgQueue.push_front(pack);
    return 0;
}
uint8_t buffer::PopFromInternalBuffer(ackMsg* rcvdAck)
{
    std::lock_guard<std::mutex> lock (ackMutex);
    if(internalAckMsgQueue.empty())
    {
        return 0;
    }
    *rcvdAck = internalAckMsgQueue.back();
    internalAckMsgQueue.pop_back();
    return 1;
}

uint8_t buffer::AddToInternalBuffer(packet pack)
{
    std::lock_guard<std::mutex> lock (mutex);
    internalPacketQueue.push_front(pack);
    return 0;
}
uint8_t buffer::PopFromInternalBuffer(packet* rcvdPckt)
{
    std::lock_guard<std::mutex> lock (mutex);
    if(internalPacketQueue.empty())
    {
        return 0;
    }
    *rcvdPckt = internalPacketQueue.back();
    internalPacketQueue.pop_back();
    return 1;
}