/**
 * @file socket.h
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
#include <string>
#include <deque>
#include <mutex>
#include <thread>
#include "logger/logger.h"

class tcpServer
{
private:
    /* data */
    int server_fd;
    std::mutex mutex;
    std::deque<uint8_t> externalRxPacketQueue;
    std::deque<uint8_t> externalTxPacketQueue;
    std::thread *SocketSendThread;
    std::thread *SocketRecvThread;
public:
    CPPLogger* cpplogger;
    uint8_t AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    static void sendThread(tcpServer* inst);
    static void recvThread(tcpServer* inst);
    int clientFd;
    tcpServer(std::string ipAddr, std::string socketID);
    ~tcpServer();
};

class tcpClient
{
private:
    /* data */
    int server_fd;
    std::mutex mutex;
    std::deque<uint8_t> externalRxPacketQueue;
    std::deque<uint8_t> externalTxPacketQueue;
    std::thread *SocketSendThread;
    std::thread *SocketRecvThread;
public:
    CPPLogger* cpplogger;
    uint8_t AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    uint8_t PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);
    static void sendThread(tcpClient* inst);
    static void recvThread(tcpClient* inst);
    tcpClient(std::string ipAddr, std::string socketID);
    ~tcpClient();
};
