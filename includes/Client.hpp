#pragma once
#include <map>
#include <iostream>
#include "DefaultSetup.hpp"

#define BODY_LIMIT 3000

class Client
{
    public:
        Client() : _fd(0), bodySize(BODY_LIMIT), _isRequestReady(false), _isHeaderReady(false),
            _isBodyReady(false), _isOpenConnection(true), _isResponseReady(false) {
            memset(&_addrClient , 0 , sizeof(struct sockaddr));
        };
        ~Client() {
            // close(_fd);
        };

        bool operator<(const Client& rhs) const
        {
            return _fd < rhs._fd;
        }

        // bool operator==(int fd) {
        //     return(_fd == fd);
        // }

        // bool operator==(const Client &rhs) {
        //     return(_fd == rhs._fd);
        // }

        // bool operator==(const struct sockaddr_in &addrClient) {
        //     return(!std::memcmp(&_addrClient, &addrClient, sizeof(sockaddr_in)));
        // }

        int getFd() const {
            return (_fd);
        }

        void setFd(const int fd) {
            _fd = fd;
        }

        void closeFd() const {
            close(_fd);
        }

        struct sockaddr_in &getAddr() {
            return (_addrClient);
        }

        void setAddr(const struct sockaddr_in &addrClient) {
            _addrClient = addrClient;
        }

        std::string getHttpRequest() {
            return (_httpRequest);
        }

        std::string getBody() {
            return (_body);
        }

        socklen_t &getAddrLen() {
            return (_sockLen);
        }

        void setResponse(const std::string &response) {
            _response = response;
            _isResponseReady = true;
        }

        void receiveMessage() {
            char buf[READ_BUFFER];
            int rdSize = recv(_fd, buf, sizeof(buf) - 1, 0);

            std::cout << "rdSize = " << rdSize << std::endl;
            if (rdSize == -1 && !(errno == EAGAIN || errno == EWOULDBLOCK)) {  // TODO Checking the value of errno is strictly forbidden after a read or a write operation.
                throw std::runtime_error(std::string("recv: ") + strerror(errno));
            } if (rdSize == 0) {  // TODO close tab. send response?
                _isOpenConnection = false;
            }
            buf[rdSize] = '\0';
            if (_isHeaderReady == false) {
                _httpRequest += buf;
                size_t headerEndPos = _httpRequest.find("\n\r\n");

                if (headerEndPos == std::string::npos) {
                    return ;
                }
                _isHeaderReady = true;
                _body = _httpRequest.erase(headerEndPos);
                // TODO  parse header
                return ;
            }
            if (rdSize == -1) {   // TODO check body length to do so
                _isBodyReady = true;
                _isRequestReady = true;
                return ;
            }
            _body += buf;
        }

        void sendMessage() {
            if (send(_fd, _response.c_str(), _response.size(), 0) == -1) {
                perror("send :");
                exit(1);
            }
            _response.clear();
            _isResponseReady = false;
        }

        bool isRequestReady() {
            return (_isRequestReady);
        }
        bool isResponseReady() {
            return (_isResponseReady);
        }

    private:
        int _fd;
        char _http[READ_BUFFER];
        std::string _httpRequest;
        std::string _requestLine;
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
        // struct sockaddr_in      _ClientInfo;
        struct sockaddr_in         _addrClient;
        // struct sockaddr_storage _addrClient;
};