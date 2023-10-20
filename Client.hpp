#pragma once

class Client
{
    public:
        Client(socklen_t len = 100) 
            : _len(len) {
            memset(&_addrClient , 0 , sizeof(struct sockaddr_in));
        };
        int getClinetSocket(const int clinetSocket) const {
            return (_clinetSocket);
        }
        void setClinetSocket(const int clinetSocket) {
            _clinetSocket = clinetSocket;
        }
        ~Client() {};
    private:
        struct sockaddr_in _addrClient;
        const socklen_t _len;
        int _clinetSocket;
};