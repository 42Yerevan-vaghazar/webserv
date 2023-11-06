#pragma once
#include <map>
#include <iostream>
#include "DefaultSetup.hpp"
#include "EvManager.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


#define BODY_LIMIT 3000

class Client
{
    public:
        Client();

        ~Client();

        bool operator<(const Client& rhs) const;

        int getFd() const;

        void setFd(const int fd);

        void closeFd() const;

        struct sockaddr_in &getAddr();

        void setAddr(const struct sockaddr_in &addrClient);

        std::string getHttpRequest();

        std::string getBody();

        socklen_t &getAddrLen();

        void setResponse(const std::string &response);

        void receiveMessage();

        bool sendMessage();

        bool isRequestReady() const;

        bool isResponseReady() const;

    private:
        int _fd;
        char _http[READ_BUFFER];
        std::string _httpRequest;
        // std::string _requestLine;
        std::string _body;
        std::string _response;
        bool _isHeaderReady;
        bool _isBodyReady;
        bool _isRequestReady;
        bool _isOpenConnection;
        bool _isResponseReady;
    private:
        unsigned long int bodySize;
        std::map<std::string, std::string> httpHeaders;
        socklen_t _sockLen;
    private:
        // struct sockaddr           _ClientInfo;
        struct sockaddr_in         _addrClient;
        // struct sockaddr_storage _addrClient;
};