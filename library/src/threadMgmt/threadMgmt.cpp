/**
 * @file threadMgmt.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <threadMgmt.h>

threadMgmt *threadMgmt::m_threadMgmtInst = nullptr;

threadMgmt::threadMgmt() :
    m_rTbl(nullptr)
{
    m_rTbl = routingTbl::GetRoutingTableInst();
}

threadMgmt::~threadMgmt()
{
}


void threadMgmt::managerThread()
{
    for (auto &i : m_rTbl->routerTableInst())
    {
        
    }
}

threadMgmt *threadMgmt::OverWatch()
{ 
    if(m_threadMgmtInst == nullptr)
    {
        m_threadMgmtInst = new threadMgmt;
    }

    return m_threadMgmtInst;
}