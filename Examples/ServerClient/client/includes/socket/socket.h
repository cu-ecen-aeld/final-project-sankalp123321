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

    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param inst 
     */
    static void sendThread(tcpServer* inst);

    /**
     * @brief 
     * 
     * @param inst 
     */
    static void recvThread(tcpServer* inst);

    int clientFd;

    /**
     * @brief Construct a new tcp Server object
     * 
     * @param ipAddr 
     * @param socketID 
     */
    tcpServer(std::string ipAddr, std::string socketID);

    /**
     * @brief Destroy the tcp Server object
     * 
     */
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
    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param bytes 
     * @param numOfBytes 
     * @return uint8_t 
     */
    uint8_t PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes);

    /**
     * @brief 
     * 
     * @param inst 
     */
    static void sendThread(tcpClient* inst);

    /**
     * @brief 
     * 
     * @param inst 
     */
    static void recvThread(tcpClient* inst);

    /**
     * @brief Construct a new tcp Client object
     * 
     * @param ipAddr 
     * @param socketID 
     */
    tcpClient(std::string ipAddr, std::string socketID);

    /**
     * @brief Destroy the tcp Client object
     * 
     */
    ~tcpClient();
};
