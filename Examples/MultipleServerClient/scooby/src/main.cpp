#include <iostream>
#include "threadBase/threadBase.h"
#include "router/routingTbl.h"
#include "threadMgmt/threadMgmt.h"
#include "threadServer/threadServer.h"
#include "threadClient/threadClient.h"
#include "packet/packet.h"
#include "logger/logger.h"

class example : public threadBase
{
private:
    /* data */
public:
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes) override;
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    virtual void AddToTxBuffer(uint8_t* data, uint16_t numOfBytes){}
    virtual uint16_t PopRxBuffer(uint8_t* data, uint16_t numOfBytes){}
    example(uint32_t threadID);
    virtual ~example();
};

example::example(uint32_t threadID) : 
    threadBase(threadID)
{
    // Register the thread
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);

    for (size_t i = 0; i < 1; i++)
    {
        
    }
}

example::~example()
{
}

void example::RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes)
{
    printf("RecvMessageAsync called \n");
    for (uint16_t i = 0; i < numOfBytes; i++)
    {
        printf("%02X ", buffer[i]);
    }
    printf("\r\n");
}

void example::PeriodicFunction()
{

}

void example::Notification(uint8_t notifId)
{

}

int main(int argv, const char *argc[])
{
    // std::cout << "Hello Server." << std::endl;
    CPPLogger* cpplogger = CPPLogger::getLoggerInst();
    logger_setLevel(cpplogger, LEVEL_TRACE);
    logger_log(cpplogger, LEVEL_INFO, "Hello Scooby.\n");
    example ex(34567);
    threadServer tServer_1(56775, argc[1], argc[2]);
	threadServer tServer_2(56780, argc[1], argc[3]);
    threadClient tClient (62437, argc[1], argc[4]);

    uint8_t dataStram[12] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    packet sendDataDooby(34569, 45427);
    sendDataDooby.Serialize(dataStram, sizeof(dataStram));
    packet::SendMessage(sendDataDooby, 56775);

    packet sendDataDoo(34567, 45427);
    sendDataDoo.Serialize(dataStram, sizeof(dataStram));
    packet::SendMessage(sendDataDoo, 56780);
    

    threadMgmt *tManager = threadMgmt::OverWatch(); 
    tManager->managerThread();
    return  0;
}