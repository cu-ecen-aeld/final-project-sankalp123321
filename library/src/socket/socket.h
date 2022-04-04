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

class tcpServer
{
private:
    /* data */
    int server_fd;
public:
    tcpServer(std::string ipAddr, std::string socketID);
    ~tcpServer();
};

class tcpClient
{
private:
    /* data */
    int server_fd;
public:
    tcpClient(std::string ipAddr, std::string socketID);
    ~tcpClient();
};
