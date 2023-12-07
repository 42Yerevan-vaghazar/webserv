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

class Client;
class HTTPServer;
class ServerManager : public std::vector<HTTPServer>
{
    public:
        ServerManager(const char *configFile);
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
        void setmax(sock_t lastfd);
        void set( void );
        void set_r(sock_t fd);
        void set_w(sock_t fd);
        void set_e(sock_t fd);
        fd_set r_set( void ) const;
        fd_set w_set( void ) const;
        fd_set e_set( void ) const;
        void rm_r(sock_t fd);
        void rm_w(sock_t fd);
        void rm_e(sock_t fd);
    private:
        sock_t max_fd;
        fd_set s_rd;
        fd_set s_wr;
        fd_set s_except;
};

#endif
