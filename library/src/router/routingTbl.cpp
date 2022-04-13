/**
 * @file routingTbl.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "routingTbl.h"

routingTbl* routingTbl::m_RTblInst(nullptr);

routingTbl* routingTbl::GetRoutingTableInst()
{
    if(m_RTblInst == nullptr)
    {
        m_RTblInst = new routingTbl;
    }
    // check if memory got allocated.
    return m_RTblInst;
}

routingTbl::routingTbl(/* args */)
{
}

routingTbl::~routingTbl()
{
    delete m_RTblInst;
}

std::map<uint32_t, threadBase*> routingTbl::routerTableInst()
{
    return table;
}

void routingTbl::registerThread(uint32_t threadID, threadBase* thread)
{
    std::cout << "New thread added: " << threadID << std::endl;
    table.emplace(threadID, thread);
    printTable();
}

void routingTbl::printTable()
{
    for(auto &i : table)
    {
        std::cout << "Thread in table: " << i.first << std::endl;
    }
}

threadBase* routingTbl::GetThreadInstanceFromID(uint32_t threadID)
{
    return table.find(threadID)->second;
}