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
#include <ctime>
#include "HelperFunctions.hpp"
#include <signal.h>
#include "EvManager.hpp"

#define CGI_TIMEOUT 500 // milsec

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
        void parseHeader();
        void parseBody();
        bool sendResponse();
        void setResponseLine(std::string const &);
        const HTTPServer &getSrv( void ) const;
        HTTPServer &getSrv( void );
        void setCgiStartTime();
        bool checkCgi();
        void setCgiPipeFd(int fd);
        void setCgiPID(int fd);
    private:
        void readChunkedRequest();
        int rd;
        sock_t fd;
        sock_t serverFd;
        // std::string _httpRequest;
        HTTPServer &_defaultSrv;
        HTTPServer *_subSrv;
        // HTTPRequest _request;
        // HTTPResponse _response;
        std::string _responseLine;
        bool workingOnResponseStatus;
        double	 _lastSeen;
        double	 _cgiStartTime;
        int _cgiPipeFd;
        int _cgiPID;
    private:
        struct sockaddr_in ClientInfo;
        struct sockaddr ClientAddress;
        struct sockaddr_storage addressStorage;
};

#endif