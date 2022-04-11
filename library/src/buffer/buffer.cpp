/**
 * @file buffer.cpp
 * @author your name (you@domain.com)
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
}

uint8_t buffer::AddToExternalBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    std::lock_guard<std::mutex> lock (mutex);
    for (uint16_t i = 0; i < numOfBytes; i++)
    {
        // printf("0x%02X ", bytes[i]);
        externalPacketQueue.push_back(bytes[i]);
    }
    // printf("\r\n");
}

uint8_t buffer::AddToInternalBuffer(packet pack)
{
    std::lock_guard<std::mutex> lock (mutex);
    internalPacketQueue.push_back(pack);
}