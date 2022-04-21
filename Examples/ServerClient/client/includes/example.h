#pragma once

#include "threadBase/threadBase.h"
#include "router/routingTbl.h"
#include "threadMgmt/threadMgmt.h"
#include "threadServer/threadServer.h"
#include "threadClient/threadClient.h"
#include "packet/packet.h"

class example : public threadBase
{
private:
    /* data */
public:
    virtual void RecvMessageAsync(uint8_t *buffer, uint8_t numOfBytes) override;
    virtual void PeriodicFunction();
    virtual void Notification(uint8_t notifId);
    int MessageHandler(packet* packet);
    virtual void AddToTxBuffer(uint8_t* data, uint16_t numOfBytes){}
    virtual uint16_t PopRxBuffer(uint8_t* data, uint16_t numOfBytes){}
    example(uint32_t threadID);
    virtual ~example();
};
