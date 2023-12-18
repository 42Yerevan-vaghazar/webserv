/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 23:56:30 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/07 18:31:31 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP
#include "Libs.hpp"
#include "Tcp.hpp"
#include "IListener.hpp"
#include "ServerCore.hpp"
#include "ServerManager.hpp"
#include "ResponseError.hpp"
#include <errno.h>

class HTTPRequest;
class ServerManager;
class Location;
class Client;
class HTTPServer : public Tcp, public IListener, public ServerCore
{
    public:
        HTTPServer( void );
        ~HTTPServer();
    public:
		 void up(ServerManager &mgn);
         sock_t getfd( void ) const;
    public:
        Client *getClient(sock_t fd);
        void removeClient(sock_t fd);
    public:
        void push(std::string const &prefix, Location locationDirective);
        void push(sock_t clFd, Client *clt);
        void push(HTTPServer srv);
        const HTTPServer &getServerByName(std::string const &serverName) const;
        int  pop(sock_t clFd);
        // void push_serverName(std::string const &srvName);
    public:
        const Location *find(std::string const &prefix) const;
        const Location* findMatching(std::string const &realPath) const;
        bool exist(sock_t fd);
        // std::vector<std::string> const &getServerNames( void ) const;
        std::map<std::string, Location> const &getLocations( void ) const;
    private:
        std::map<std::string, HTTPServer> _srvs;
        // std::vector<std::string> ServerName;
        std::map<sock_t, Client *> clnt;                   // [Clients]
        std::map<std::string, Location> locations;      // <prefix, LocationDirective>  location / {Location}
    public: //ip port interface
		virtual void setPort(std::string const &port);
		virtual void setIp(std::string const &ipv);
		virtual const char* getIp( void ) const;
		virtual const char* getPort( void ) const;
        virtual uint32_t getNIp( void ) const;
		virtual uint16_t getNPort( void ) const;
        std::string	directory_listing(const std::string &path, std::string displayPath);
    public:
        bool operator==(HTTPServer const &);
        bool operator==(sock_t);
    public:
        std::string get(Client &client);
        std::string post(Client &client);
        std::string del(Client &client);
        std::string processing(Client &client);

        std::string executeCgi(Client &client);
    private:
        std::map<std::string, std::string (HTTPServer::*)(Client&)> methodsMap;
    public:
};

#endif