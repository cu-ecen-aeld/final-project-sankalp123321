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

uint8_t buffer::AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    std::lock_guard<std::mutex> lock (mutex);
    for (uint16_t i = 0; i < numOfBytes; i++)
    {
        printf("0x%02X ", bytes[i]);
        externalRxPacketQueue.push_front(bytes[i]);
    }
    printf("\r\n");
    return 0;
}

uint8_t buffer::PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    int i = 0;
    std::lock_guard<std::mutex> lock (mutex);
    while (!externalRxPacketQueue.empty())
    {
        if (i == numOfBytes)
        {
            break;
        }
        // printf("0x%02X ", bytes[i]);
        bytes[i] = externalRxPacketQueue.back();
        externalRxPacketQueue.pop_back();
        i++;
    }
    // printf("\r\n");
    return i;
}

uint8_t buffer::AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{

}
uint8_t buffer::PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{

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