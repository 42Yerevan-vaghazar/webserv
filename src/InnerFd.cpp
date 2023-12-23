#include "InnerFd.hpp"

InnerFd::InnerFd(int fd, Client  &client, std::string &str, int flag)
    : _client(client), _str(str) {
    _fd = fd;
    _flag = flag;
};
InnerFd::InnerFd(const InnerFd &obj) : _str(obj._str), _client(obj._client) {
    _fd = obj._fd;
    _flag = obj._flag;
};

InnerFd::~InnerFd(){};