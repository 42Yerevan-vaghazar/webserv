#pragma once
#include <iostream>
#include "Client.hpp"

class Client;

struct InnerFd
{
    InnerFd(int fd, Client  &client, std::string &str, int flag);
    InnerFd(const InnerFd &obj);
    InnerFd &operator=(const InnerFd &obj);
    ~InnerFd();
    bool operator<(int fd) const;
    int _fd;
    Client * _client;
    std::string *_str;
    int _flag;
};