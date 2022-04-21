/**
 * @file routingTbl.h
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
#include <vector>
#include <map>
#include "threadBase/threadBase.h"
#include "logger/logger.h"

class routingTbl
{
private:
    /* data */
    CPPLogger* cpplogger;
    routingTbl(/* args */);
    static routingTbl *m_RTblInst;
public: 
    std::map<uint32_t, threadBase*> table;
    std::map<uint32_t, threadBase*> routerTableInst();
    routingTbl(routingTbl&) = delete;
    void operator=(const routingTbl&) = delete;
    static routingTbl* GetRoutingTableInst();
    void printTable();
    void registerThread(uint32_t threadID, threadBase* thread);
    threadBase* GetThreadInstanceFromID(uint32_t threadID);
    ~routingTbl();
};
