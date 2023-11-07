#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <cstdio>
#include <fstream>
#include <unordered_map>

#include <fcntl.h>

#include <set>
// #include <unordered_set>
#include <map>
#include <vector>
#include <sstream>

#include	<sys/types.h>
// #include	<sys/event.h>
#include	<sys/time.h>


#include "Client.hpp"
#include "EvManager.hpp"

#include <stdlib.h>

#define PROTOCOL "HTTP/1.1"

#include "DefaultSetup.hpp"
#include <exception>

const int HEADER_SIZE = 1000;
const int CLIENT_LIMIT = 1000;
const int SERVER_LIMIT = 1000;
const int MAX_MSG_SIZE = 10000;


class Server
{
    public:
        Server(const std::string &ipAddress, int port);
        ~Server();
        bool start();
        void eventLoop();
        std::string generateResponse(const std::string &httpRequest, const std::string &body);
        std::string get(const std::string &fileName, const std::string  &contentType,
            std::unordered_map<std::string, std::string> &headerContent);
        std::string post(const std::string &fileName, const std::string &body);
        std::string del(const std::string &fileName);

    private:
        bool closeConnetcion(int fd);
        std::string _ipAddress;
        int _port;
        int _serverSocket;
        int _clientSockets[CLIENT_LIMIT];
        std::vector<std::string> _data;
        std::map<int, Client> _clients;

    class Error : public std::exception
    {
        public:
            Error(int statusCode, const std::string &errMessage)
                : _statusCode(statusCode), _errMessage(errMessage) {};
            Error(const Error& rhs) {
                _statusCode = rhs._statusCode;
                _errMessage = rhs._errMessage;
            };
            ~Error() throw() {};
            
            const char * what() const throw() {
                return _errMessage.c_str();
            }

            int getStatusCode() const {
                return (_statusCode);
            }

        private:
            Error() : _statusCode(0) {};
            Error& operator=(const Error& rhs);
        private:
            int _statusCode;
            std::string _errMessage;
    };
};