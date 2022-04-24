/**
 * @file baseMsg.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <iostream>

class baseMsg
{
private:
    uint8_t isMessagePacket;

public:

    void setIsMessagePacket(uint8_t val) { isMessagePacket = val; }
    uint8_t getIsMessagePacket() { return isMessagePacket; }

    /**
     * 
     * @brief Construct a new base Msg object
     * 
     */
    baseMsg() { }

    /**
     * @brief Destroy the base Msg object
     * 
     */
    virtual ~baseMsg() { }
};
