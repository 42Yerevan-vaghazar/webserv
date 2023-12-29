/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 10:29:55 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/21 01:45:16 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "ResponseError.hpp"
#include "InnerFd.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include "Cgi.hpp"

Client::Client(sock_t clfd, sock_t srfd, HTTPServer &srv) : _defaultSrv(srv)
{
    this->_fd = clfd;
    this->serverFd = srfd;
    _subSrv = NULL;
    _cgiPipeFd = -1;
    _cgiPID = -1;
    _lastSeen = time(NULL);
}

Client::~Client()
{
    std::map<int, InnerFd *>::iterator it = _innerFds.begin();
    while (it != _innerFds.end()) {
        delete it->second;
        EvManager::delEvent(it->first, EvManager::read);
        EvManager::delEvent(it->first, EvManager::write);
        ++it;
    }
}

sock_t Client::getFd( void ) const
{
    return (this->_fd);
}

sock_t Client::getServerFd( void ) const
{
    return (this->serverFd);
}

std::string Client::getServerPort( void ) const {
    return (_defaultSrv.getPort());
};

void Client::readChunkedRequest() {
    char *ptr;
    if (_isChunkNewLineCuted == false) {
        size_t pos = _requestBuf.find("\r\n");
        if (pos != std::string::npos && pos == 0) {
            _requestBuf.erase(0, strlen("\r\n"));
            _isChunkNewLineCuted = true;
        }
    }
    if (_chunkSize == std::string::npos && _requestBuf.find("\r\n") != std::string::npos && _isChunkNewLineCuted == true) {
        _chunkSize =  std::strtoul(_requestBuf.c_str(), &ptr, 16);
        if (_chunkSize == 0) {
            _isBodyReady = true;
            _isRequestReady = true;
            return ;
        }
        size_t posEndl = _requestBuf.find("\r\n");
        _requestBuf.erase(0, posEndl + strlen("\r\n"));
    }
    size_t existChunkSize = _chunkSize < _requestBuf.size() ? _chunkSize : _requestBuf.size();
    _body.append(_requestBuf.c_str(), existChunkSize);
    if (existChunkSize == _chunkSize) {
        _isChunkNewLineCuted = false;
        _chunkSize = std::string::npos;
    }
}

int Client::receiveRequest() {
    char buf[READ_BUFFER];
    errno = 0;
    int rdSize = recv(_fd, buf, sizeof(buf), 0);
    if (rdSize == -1) { 
        if (time(NULL) - _lastSeen == LAST_SENN_RIMEOUT) {
            return (-1);
        }
        return (0);
    }
    _lastSeen = time(NULL);
    if (rdSize == 0) {
        return (-1);
    }
    _requestBuf.append(buf, rdSize);
    if (_isHeaderReady == false) {
        size_t headerEndPos = _requestBuf.find("\r\n\r\n");
        if (headerEndPos == std::string::npos) {
            return 0;
        }
        _isHeaderReady = true;

        httpRequest  = _requestBuf.substr(0, headerEndPos);
        _requestBuf.erase(0, headerEndPos + strlen("\r\n\r\n"));
        this->parseHeader();
        std::map<std::string, std::string>::const_iterator it = _httpHeaders.find("Content-Length");
        if (it != _httpHeaders.end()) {
            char *ptr;
            _bodySize = std::strtoul(it->second.c_str(), &ptr, 10);
            if (_bodySize > this->getCurrentLoc().getClientBodySize()) {
                throw ResponseError(413, "Content Too Large");
            }
        }
        it = _httpHeaders.find("Transfer-Encoding");
        if (it != _httpHeaders.end() && (it->second.find("Chunked") != std::string::npos ||  it->second.find("chunked") != std::string::npos)) {
            if (it->second.find("Chunked") != std::string::npos ||  it->second.find("chunked") != std::string::npos) {
                _isChunked = true;
            } else {
                throw ResponseError(400, "Bad Request");
            }
        }
    }
    if (_isHeaderReady == true) {
        if (_isChunked ) {
            readChunkedRequest();
            return (0);
        } else {
            if (_bodySize == 0) {
                // EvManager::delEvent(_fd, EvManager::read);
                _isBodyReady = true;
                _isRequestReady = true;
                return (0);
            }
            _body.append(_requestBuf.c_str(), _requestBuf.size());
            _requestBuf.clear();
            if (_bodySize <= _body.size()) {
                _body.erase(_bodySize);
                // EvManager::delEvent(_fd, EvManager::read);
                _isBodyReady = true;
                _isRequestReady = true;
            }
        }
    }
    return 0;
}

void Client::parseHeader()
{
    size_t space = 0;
    size_t pos = httpRequest.find("\r\n");
    request = httpRequest.substr(0, pos);
    httpRequest.erase(0, pos + 2);

    for (size_t i = 0; i < request.size(); i++)
        if (std::isspace(request[i]))
            space++;
    if (space == 2)
    {
        method = trim(request.substr(0, request.find_first_of(" ")));
        request.erase(0, request.find_first_of(" ") + 1);
        _path = trim(request.substr(0, request.find_first_of(" ")));
        request.erase(0, request.find_first_of(" ") + 1);
        version = trim(request.substr(0, request.find("\r\n")));
    }
    std::stringstream iss(httpRequest);
    std::string get_next_line;

    while (std::getline(iss, get_next_line) && get_next_line != "\r\n")
    {
        size_t colon;
        if ((colon = get_next_line.find_first_of(":")) != std::string::npos && std::isspace(get_next_line[colon+1]))
        {
            std::string key = trim(get_next_line.substr(0, colon));
            std::string value = trim(get_next_line.substr(colon+2, get_next_line.find("\r\n")));
            _httpHeaders.insert(std::make_pair(key, value));
        }
    }
    httpRequest.clear();
    std::map<std::string, std::string>::iterator it = _httpHeaders.find("Host");
    if (it != _httpHeaders.end()) {
        _subSrv = _defaultSrv.getSubServerByName(it->second);
    }
    HTTPRequest::checkPath(this->getSrv());
}

void Client::parseBody()
{
    if (method == "POST") {
        if (_isCgi == false) {
            if (this->findInMap("Content-Type").find("multipart/form-data") != std::string::npos) {
                multipart();
            }
        }
    }
}

bool Client::sendResponse() {
    if (_responseLine.empty() == false) {
        size_t sendSize = WRITE_BUFFER < _responseLine.size() ? WRITE_BUFFER : _responseLine.size();
        if (send(_fd, _responseLine.c_str(), sendSize, 0) <= 0) {
            return (false);
        }
        _responseLine.erase(0, sendSize);

    }
    else if (_header.empty() == false) {
        size_t sendSize = WRITE_BUFFER < _header.size() ? WRITE_BUFFER : _header.size();
        if (send(_fd, _header.c_str(), sendSize, 0) <= 0) {
            return (false);
        }
        _header.erase(0, sendSize);
    } else if (_responseBody.empty() == false) {
        size_t sendSize = WRITE_BUFFER < _responseBody.size() ? WRITE_BUFFER : _responseBody.size();
        if (send(_fd, _responseBody.c_str(), sendSize, 0) <= 0) {
            return (false);
        }
        _responseBody.erase(0, sendSize);
    }
    return (_responseBody.empty() && _header.empty() && _responseLine.empty());
}

const HTTPServer &Client::getSrv( void ) const {
    if (_subSrv) {
        return (*_subSrv);
    }
    return (_defaultSrv);
};

HTTPServer &Client::getDefaultSrv( void ) {
    return (_defaultSrv);
};

HTTPServer &Client::getSrv( void ) {
    if (_subSrv) {
        return (*_subSrv);
    }
    return (_defaultSrv);
};


void Client::setCgiStartTime() {
    _cgiStartTime = time(NULL);
};

bool Client::checkCgi() {
    if (_cgiPID != -1) {
        int status;
        int waitRet;
        waitRet = waitpid(_cgiPID, &status, WNOHANG);
        if (waitRet == -1) {
            throw ResponseError(500, "Internal Server Error");
        }
        if (waitRet == 0 && time(NULL) - _cgiStartTime > CGI_TIMEOUT) {
            if (kill(_cgiPID, SIGKILL) != 0) {
                throw std::runtime_error(std::string("kill: ") + strerror(errno));
            };
            waitpid(_cgiPID, &status, 0);
            _cgiPID = -1;
            if (WTERMSIG(status) == SIGKILL) {
                throw ResponseError(508, "Loop Detected");
            }
            throw ResponseError(500, "Internal Server Error");
        }
        if (waitRet != 0 && WIFEXITED(status)) {
            _cgiPID = -1;
            if (WEXITSTATUS(status) != 0) {
                std::cout << "WEXITSTATUS(status) = " << WEXITSTATUS(status) << std::endl;
                throw ResponseError(500, "Internal Server Error");
            }
            std::cout << "WEXITSTATUS(status) = " << WEXITSTATUS(status) << std::endl;
            EvManager::addEvent(_cgiPipeFd, EvManager::read);
            this->addInnerFd(new InnerFd(_cgiPipeFd, *this, this->getResponseBody(), EvManager::read));
        }
    }
    return (true);
};


void Client::setCgiPipeFd(int fd) {
    _cgiPipeFd = fd;
};

void Client::setCgiPID(int pid) {
    _cgiPID = pid;
};

const ServerCore &Client::getCurrentLoc() const {
    if (_location) {
        // std::cout << "_location = " << _location->getLocation() << std::endl;;
        return (*_location);
    }
    if (_subSrv) {
        // std::cout << "_subSrv\n";
        return (*_subSrv);
    }
        // std::cout << "_defaultSrv\n";
    return (_defaultSrv);
};


InnerFd *Client:: getInnerFd(int fd) {
    std::map<int, InnerFd *  >::iterator it = _innerFds.find(fd);
    if (it != _innerFds.end()) {
        return(it->second);
    }
    return (NULL);
};

void Client::addInnerFd(InnerFd *obj) {
    if (obj) {
        _innerFds.insert(std::pair<int, InnerFd *>(obj->_fd, obj));
    }
};

void Client::removeInnerFd(int fd) {
    std::map<int, InnerFd *>::iterator it = _innerFds.find(fd);
    if (it != _innerFds.end()) {
        delete it->second;
        _innerFds.erase(fd);
        EvManager::delEvent(fd, EvManager::read);
        EvManager::delEvent(fd, EvManager::write);
    }
};

std::string Client::getUser(std::string const &pwd) const
{
    std::string user = pwd;
    if (user.find("/home/") != std::string::npos)
    {
        user = user.substr(user.find("/home/") + 6);
        if (user.find("/") != std::string::npos)
            user = user.substr(0, user.find("/"));
    }
    return (user);
}

void Client::setSocketAddress(struct sockaddr_in *addr)
{
    this->clientInfo = *addr;
}

const struct sockaddr_in* Client::getSocketAddress( void ) const
{
    return (&this->clientInfo);
}

char* Client::inet_ntoa(struct in_addr clAddr) const
{
    static char ip[INET_ADDRSTRLEN];
    
    uint32_t ipaddr = htonl(clAddr.s_addr);

    snprintf(ip, sizeof(ip), "%u.%u.%u.%u",
        (ipaddr >> 24) & 0xFF,
        (ipaddr >> 16) & 0xFF,
        (ipaddr >> 8) & 0xFF,
        ipaddr & 0xFF
    );
    return (ip);
}