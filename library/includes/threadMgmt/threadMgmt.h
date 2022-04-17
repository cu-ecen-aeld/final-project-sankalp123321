/**
 * @file threadMgmt.h
 * @author your name (you@domain.com)
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

class threadMgmt
{
private:
    threadMgmt(/* args */);
    static threadMgmt *m_threadMgmtInst;
    routingTbl* m_rTbl;
    std::thread* messageRoutingThread;
    std::vector<std::thread*> threadList;
public:
    CPPLogger* cpplogger;
    static void messageRouter();
    void managerThread();       
    static threadMgmt *OverWatch(); 
    ~threadMgmt();
};
