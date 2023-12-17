/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 10:29:10 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/07 21:14:20 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Libs.hpp"
#include "HTTPRequest.hpp"
#include "ServerManager.hpp"

class HTTPServer;
class Client : public HTTPRequest, public HTTPResponse
{
    public:
        // Client( void );
        Client(sock_t clfd, sock_t srfd, HTTPServer &srv);
        // Client(sock_t clfd);
        ~Client();
    public:
        // void processing(HTTPServer &srv);
    public:
        sock_t getFd( void ) const;
        sock_t getServerFd( void ) const;
        std::string getServerPort( void ) const;
        // const HTTPRequest &getRequest() const;
        // HTTPResponse &getResponse();
        int receiveRequest();
        void parse();
        bool sendResponse();
        void setResponse(const std::string &response);
        const HTTPServer &getSrv( void ) const;
    private:
        int rd;
        sock_t fd;
        sock_t serverFd;
        // std::string _httpRequest;
        // std::string _body;
        std::string _response;
        HTTPServer &_srv;
        // HTTPRequest _request;
        // HTTPResponse _response;
    private:
        struct sockaddr_in ClientInfo;
        struct sockaddr ClientAddress;
        struct sockaddr_storage addressStorage;
};

#endif