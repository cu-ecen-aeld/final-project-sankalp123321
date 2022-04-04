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

#include "socket.h"
#define ALLOWED_BACKLOG_CONNS 10
#define IP_MAX_LEN 100
#define MAX_BUFFER_SIZE 1024

void *get_in_addr(struct sockaddr *sa) {
  return sa->sa_family == AF_INET
    ? (void *) &(((struct sockaddr_in*)sa)->sin_addr)
    : (void *) &(((struct sockaddr_in6*)sa)->sin6_addr);
}

tcpServer::tcpServer(std::string ipAddr, std::string socketID):
    server_fd(-1)
{
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

    std::cout << "Opening the socket.\n" << std::endl;

    server_fd = socket(s_addr.ai_family, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        syslog(LOG_ERR, "socket: %s", strerror(errno));
        freeaddrinfo(new_addr);
    }

    std::cout << "Binding the file descriptor[%d]." <<  server_fd << std::endl;

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

    printf("Listening on the socket.\n");

    int listen_ret = listen(server_fd, ALLOWED_BACKLOG_CONNS);
    if (listen_ret < 0)
    {
        syslog(LOG_ERR, "listen: %s", strerror(errno));
    }

    socklen_t add_size = sizeof(client_addr);
    struct sockaddr *sck_addr = (struct sockaddr *)&client_addr;

    int new_fd = accept(server_fd, sck_addr, &add_size);
    if (new_fd < 0)
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

    syslog(LOG_INFO, "Accepted connection from %s\n", str_ip);
}

tcpServer::~tcpServer()
{
}

tcpClient::tcpClient(std::string ipAddr, std::string socketID)
{
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

    std::cout << "Opening the socket.\n" << std::endl;

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
        break;
    }

    if(ptr == NULL)
    {
        syslog(LOG_ERR, "socket: %s", strerror(errno));
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
    std::cout << "Connecting to : " << str_ip << std::endl;
    freeaddrinfo(new_addr);
}
tcpClient::~tcpClient()
{

}