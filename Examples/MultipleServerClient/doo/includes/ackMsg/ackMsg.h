/**
 * @file ackMsg.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>

#define ACK_HEADER_ID (0x01C0FFEE)
#define ACK_HEADER_ID_INV (0xEEFFC001)

class ackMsg
{
private:
    #pragma pack(1)
    struct AckMsg
    {
        uint32_t m_header;
        uint16_t m_destThreadID;
        uint16_t m_srcThreadID;
        uint8_t m_isAck;
        uint16_t m_errorCode;
        uint8_t m_cksum;
    };
public:
    AckMsg ackBackMsg;
    void setDesThreadID(uint16_t destThrdID) {ackBackMsg.m_destThreadID = destThrdID;}
    void setSrcThreadID(uint16_t srcThrdID) {ackBackMsg.m_srcThreadID = srcThrdID;}
    void setAckStatus(uint8_t isAck) {ackBackMsg.m_isAck = isAck;}
    void setErrorCode(uint16_t errorCode) {ackBackMsg.m_errorCode = errorCode;}

    void calCulateChecksum()
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            ackBackMsg.m_cksum ^= (ackBackMsg.m_srcThreadID & 0xFF);
            ackBackMsg.m_cksum ^= (ackBackMsg.m_destThreadID & 0xFF);
            ackBackMsg.m_cksum ^= (ackBackMsg.m_errorCode & 0xFF);

            ackBackMsg.m_srcThreadID = ackBackMsg.m_srcThreadID >> 8;
            ackBackMsg.m_destThreadID = ackBackMsg.m_destThreadID >> 8;
            ackBackMsg.m_destThreadID = ackBackMsg.m_errorCode >> 8;
        }

        ackBackMsg.m_cksum ^= ackBackMsg.m_isAck;
    }

    ackMsg(/* args */){}
    ~ackMsg(){}
};