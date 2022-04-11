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
        printf("0x%02X ", bytes[i]);
        externalPacketQueue.push_front(bytes[i]);
    }
    printf("\r\n");
    return 0;
}

uint8_t buffer::AddToInternalBuffer(packet pack)
{
    std::lock_guard<std::mutex> lock (mutex);
    internalPacketQueue.push_front(pack);
    return 0;
}

uint8_t buffer::PopFromExternalBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    int i = 0;
    std::lock_guard<std::mutex> lock (mutex);
    while (!externalPacketQueue.empty())
    {
        if (i == numOfBytes)
        {
            break;
        }
        // printf("0x%02X ", bytes[i]);
        bytes[i] = externalPacketQueue.back();
        externalPacketQueue.pop_back();
        i++;
    }
    // printf("\r\n");
    return i;
}

packet buffer::PopFromInternalBuffer()
{
    std::lock_guard<std::mutex> lock (mutex);
    packet pack = internalPacketQueue.back();
    internalPacketQueue.pop_back();
    return pack;
}