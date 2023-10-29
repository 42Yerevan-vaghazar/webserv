#pragma once
#include <map>
#include <iostream>
#include "DefaultSetup.hpp"

class Client
{
    public:
        Client() {
            memset(&_addrClient , 0 , sizeof(struct sockaddr));
        };

        bool operator<(const Client& rhs) const
        {
            return _fd < rhs._fd;
        }

        bool operator==(int fd) {
            return(_fd == fd);
        }

        bool operator==(const struct sockaddr &addrClient) {
            return(!std::memcmp(&_addrClient, &addrClient, sizeof(sockaddr)));
        }

        int getFd(const int fd) const {
            return (_fd);
        }

        void setFd(const int fd) {
            _fd = fd;
        }

        struct sockaddr &getAddr() {
            return (_addrClient);
        }

        void setAddr(const struct sockaddr &addrClient) {
            _addrClient = addrClient;
        }

        socklen_t &getAddrLen() {
            return (_sockLen);
        }

        ~Client() {};

    private:
        int _fd;
        char _http[READ_BUFFER];
        std::string _httpRequest;
        std::string _requestLine;
        std::string _body;
    private:
        unsigned long int bodySize;
        std::map<std::string, std::string> httpHeaders;
        socklen_t _sockLen;
    private:
        // struct sockaddr_in      _ClientInfo;
        struct sockaddr         _addrClient;
        // struct sockaddr_storage _addrClient;
};