#include <iostream>
#include "logger/logger.h"
#include "example.h"

example::example(uint32_t threadID) : 
    threadBase (threadID)
{
    routingTbl* rTbl = routingTbl::GetRoutingTableInst();
    rTbl->registerThread(threadID, this);

    RegisterMethods(0x4100, CastHadnler(&example::MessageHandler, this));
    // RegisterMethods(0x8100, &MessageHandler);
    // uint8_t dataStram[12] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    // Register the thread
    
    // packet sendData(34567, 45677);
    // sendData.Serialize(dataStram, sizeof(dataStram));
    // packet::SendMessage(sendData, 56776);
}

example::~example()
{
}

int example::MessageHandler(packet* packet)
{
    printf("MessageHandler called \n");
    for (uint16_t i = 0; i < packet->datagram.m_payLoadSize; i++)
    {
        printf("%02X ", packet->datagram.m_payload[i]);
    }
    printf("\r\n");
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
    // std::cout << "Hello Client." << std::endl;
    CPPLogger* cpplogger = CPPLogger::getLoggerInst();
    logger_setLevel(cpplogger, LEVEL_TRACE);
    logger_log(cpplogger, LEVEL_INFO, "Hello Client.\n");
    example ex(34567);
    threadClient tClient(56776, argc[1], argc[2]);

    threadMgmt *tManager = threadMgmt::OverWatch(); 
    tManager->managerThread();
    return  0;
} 