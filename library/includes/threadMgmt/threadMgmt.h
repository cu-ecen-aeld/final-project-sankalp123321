/**
 * @file threadMgmt.h
 * @author Sankalp Agrawal (saag2511@colorado.edu)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <iostream>
#include <thread>
#include <vector>

#include "router/routingTbl.h"
#include "logger/logger.h"
#include "ackMsg/ackMsg.h"

class threadMgmt
{
private:
    threadMgmt(/* args */);
    static threadMgmt *m_threadMgmtInst;
    routingTbl* m_rTbl;
    std::thread* messageRoutingThread;
    std::vector<std::thread*> threadList;
    static uint16_t pktIDCntr;
public:
    CPPLogger* cpplogger;
    static void messageRouter();
    static void SendAckMessage(ackMsg &pkt);
    static void SendAckMessage(ackMsg &pkt, uint32_t commdThread);
    static void SendMessage(packet &pkt);
    static void SendMessage(packet &pkt, uint32_t commdThread);
    void managerThread();       
    static threadMgmt *OverWatch(); 
    ~threadMgmt();
};
