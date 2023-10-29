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

#include <fcntl.h>

#include <set>
#include <vector>
#include <sstream>

#include	<sys/types.h>
#include	<sys/event.h>
#include	<sys/time.h>


#include "Client.hpp"
#include "EvManager.hpp"

#define PROTOCOL "HTTP/1.1"

#include "DefaultSetup.hpp"
const int HEADER_SIZE = 1000;
const int CLIENT_LIMIT = 1000;
const int SERVER_LIMIT = 1000;
const int MAX_MSG_SIZE = 10000;


class Server
{
    public:
        Server(const std::string &ipAddress, int port)
            : _port(port), _ipAddress(ipAddress) {};
        ~Server() {};
        bool start() {
            int a = 1;
            _serverSocket = socket(AF_INET, SOCK_STREAM, 0);

            if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0) {
                std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
            }
            // fcntl(_serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
            struct sockaddr_in addr; // TODO sockaddr_storage
            memset(&addr , 0 , sizeof(struct sockaddr_in));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(_ipAddress.c_str());
            addr.sin_port = htons(_port);
            std::cout << "addr.sin_port = " << addr.sin_port << std::endl;
            std::cout << "addr.sin_addr.s_addr = " << addr.sin_addr.s_addr << std::endl;

            if (bind(_serverSocket, (sockaddr *)&addr, sizeof(addr)) == -1) {
                std::runtime_error(std::string("bind: ") + strerror(errno));
            }

            if (listen(_serverSocket, 0) == -1) {
                std::runtime_error(std::string("listen: ") + strerror(errno));
            };
            EvManager::start();
            EvManager::addEvent(_serverSocket, EvManager::read);
            while(true) {
                std::pair<int, int> event = EvManager::listen();
                if (event.second == _serverSocket) {
                    std::cout << "\n_serverSocket\n" << std::endl;
                    Client client;
                    int clientSocket = accept(_serverSocket, &client.getAddr(), &client.getAddrLen());
                    std::cout << "clientSocket = "  << clientSocket << std::endl;
                    if (clientSocket == -1) {
                        throw std::runtime_error(std::string("accept: ") + strerror(errno));
                    }
                    client.setFd(clientSocket);
                    // if (!_clients.find(client.getAddr())) {
                    _clients.insert(client);
                    EvManager::addEvent(clientSocket, EvManager::read);
                    // }
                } else if (event.first == EvManager::eof) {
                    std::cout << "\nEV_EOF\n" << std::endl;
                    EvManager::delEvent(event.second, event.first);
                } else if (event.first == EvManager::read) {
                    std::cout << "\nEVFILT_READ\n" << std::endl;
                    // std::cout << "\evList[i].ident = " << evList[i].ident << std::endl;
                    std::cout << "event.first = " << event.first << std::endl;
                    std::cout << "event.second = " << event.second << std::endl;
                    std::string request = receiveMessage(event.second);
                    // std::cout << "request = " << request << std::endl;
                    size_t pos = request.find(' ');
                    pos += 1;
                    // parse request

                    sendMessage(event.second, request, pos);
                }
            }
        };

        // void run_event_loop(int kq, int fd) {

        // }

        std::string get(const std::string &fileName, const std::string  &contentType) {
            std::unordered_map<std::string, std::string> headerContent;
            std::string response;
            int statusCode = 200;
            std::string status = " OK";
            std::cout << "contentType = " <<  contentType << std::endl;
            if (contentType == "png"){
                headerContent["Content-Type"] = "image/png";
            } else {
                headerContent["Content-Type"] = "text/html";
            }
            response += "HTTP/1.1 ";
            std::cout << "fileName = " << fileName << std::endl;
            if (access(fileName.c_str(), F_OK) == 0) {   // TODO check permission to read
                std::string fileContent;
                std::ostringstream stream;
                std::ifstream ifs(fileName);
                if (ifs.is_open() == false) {
                    throw std::logic_error("can not open file");
                }
                stream << ifs.rdbuf();
                fileContent = stream.str();
                headerContent["Content-Length"] = std::to_string(fileContent.size());
                std::ofstream ofs("test.png");
                if (ofs.is_open() == false) {
                    throw std::logic_error("Error opening file");
                }
                ofs << fileContent;
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
            response += PROTOCOL;
            response += " 200 ";
            response += "OK";
            _data.push_back(body);
            // ofs << body;
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

        void sendMessage(int clientSocket, const std::string &request, int pos) {
            if (request[0] == 'P') {
                char buf[5000];
                if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
                    perror("recv :");
                    exit(1);
                }
                std::cout << buf << std::endl;
                std::string response = post(request.substr(pos, request.find(' ', pos) - pos), buf);
                if (send(clientSocket, response.c_str(), strlen(response.c_str()), 0) == -1) {
                    perror("send :");
                    exit(1);
                }
            } else if (request[0] == 'G') {
                std::string response;
                std::string filePath = request.substr(pos, request.find(' ', pos) - pos);
                filePath = "." + filePath;
                size_t contentTypePos = filePath.rfind(".");
                std::string contentType = filePath.substr(contentTypePos + 1);
                if (filePath == "./") {
                    response = get("index.html", contentType);
                } else {
                    response = get(filePath, contentType);
                }
                if (send(clientSocket, response.c_str(), response.size(), 0) == -1)
                {
                    perror("send :");
                    exit(1);
                }
            } else if (request[0] == 'D') {
                std::string response = del(request.substr(pos, request.find(' ', pos) - pos));
                if (send(clientSocket, response.c_str(), strlen(response.c_str()), 0) == -1) {
                    perror("send :");
                    exit(1);
                }
            }
        }

        std::string receiveMessage(int fd) {
            char buf[MAX_MSG_SIZE];
            int rdSize = recv(fd, buf, sizeof(buf), 0);

            if (rdSize == -1) {
                perror("recv");
                exit(1);
            }
            std::cout << "rdSize = " << rdSize << std::endl;
            buf[rdSize] = '\0';
            return (buf);
        }

    private:
        std::string _ipAddress;
        int _port;
        int _serverSocket;
        int _clientSockets[CLIENT_LIMIT];
        std::vector<std::string> _data;
        std::set<Client> _clients;
};