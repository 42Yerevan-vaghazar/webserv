#pragma once
#include "Client.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <errno.h>


#include <cstdio>
#include <iostream>
#include <fstream>
#include <unordered_map>

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
                char buf[4000];

                if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
                    perror("recv :");
                    exit(1);
                }
                std::cout << buf << std::endl;
                // TODO parser should be parser here
                if (buf[0] == 'P') {
                    char buf[4000];
                    if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
                        perror("recv :");
                        exit(1);
                    }
                    std::cout << buf << std::endl;
                    std::string response = post("./test.json", buf);
                    if (send(clientSocket, response.c_str(), strlen(response.c_str()), 0) == -1)
                    {
                        perror("send :");
                        exit(1);
                    }
                } else if (buf[0] == 'G') {
                    std::string response = get("./index.html");
                    if (send(clientSocket, response.c_str(), strlen(response.c_str()), 0) == -1)
                    {
                        perror("send :");
                        exit(1);
                    }
                } else if (buf[0] == 'D') {
                    std::string response = del("./test.json");
                    if (send(clientSocket, response.c_str(), strlen(response.c_str()), 0) == -1)
                    {
                        perror("send :");
                        exit(1);
                    }
                }

                close(clientSocket);
            }
        };

        std::string get(const std::string &fileName) {
            std::unordered_map<std::string, std::string> headerContent; // TODO unordered_map
            std::string response;
            int statusCode = 200;
            std::string status = " OK";
            headerContent["Content-Type"] = "text/html";
            response += "HTTP/1.1 ";

            if (access(fileName.c_str(), F_OK) == 0) {   // TODO check permission to read
                std::string fileContent;
                std::ifstream ifs(fileName);
                if (ifs.is_open() == false) {
                    throw std::logic_error("can not open file");
                }
                std::getline(ifs, fileContent, '\0');
                headerContent["Content-Length"] = std::to_string(fileContent.size());
                // TODO if not html cgi works here to generate fileContent
                response += std::to_string(statusCode);
                response +=  status;
                response +=  "\r\n";


                for (std::unordered_map<std::string, std::string>::iterator it = headerContent.begin();
                    it != headerContent.end(); ++it) {
                        response += it->first;
                        response += ": ";
                        response += it->second;
                        response += "\r\n";
                }
                response +=  "\n";
                response +=  fileContent;
            } else {
                // TODO automate it
                std::string fileContent;
                std::ifstream ifs("./error_pages/404.html");
                if (ifs.is_open() == false) {
                    throw std::logic_error("can not open file");
                }
                std::getline(ifs, fileContent, '\0');
                headerContent["Content-Length"] = std::to_string(fileContent.size());
                response += std::to_string(404);
                response += "not found";
                response += "\r\n";

                for (std::unordered_map<std::string, std::string>::iterator it = headerContent.begin();
                    it != headerContent.end(); ++it) {
                        response += it->first;
                        response += ": ";
                        response += it->second;
                        response += "\r\n";
                }
                response +=  "\n";
                response +=  fileContent;
            }
            return (response);
        };
        std::string post(const std::string &fileName, const std::string &body) {
            std::string response;
            std::ofstream ofs(fileName);

            response += "HTTP/1.1 ";
            response += "200 ";
            response += "OK";
            // int fd = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, 0664);
            // std::cout << body << std::endl;
            ofs << body;
            return (response);
        };

        std::string del(const std::string &fileName) {
            std::string response;
            response += "HTTP/1.1 ";
            response += "200 ";
            response += "OK";
            std::remove(fileName.c_str());
            return (response);
        };

    private:
        std::string _ipAddress;
        int _port;
        int _serverSockets[_serverLimit];
};