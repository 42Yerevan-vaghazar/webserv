/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 23:52:27 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/07 18:34:31 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_MANAGER
#define SERVER_MANAGER
#include "Libs.hpp"
#include "Parser.hpp"
#include "Location.hpp"
#include "HTTPServer.hpp"
#include "Client.hpp"
#include <ResponseError.hpp>
#define SUCCSSES_STATUS "OK"

class Client;
class HTTPServer;
class Error;

class ServerManager : public std::vector<HTTPServer *>
{
    public:
        ServerManager(const char *configfile);
        ~ServerManager();
    public:
        // int isServer(sock_t fd);
        // int isClient(sock_t fd);
    public:
        void printFds( void );
    public:
        HTTPServer  *getServerBySocket(sock_t fd); 
        HTTPServer  *getServerByClientSocket(sock_t fd);  //Վերադարձնումա են սերվերը որի մեջ fd-ով client գոյություն ունի
        sock_t getmax( void ) const;
        int used(HTTPServer &srv) const;
    public:
        // void push(HTTPServer const &srv);
        std::vector<HTTPServer> getVirtualServers( void );
    private:
        // std::vector<HTTPServer> _srvs;
    public:
        // std::vector<Client> clnt;
    public:
        void start();
        void generateResponse(Client &client);
        void generateErrorResponse(const ResponseError& e, Client &client);
    private:
        bool closeConnetcion(sock_t fd);
        bool newClient(int fd);
};

#endif
