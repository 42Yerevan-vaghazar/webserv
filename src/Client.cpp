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

Client::Client(sock_t clfd, sock_t srfd, HTTPServer &srv) : _defaultSrv(srv)
{
    this->fd = clfd;
    this->serverFd = srfd;
    this->rd = 0;
    _subSrv = NULL;
    _cgiPipeFd = -1;
    _cgiPID = -1;
}

Client::~Client()
{
}

sock_t Client::getFd( void ) const
{
    return (this->fd);
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
    int rdSize = recv(fd, buf, sizeof(buf), 0);
    if (rdSize == -1) { 
        _isBodyReady = true;
        _isRequestReady = true;
        return (0);
    }
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
        // this->showHeaders();
        std::map<std::string, std::string>::const_iterator it = httpHeaders.find("Content-Length");
        if (it != httpHeaders.end()) {
            char *ptr;
            _bodySize = std::strtoul(it->second.c_str(), &ptr, 10);
            if (_bodySize > this->getSrv().getClientBodySize()) {
                throw ResponseError(413, "Content Too Large");
            }
        }
        it = httpHeaders.find("Transfer-Encoding");
        if (it != httpHeaders.end() && (it->second.find("Chunked") != std::string::npos ||  it->second.find("chunked") != std::string::npos)) {
            if (it->second.find("Chunked") != std::string::npos ||  it->second.find("chunked") != std::string::npos) {
                _isChunked = true;
                std::cout << "chunked = true " << std::endl;
            } else {
                throw ResponseError(400, "Bad Request");
            }
        }
        return (0);
    } 
    if (_isChunked) {
        readChunkedRequest();
        return (0);
    }      
    else {
        _body.append(_requestBuf.c_str(), _requestBuf.size());
        _requestBuf.clear();
        if (_bodySize <= _body.size()) {
            _body.erase(_bodySize);
            _isBodyReady = true;
            _isRequestReady = true;
        }
    }
    return 0;
}
// echo -ne '6\r\nHello,\r\n6\r\nworld!\r\n\r\n' | curl -X POST http://localhost:3000/chunked-H "Transfer-Encoding: chunked" --data @-
// curl -H "Transfer-Encoding: chunked" http://localhost:3000/--data --data-binary "This is the second chunk." --data-binary "And this is the third chunk."
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
            httpHeaders.insert(std::make_pair(key, value));
        }
    }
    httpRequest.clear();
    std::map<std::string, std::string>::iterator it = httpHeaders.find("Host");
    if (it != httpHeaders.end()) {
        _subSrv = _defaultSrv.getSubServerByName(it->second);
    }
    HTTPRequest::checkPath(this->getSrv());
}

void Client::parseBody()
{
    if (method == "POST") {
        if (_isCgi == true) {
            
        }
        multipart();
    }
}

bool Client::sendResponse() {
    if (_responseLine.empty() == false) {
        size_t sendSize = WRITE_BUFFER < _responseLine.size() ? WRITE_BUFFER : _responseLine.size();
        if (send(fd, _responseLine.c_str(), sendSize, 0) == -1) {
            return (false);
        }
        _responseLine.erase(0, sendSize);

    }
    else if (_header.empty() == false) {
        size_t sendSize = WRITE_BUFFER < _header.size() ? WRITE_BUFFER : _header.size();
        if (send(fd, _header.c_str(), sendSize, 0) == -1) {
            return (false);
        }
        _header.erase(0, sendSize);
    } else if (_responseBody.empty() == false) {
        size_t sendSize = WRITE_BUFFER < _responseBody.size() ? WRITE_BUFFER : _responseBody.size();
        if (send(fd, _responseBody.c_str(), sendSize, 0) == -1) {
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

HTTPServer &Client::getSrv( void ) {
    if (_subSrv) {
        return (*_subSrv);
    }
    return (_defaultSrv);
};

void Client::setResponseLine(std::string const &line) {
    _responseLine = line;
};

void Client::setCgiStartTime() {
    _cgiStartTime = get_current_time();
};

bool Client::checkCgi() {
    if (_cgiPID != -1 && get_current_time() - _cgiStartTime > CGI_TIMEOUT) {
        _isCgi = false;
        int status;
        kill(_cgiPID, SIGKILL);
        waitpid(_cgiPID, &status, 0);
        std::cout << "status = " << status << std::endl;
        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                std::cout << "WEXITSTATUS(status) = " << WEXITSTATUS(status) << std::endl;
                throw ResponseError(500, "Internal Server Error");
            }
        }
        if (WIFSIGNALED(status)) {
            if (WTERMSIG(status) == SIGKILL) {
                throw ResponseError(508, "Loop Detected");
            }
            throw ResponseError(500, "Internal Server Error");
        }
        EvManager::addEvent(_cgiPipeFd, EvManager::read);
        this->getSrv().addInnerFd(new InnerFd(_cgiPipeFd, *this, this->getResponseBody(), EvManager::read));
    }
    return (true);
};


void Client::setCgiPipeFd(int fd) {
    _cgiPipeFd = fd;
};

void Client::setCgiPID(int pid) {
    _cgiPID = pid;
};