#pragma once

class Client
{
    public:
        Client() 
            : _len(sizeof(struct sockaddr_in)){
            memset(&_addrClient , 0 , sizeof(struct sockaddr_in));
        };

        bool operator<(const Client& rhs) const
        {
            return _fd < rhs._fd;
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

        socklen_t &getAddrLen() {
            return (_len);
        }

        void setAddr(const struct sockaddr &addrClient) {
            _addrClient = addrClient;
        }

        void setAddrLen(const socklen_t &len) {
            _len = len;
        }

        bool operator==(int fd) {
            return(_fd == fd);
        }

        ~Client() {};
    private:
        struct sockaddr _addrClient;
        socklen_t _len;
        int _fd;
};