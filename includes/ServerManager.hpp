/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 23:52:27 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/06 00:09:51 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER
#define SERVER_MANAGER
#include "Libs.hpp"
#include "Parser.hpp"
#include "Location.hpp"
#include "HTTPServer.hpp"
#include "Client.hpp"
#define SUCCSSES_STATUS "OK"

class Client;
class HTTPServer;
class ServerManager : public std::vector<HTTPServer>
{
    public:
        ServerManager(std::string const &configFile);
        ~ServerManager();
    public:
        int isServer(sock_t fd);
        int isClient(sock_t fd);
    public:
        void printFds( void );
    public:
        HTTPServer  *getServerBySocket(sock_t fd); 
        HTTPServer  *getServerByClientSocket(sock_t fd);  //Վերադարձնումա են սերվերը որի մեջ fd-ով client գոյություն ունի
        sock_t findServerBySocket(sock_t issetfd);
        sock_t findClientBySocket(sock_t issetfd);
        sock_t getmax( void ) const;
        int used(HTTPServer *srv) const;
    public:
        void push(HTTPServer const &srv);
    private:
        std::vector<HTTPServer> srvs;
    public:
        // std::vector<Client> clnt;
    public:
        void start();
        std::string generateResponse(Client &client);
    private:
        bool closeConnetcion(sock_t fd);
        bool newClient(int fd);
};

#endif
