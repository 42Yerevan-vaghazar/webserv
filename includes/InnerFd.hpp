#pragma once
#include <iostream>
#include "Client.hpp"

class Client;

struct InnerFd
{
    InnerFd(int fd, Client  &client, std::string &str, int flag) {
        _fd = fd;
        _str = &str;
        _client = &client;
        _flag = flag;
    };
    InnerFd(const InnerFd &obj) : _str(obj._str) {
        _fd = obj._fd;
        _client = obj._client;
        _flag = obj._flag;
    };

    InnerFd &operator=(const InnerFd &obj) {
        if (this != &obj) {
            _fd = obj._fd;
            _client = obj._client;
            _flag = obj._flag;
        }
        return (*this);
    };

    ~InnerFd(){};

    // bool operator<(const InnerFd &obj) const {
    //     return (_fd < obj._fd);
    // }

    bool operator<(int fd) const {
        return (_fd < fd);
    }

    int _fd;
    Client * _client;
    std::string *_str;
    int _flag;
};
// TODO move to cpp