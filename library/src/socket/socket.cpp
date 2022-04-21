/**
 * @file socket.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <cstring>
#include <syslog.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include "buffer/buffer.h"
#include "socket.h"

#define MAX_RETRIAL_TIMES 5

#define ALLOWED_BACKLOG_CONNS 10
#define IP_MAX_LEN 100
#define MAX_BUFFER_SIZE 1024

void *get_in_addr(struct sockaddr *sa) {
  return sa->sa_family == AF_INET
    ? (void *) &(((struct sockaddr_in*)sa)->sin_addr)
    : (void *) &(((struct sockaddr_in6*)sa)->sin6_addr);
}

tcpServer::tcpServer(std::string ipAddr, std::string socketID):
    server_fd(-1), clientFd(-1), cpplogger(nullptr)
{
    cpplogger = CPPLogger::getLoggerInst();
    openlog("icl-lib", LOG_CONS | LOG_PERROR | LOG_PID, LOG_USER);

    struct addrinfo s_addr, *new_addr = nullptr;
    struct sockaddr_storage client_addr;

    memset(&s_addr, 0, sizeof(s_addr));

    s_addr.ai_family = AF_INET;
    s_addr.ai_socktype = SOCK_STREAM;
    s_addr.ai_flags = AI_PASSIVE;

    int ret = getaddrinfo(nullptr, socketID.c_str(), &s_addr, &new_addr);
    if (ret != 0)
    {
        syslog(LOG_ERR, "getaddrinfo error %s\n", gai_strerror(ret));
    }

    logger_log(cpplogger, LEVEL_DEBUG, "Opening the socket.\n");

    server_fd = socket(s_addr.ai_family, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        syslog(LOG_ERR, "socket: %s", strerror(errno));
        freeaddrinfo(new_addr);
    }

    logger_log(cpplogger, LEVEL_DEBUG, "Binding the file descriptor[%d].\n", server_fd);

    struct addrinfo *tmp = nullptr;
    int bind_ret = 0, binded = 0;
    for (tmp = new_addr; tmp != nullptr; tmp = tmp->ai_next)
    {
        bind_ret = bind(server_fd, tmp->ai_addr, new_addr->ai_addrlen);
        if (bind_ret != -1)
        {
            binded = 1;
            break;
        }
    }

    if (!binded)
    {
        syslog(LOG_ERR, "bind: %s", strerror(errno));
        freeaddrinfo(new_addr);
    }

    freeaddrinfo(new_addr);

    logger_log(cpplogger, LEVEL_DEBUG, "Listening on the socket.\n");

    int listen_ret = listen(server_fd, ALLOWED_BACKLOG_CONNS);
    if (listen_ret < 0)
    {
        syslog(LOG_ERR, "listen: %s", strerror(errno));
    }

    socklen_t add_size = sizeof(client_addr);
    struct sockaddr *sck_addr = (struct sockaddr *)&client_addr;

    clientFd = accept(server_fd, sck_addr, &add_size);
    if (clientFd < 0)
    {
        syslog(LOG_ERR, "accept: %s", strerror(errno));
        // return EXIT_FAILURE;
    }

    char str_ip[IP_MAX_LEN];
    const char *returnVal = inet_ntop(s_addr.ai_family, get_in_addr((struct sockaddr *)&client_addr), str_ip, IP_MAX_LEN);
    if (returnVal == nullptr)
    {
        syslog(LOG_ERR, "inet_ntop: %s", strerror(errno));
        // return EXIT_FAILURE;
    }

    logger_log(cpplogger, LEVEL_INFO, "Accepted connection from %s\n", str_ip);
    SocketSendThread = new std::thread(sendThread, this);
    SocketRecvThread = new std::thread(recvThread, this);
}

tcpServer::~tcpServer()
{
}

uint8_t tcpServer::AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    std::lock_guard<std::mutex> lock (mutex);
    // logger_log(cpplogger, LEVEL_DEBUG, "Rx Data: ");
    for (uint16_t i = 0; i < numOfBytes; i++)
    {
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        externalRxPacketQueue.push_front(bytes[i]);
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return 0;
}

uint8_t tcpServer::PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    int i = 0;
    std::lock_guard<std::mutex> lock (mutex);
    while (!externalRxPacketQueue.empty())
    {
        if (i == numOfBytes)
        {
            break;
        }
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        bytes[i] = externalRxPacketQueue.back();
        externalRxPacketQueue.pop_back();
        i++;
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return i;
}

uint8_t tcpServer::AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    std::lock_guard<std::mutex> lock (mutex);
    for (uint16_t i = 0; i < numOfBytes; i++)
    {
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        externalTxPacketQueue.push_front(bytes[i]);
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return 0;
}
uint8_t tcpServer::PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    int i = 0;
    std::lock_guard<std::mutex> lock (mutex);
    while (!externalTxPacketQueue.empty())
    {
        if (i == numOfBytes)
        {
            break;
        }
        
        bytes[i] = externalTxPacketQueue.back();
        externalTxPacketQueue.pop_back();
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        i++;
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return i;
}

void tcpServer::sendThread(tcpServer* inst)
{
    uint8_t buffer[256];
    logger_log(inst->cpplogger, LEVEL_DEBUG, "Running thread sendThread.\n");
    while (1)
    {
        uint16_t numOfBytesRead = inst->PopFromExternalTxBuffer(buffer, sizeof(buffer));
        if(!numOfBytesRead)
        {
            continue;
        }

        logger_log(inst->cpplogger, LEVEL_DEBUG, "Number of Bytes to send [%d].\n", numOfBytesRead);
        
        if(inst->clientFd == -1)
        {
            continue;
        }

        int retVal = 0;
        while(retVal = send(inst->clientFd, buffer, numOfBytesRead, 0))
        {
            if(retVal == -1)
            {
                // error
            }
            if(retVal == numOfBytesRead)
            {
                logger_log(inst->cpplogger, LEVEL_DEBUG, "Successfully sent out data. %dbytes.\n", retVal);
                break;
            }
        }
    }
}
void tcpServer::recvThread(tcpServer* inst)
{
    uint8_t bufferBytes[256];
    logger_log(inst->cpplogger, LEVEL_DEBUG, "Running thread recvThread.\n");
    while (1)
    {
        int ret_val = recv(inst->clientFd, bufferBytes, sizeof(bufferBytes), 0);
        // logger_log(inst->cpplogger, LEVEL_DEBUG, "ret status %d\n",ret_val);
        if(ret_val == -1)
        {
            // perror
        }
        if (!ret_val)
        {
            // No more data to recv
            continue;
        }
        if(ret_val > 0)
        {
            inst->AddToExternalRxBuffer(bufferBytes, ret_val);
        }
    }
}

tcpClient::tcpClient(std::string ipAddr, std::string socketID):
    server_fd(-1), cpplogger(nullptr)
{
    cpplogger = CPPLogger::getLoggerInst();
    struct addrinfo s_addr, *new_addr = nullptr, *ptr;
    struct sockaddr_storage client_addr;

    memset(&s_addr, 0, sizeof(s_addr));

    s_addr.ai_family = AF_INET;
    s_addr.ai_socktype = SOCK_STREAM;

    int ret = getaddrinfo(ipAddr.c_str(), socketID.c_str(), &s_addr, &new_addr);
    if (ret != 0)
    {
        syslog(LOG_ERR, "getaddrinfo error %s\n", gai_strerror(ret));
    }

    for (uint8_t i = 0; i < MAX_RETRIAL_TIMES; i++)
    {
        logger_log(cpplogger, LEVEL_DEBUG, "Trying to connect to the server.\n");

        for (ptr = new_addr; ptr != NULL; ptr = ptr->ai_next)
        {
            server_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (server_fd == -1)
            {
                syslog(LOG_ERR, "socket: %s", strerror(errno));
                continue;
            }

            int connRetVal = connect(server_fd, ptr->ai_addr, ptr->ai_addrlen);
            if(connRetVal == -1)
            {
                close(server_fd);
                continue;
            }
            i = MAX_RETRIAL_TIMES;
            break;
        }
    }

    if(ptr == NULL)
    {
        syslog(LOG_ERR, "socket: %s", strerror(errno));
        logger_log(cpplogger, LEVEL_DEBUG, "No connection available.\r\n");
        exit(EXIT_FAILURE);
    }

    socklen_t add_size = sizeof(client_addr);
    struct sockaddr *sck_addr = (struct sockaddr *)&client_addr;
    
    char str_ip[IP_MAX_LEN];
    const char *returnVal = inet_ntop(ptr->ai_family, get_in_addr((struct sockaddr *)&ptr->ai_addr), str_ip, IP_MAX_LEN);
    if (returnVal == nullptr)
    {
        syslog(LOG_ERR, "inet_ntop: %s", strerror(errno));
        // return EXIT_FAILURE;
    }
    logger_log(cpplogger, LEVEL_DEBUG, "Connecting to : %s\n", str_ip);
    freeaddrinfo(new_addr);
    SocketSendThread = new std::thread(sendThread, this);
    SocketRecvThread = new std::thread(recvThread, this);
}
tcpClient::~tcpClient()
{

}

uint8_t tcpClient::AddToExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    std::lock_guard<std::mutex> lock (mutex);
    // logger_log(cpplogger, LEVEL_DEBUG, "Rx Data: ");
    for (uint16_t i = 0; i < numOfBytes; i++)
    {
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        externalRxPacketQueue.push_front(bytes[i]);
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return 0;
}

uint8_t tcpClient::PopFromExternalRxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    int i = 0;
    std::lock_guard<std::mutex> lock (mutex);
    while (!externalRxPacketQueue.empty())
    {
        if (i == numOfBytes)
        {
            break;
        }
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        bytes[i] = externalRxPacketQueue.back();
        externalRxPacketQueue.pop_back();
        i++;
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return i;
}

uint8_t tcpClient::AddToExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    std::lock_guard<std::mutex> lock (mutex);
    for (uint16_t i = 0; i < numOfBytes; i++)
    {
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        externalTxPacketQueue.push_front(bytes[i]);
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return 0;
}
uint8_t tcpClient::PopFromExternalTxBuffer(uint8_t* bytes, uint16_t numOfBytes)
{
    int i = 0;
    std::lock_guard<std::mutex> lock (mutex);
    while (!externalTxPacketQueue.empty())
    {
        if (i == numOfBytes)
        {
            break;
        }
        
        bytes[i] = externalTxPacketQueue.back();
        externalTxPacketQueue.pop_back();
        // logger_log(cpplogger, LEVEL_DEBUG, "0x%02X ", bytes[i]);
        i++;
    }
    // logger_log(cpplogger, LEVEL_DEBUG, "\r\n");
    return i;
}

void tcpClient::sendThread(tcpClient* inst)
{
    uint8_t buffer[256];
    logger_log(inst->cpplogger, LEVEL_DEBUG, "Running thread sendThread.\n");
    while (1)
    {
        uint16_t numOfBytesRead = inst->PopFromExternalTxBuffer(buffer, sizeof(buffer));
        if(!numOfBytesRead)
        {
            continue;
        }

        logger_log(inst->cpplogger, LEVEL_DEBUG, "Number of Bytes to send [%d].\n", numOfBytesRead);
        
        if(inst->server_fd == -1)
        {
            continue;
        }

        int retVal = 0;
        while(retVal = send(inst->server_fd, buffer, numOfBytesRead, 0))
        {
            if(retVal == -1)
            {
                // error
            }
            if(retVal == numOfBytesRead)
            {
                logger_log(inst->cpplogger, LEVEL_DEBUG, "Successfully sent out data. %dbytes.\n", retVal);
                break;
            }
        }
    }
}
void tcpClient::recvThread(tcpClient* inst)
{
    uint8_t bufferBytes[256];
    logger_log(inst->cpplogger, LEVEL_DEBUG, "Running thread recvThread.\n");
    while (1)
    {
        int ret_val = recv(inst->server_fd, bufferBytes, sizeof(bufferBytes), 0);
        // logger_log(inst->cpplogger, LEVEL_DEBUG, "ret status %d\n",ret_val);
        if(ret_val == -1)
        {
            // perror
        }
        if (!ret_val)
        {
            // No more data to recv
            continue;
        }
        if(ret_val > 0)
        {
            inst->AddToExternalRxBuffer(bufferBytes, ret_val);
        }
    }
}