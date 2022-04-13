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

class threadMgmt
{
private:
    threadMgmt(/* args */);
    static threadMgmt *m_threadMgmtInst;
    routingTbl* m_rTbl;
    
    std::vector<std::thread*> threadList;
public:
    static void messageRouter();
    void managerThread();       
    static threadMgmt *OverWatch(); 
    ~threadMgmt();
};
