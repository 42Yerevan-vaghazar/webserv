/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 23:56:30 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/28 20:31:48 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP
#include "Libs.hpp"
#include "Tcp.hpp"
#include "core.hpp"
#include "ServerCore.hpp"
#include "ServerManager.hpp"

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
        void push(sock_t clFd, Client &clt);
        int  pop(sock_t clFd);
        void push_serverName(std::string const &srvName);
    public:
        const Location *find(std::string const &prefix) const;
        const Location* findMatching(std::string const &realPath) const;
        bool exist(sock_t fd);
        std::vector<std::string> const &getServerNames( void ) const;
    public:
        void request(Client &cl);
    private:
        std::vector<std::string> ServerName;
        std::map<sock_t, Client> clnt;                   // [Clients]
        std::map<std::string, Location> locations;      // <prefix, LocationDirective>  location / {Location}
    public: //ip port interface
		virtual void setPort(std::string const &port);
		virtual void setIp(std::string const &ipv);
		virtual const char* getIp( void ) const;
		virtual const char* getPort( void ) const;
        virtual uint32_t getNIp( void ) const;
		virtual uint16_t getNPort( void ) const;
    public:
        bool operator==(HTTPServer const &);
};

#endif