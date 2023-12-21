#include "InnerFd.hpp"

InnerFd::InnerFd(int fd, Client  &client, std::string &str, int flag) {
    _fd = fd;
    _str = &str;
    _client = &client;
    _flag = flag;
};
InnerFd::InnerFd(const InnerFd &obj) : _str(obj._str) {
    _fd = obj._fd;
    _client = obj._client;
    _flag = obj._flag;
};

InnerFd &InnerFd::operator=(const InnerFd &obj) {
    if (this != &obj) {
        _fd = obj._fd;
        _client = obj._client;
        _flag = obj._flag;
    }
    return (*this);
};

InnerFd::~InnerFd(){};


bool InnerFd::operator<(int fd) const {
    return (_fd < fd);
}