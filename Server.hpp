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

#include <vector>
#include <sstream>

#define PROTOCOL "HTTP/1.1"

const int _clientLimit = 1000;
const int _serverLimit = 1000;


// std::string appendFile(const std::string &str, const std::string &file) {

// }

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

                char buf[2000];

                if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
                    perror("recv :");
                    exit(1);
                }
                // TODO parser should be parser here
                std::cout << buf << std::endl;
                std::string strBuf(buf);
                size_t pos = strBuf.find(' ');
                pos += 1;
                if (buf[0] == 'P') {
                    char buf[5000];
                    std::cout << "stex" << std::endl;
                    // read(0, buf, sizeof(buf));
                    if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
                        perror("recv :");
                        exit(1);
                    }
                    std::cout << buf << std::endl;
                    std::string response = post(strBuf.substr(pos, strBuf.find(' ', pos) - pos), buf);
                    if (send(clientSocket, response.c_str(), strlen(response.c_str()), 0) == -1) {
                        perror("send :");
                        exit(1);
                    }
                } else if (buf[0] == 'G') {
                    std::string response;
                    std::string filePath = strBuf.substr(pos, strBuf.find(' ', pos) - pos);
                    filePath = "." + filePath;
                    size_t contentTypePos = filePath.rfind(".");
                    std::string contentType = filePath.substr(contentTypePos + 1);
                    std::cout << "file = " << filePath << std::endl;
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
                } else if (buf[0] == 'D') {
                    std::string response = del(strBuf.substr(pos, strBuf.find(' ', pos) - pos));
                    if (send(clientSocket, response.c_str(), strlen(response.c_str()), 0) == -1) {
                        perror("send :");
                        exit(1);
                    }
                }
                close(clientSocket);
            }
        };

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
                std::stringstream buffer;
                std::ostringstream stream;
                std::ifstream ifs(fileName, std::ios::binary);
                if (ifs.is_open() == false) {
                    throw std::logic_error("can not open file");
                }
                stream << ifs.rdbuf();
                fileContent = stream.str();
                headerContent["Content-Length"] = std::to_string(fileContent.size());
                std::ofstream ofs("test.png");
                if (ofs.is_open() == false) {
                    perror("is_open");
                    exit(1);
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

    private:
        std::string _ipAddress;
        int _port;
        int _serverSockets[_serverLimit];
        std::vector<std::string> _data;
};