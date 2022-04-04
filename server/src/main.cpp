#include <iostream>
#include "threadBase/threadBase.h"
#include "router/routingTbl.h"
#include "threadMgmt/threadMgmt.h"
#include "threadServer/threadServer.h"

class example : public threadBase
{
private:
    /* data */
public:
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes);
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    void *ThreadFunc(void *args);
    example(uint32_t threadID);
    ~example();
};

example::example(uint32_t threadID) : 
    threadBase(threadID)
{
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);
}

example::~example()
{
}

void example::RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes)
{

}

void example::PeriodicFunction()
{

}

void example::Notification(uint8_t notifId)
{

}

void *example::ThreadFunc(void *args)
{

}


int main(int argv, const char *argc[])
{
    std::cout << "Hello Server." << std::endl;
    threadServer tServer(56775, "127.0.0.1", "3644");
    example ex(34567);

    threadMgmt *tManager = threadMgmt::OverWatch(); 
    tManager->managerThread();
    return  0;
}