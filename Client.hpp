#pragma once
#include <map>
#include <iostream>
#include "DefaultSetup.hpp"

#define BODY_LIMIT 3000

class Client
{
    public:
        Client() : _fd(0), bodySize(BODY_LIMIT), _isRequestReady(false) {
            memset(&_addrClient , 0 , sizeof(struct sockaddr));
        };
        ~Client() {};

        bool operator<(const Client& rhs) const
        {
            return _fd < rhs._fd;
        }

        bool operator==(int fd) {
            return(_fd == fd);
        }

        bool operator==(const Client &rhs) {
            return(_fd == rhs._fd);
        }

        // bool operator==(const struct sockaddr_in &addrClient) {
        //     return(!std::memcmp(&_addrClient, &addrClient, sizeof(sockaddr_in)));
        // }

        int getFd() const {
            return (_fd);
        }

        void setFd(const int fd) {
            _fd = fd;
        }

        struct sockaddr_in &getAddr() {
            return (_addrClient);
        }

        void setAddr(const struct sockaddr_in &addrClient) {
            _addrClient = addrClient;
        }

        std::string getRequestLine() {
            return (_requestLine);
        }

        std::string getBody() {
            return (_body);
        }

        socklen_t &getAddrLen() {
            return (_sockLen);
        }

        std::string receiveMessage() {
            char buf[READ_BUFFER];
            int rdSize = recv(_fd, buf, sizeof(buf), 0);

            std::cout << "rdSize = " << rdSize << std::endl;
            if (rdSize == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK)) {
                throw std::runtime_error(std::string("recv: ") + strerror(errno));
            } else if (rdSize == 0) {  // TODO close tab. send response?
                _isRequestReady = true;
                return (_requestLine);
            }
            buf[rdSize] = '\0';
            _requestLine += buf;
            // std::cout << "_requestLine = " << _requestLine << std::endl;
            return (_requestLine);
        }

        bool isRequestReady() {
            return (_isRequestReady);
        }

    private:
        int _fd;
        char _http[READ_BUFFER];
        std::string _httpRequest;
        std::string _requestLine;
        std::string _body;
        bool _isRequestReady;
    private:
        unsigned long int bodySize;
        std::map<std::string, std::string> httpHeaders;
        socklen_t _sockLen;
    private:
        // struct sockaddr_in      _ClientInfo;
        struct sockaddr_in         _addrClient;
        // struct sockaddr_storage _addrClient;
};