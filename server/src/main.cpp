#include <iostream>
#include "threadBase/threadBase.h"
#include "router/routingTbl.h"
#include "threadMgmt/threadMgmt.h"
#include "threadServer/threadServer.h"
#include "packet/packet.h"

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
    uint8_t dataStram[12] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);

    // packet sendData(56788, 45427);
    // sendData.Serialize(dataStram, sizeof(dataStram));
    // packet::SendMessage(sendData, 56790);
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