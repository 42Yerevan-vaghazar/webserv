#pragma once
// #include "Client.hpp"

class Server
{
    public:
        Server(const std::string &ipAddress, int port) 
            : _port(port), _ipAddress(ipAddress) {};
        ~Server() {};
        bool start() {
            int a = 1;
            _serverSockets[0] = socket (AF_INET, SOCK_STREAM, 0);
            if (setsockopt(_serverSockets[0], SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0) {
                std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
            }

            struct sockaddr_in addr;
            memset(&addr , 0 , sizeof(struct sockaddr_in));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(_ipAddress);
            addr.sin_port = htons(_port);
            std::cout << "addr.sin_port = " << addr.sin_port << std::endl;
            std::cout << "addr.sin_addr.s_addr = " << addr.sin_addr.s_addr << std::endl;
            if (bind(_serverSockets[0], (sockaddr *)&addr, sizeof(addr)) == -1) {
                perror("bind: ");
            }
            // signal(SIGINT, INThandler);
            if (listen(_serverSockets[0], 0) == -1) {
                perror("listen: ");
            };
        };
        bool accept() {

            _clients new(accept(serverSocket, (sockaddr *)&addrClient, &len));
        }

        char *get();
        bool post();
        bool del();
    private:
        const int _clientLimit = 1000;
        const int _serverLimit = 1000;
        std::string _ipAddress;
        int _port;
        int _serverSockets[_serverLimit];
        std::vector<Client> _clients;
};