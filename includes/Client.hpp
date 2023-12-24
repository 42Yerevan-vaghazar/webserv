#ifndef CLIENT_HPP
#define CLIENT_HPP
#include "Libs.hpp"
#include "HTTPRequest.hpp"
#include "ServerManager.hpp"
#include <ctime>
#include "HelperFunctions.hpp"
#include <signal.h>
#include "EvManager.hpp"
#include "InnerFd.hpp"

#define LAST_SENN_RIMEOUT 15 // sec

class InnerFd;

class HTTPServer;
class Client : public HTTPRequest, public HTTPResponse
{
    public:
        Client(sock_t clfd, sock_t srfd, HTTPServer &srv);
        ~Client();
    public:
        void setSocketAddress(struct sockaddr_in *addr);
        const struct sockaddr_in* getSocketAddress( void ) const;
        char* inet_ntoa(struct in_addr) const;
        std::string getUser(std::string const &pwd) const;
        sock_t getFd( void ) const;
        sock_t getServerFd( void ) const;
        std::string getServerPort( void ) const;
        int receiveRequest();
        void parseHeader();
        void parseBody();
        bool sendResponse();
        void setResponseLine(std::string const &);
        const HTTPServer &getSrv( void ) const;
        HTTPServer &getSrv( void );
        HTTPServer &getDefaultSrv( void );
        void setCgiStartTime();
        bool checkCgi();
        void setCgiPipeFd(int fd);
        void setCgiPID(int fd);
        InnerFd *getInnerFd(int fd);
        void addInnerFd(InnerFd *);
        void removeInnerFd(int fd);
        const ServerCore &getCurrentLoc() const;
    private:
        void readChunkedRequest();
        std::map<int, InnerFd *> _innerFds;                   // [Clients inner fds]
        sock_t _fd;
        sock_t serverFd;
        HTTPServer &_defaultSrv;
        HTTPServer *_subSrv;
        std::string _responseLine;
        std::time_t	 _lastSeen;
        std::time_t	 _cgiStartTime;
        int _cgiPipeFd;
        int _cgiPID;
        struct sockaddr_in clientInfo;
};

#endif