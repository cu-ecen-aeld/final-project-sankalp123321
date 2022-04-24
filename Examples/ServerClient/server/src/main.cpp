#include <iostream>
#include "threadBase/threadBase.h"
#include "router/routingTbl.h"
#include "threadMgmt/threadMgmt.h"
#include "threadServer/threadServer.h"
#include "threadClient/threadClient.h"
#include "packet/packet.h"
#include "ackMsg/ackMsg.h"

class example : public threadBase
{
private:
    /* data */
public:
    int rxMsgHandler(ackMsg *msg);
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

    RegisterMethods(0x4100, CastHadnler(&example::rxMsgHandler, this));
}

example::~example()
{
}

int example::rxMsgHandler(ackMsg *msg)
{
    CPPLogger *cpplog = CPPLogger::getLoggerInst();
    printf("rxMsgHandler called \n");
    logger_log(cpplog, LEVEL_INFO, "Header[%04X] SrcAddr[0x%04X] DestAddr[0x%04X] isAck[%x] msgID[%x] errorcode[%d] cksum[%02X] \r\n", msg->ackBackMsg.m_header, msg->ackBackMsg.m_srcThreadID, \
    msg->ackBackMsg.m_destThreadID, msg->ackBackMsg.m_isAck, msg->ackBackMsg.m_msgID, msg->ackBackMsg.m_errorCode, msg->ackBackMsg.m_cksum);
    return 0;
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
    logger_log(cpplogger, LEVEL_INFO, "Hello Server.\n");
    example ex(34567);
    threadServer tServer(56775, argc[1], argc[2]);

    uint8_t dataStram[12] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    packet sendData(34567, 45427, 0x4100);
    sendData.Serialize(dataStram, sizeof(dataStram));
    threadMgmt::SendMessage(sendData, 56775);

    threadMgmt *tManager = threadMgmt::OverWatch(); 
    tManager->managerThread();
    return  0;
}