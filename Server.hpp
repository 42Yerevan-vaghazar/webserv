#pragma once
#include "Client.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <errno.h>


#include <iostream>
#include <fstream>

const int _clientLimit = 1000;
const int _serverLimit = 1000;

class Server
{
    public:
        Server(const std::string &ipAddress, int port) 
            : _port(port), _ipAddress(ipAddress) {};
        ~Server() {};
        bool start() {
            int a = 1;
            _serverSockets[0] = socket(AF_INET, SOCK_STREAM, 0);

            if (setsockopt(_serverSockets[0], SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0) {
                std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
            }

            struct sockaddr_in addr;
            memset(&addr , 0 , sizeof(struct sockaddr_in));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(_ipAddress.c_str());
            addr.sin_port = htons(_port);
            std::cout << "addr.sin_port = " << addr.sin_port << std::endl;
            std::cout << "addr.sin_addr.s_addr = " << addr.sin_addr.s_addr << std::endl;

            if (bind(_serverSockets[0], (sockaddr *)&addr, sizeof(addr)) == -1) {
                perror("bind: ");
                exit(1);
            }

            if (listen(_serverSockets[0], 0) == -1) {
                perror("listen: ");
                exit(1);
            };

            while(true) {
                // struct sockaddr_in addrClient;
                // socklen_t len;
                // memset(&addrClient , 0 , sizeof(struct sockaddr_in));
                // clientSocket = accept(serverSocket, (sockaddr *)&addrClient, &len);
                int clientSocket = accept(_serverSockets[0], NULL, NULL);
                // std::cout << "addrClient.sin_port = " << addrClient.sin_port << std::endl;
                // std::cout << "addrClient.sin_addr.s_addr = " << addrClient.sin_addr.s_addr << std::endl;

                if (clientSocket == -1) {
                    perror("accept: ");
                    exit(1);
                }
                char buf[2044];

                if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
                    perror("recv :");
                    exit(1);
                }
                std::cout << buf << std::endl;
                const char *response = get("./ind.html");
                if (send(clientSocket, response, strlen(response), 0) == -1)
                {
                    perror("send :");
                    exit(1);
                }
                close(clientSocket);
            }
        };

        const char *get(std::string request) {
            std::string response;
            int statusCode = 200;
            std::string status = "OK\r\n";
            std::string contentType = "Content-Type: text/html\r\n";

            response += "HTTP/1.1 ";

            if (access(request.c_str(), F_OK) == 0) {   // TODO check permission to read
                std::string fileContent;
                std::ifstream ifs(request);
                if (ifs.is_open() == false) {
                    throw std::logic_error("can not open file");
                }
                std::getline(ifs, fileContent, '\0');
                response += std::to_string(statusCode);
                response +=  status;
                response +=  contentType;
                response +=  "Content-Length: " + std::to_string(fileContent.size());
                response +=  "\n\n";
                response +=  fileContent;
            } else {
                std::string fileContent;
                std::ifstream ifs("./error_pages/404.html");
                if (ifs.is_open() == false) {
                    throw std::logic_error("can not open file");
                }
                std::getline(ifs, fileContent, '\0');
                response += std::to_string(404);
                response +=   "not found";
                response +=  contentType;
                response +=  "Content-Length: " + std::to_string(fileContent.size());
                response +=  "\n\n";
                response +=  fileContent;
            }
            return (response.c_str());
        };
        bool post();
        bool del();
    private:
        std::string _ipAddress;
        int _port;
        int _serverSockets[_serverLimit];
};