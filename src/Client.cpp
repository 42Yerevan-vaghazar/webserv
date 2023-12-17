/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 10:29:55 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/12 21:55:22 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(sock_t clfd, sock_t srfd, HTTPServer &srv) : _srv(srv)
{
    this->fd = clfd;
    this->serverFd = srfd;
    this->rd = 0;
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
    return (_srv.getPort());
};

int Client::receiveRequest() {
    char buf[READ_BUFFER];
    errno = 0;
    int rdSize = recv(fd, buf, sizeof(buf), 0);
    if (rdSize == -1) { // TODO Checking the value of errno is strictly forbidden after a read or a write operation.
    // TODO add time information for client and throw request timout error response 408 Request Timeout
        return (0);
    }
    // std::cout << "rdSize = " << rdSize << std::endl;
    if (rdSize == 0) {
        return (-1);
    }
    if (_isHeaderReady == false) {
        httpRequest.append(buf, rdSize);
        size_t headerEndPos = httpRequest.find("\n\r\n");
        if (headerEndPos == std::string::npos) {
            return 0;
        }
        _isHeaderReady = true;
        size_t pos = httpRequest.find("Content-Length: ");
        if (pos == std::string::npos) {
            _bodySize = 0;
        } else {
            _bodySize = std::stoi(httpRequest.substr(httpRequest.find("Content-Length: ") + strlen("Content-Length: "), 10));  // TODO throw 413 if the bodt size of payload is bigger then limits predefined configs;
        }

        std::string tmpBody = httpRequest.substr(headerEndPos + 3);
        httpRequest.erase(headerEndPos);
        if (_bodySize != 0) {
            _body = tmpBody;
            if (_bodySize <= _body.size()) {
                _body.erase(_bodySize);
                _isBodyReady = true;
                _isRequestReady = true;
            }
            
        } else {
            _isBodyReady = true;
            _isRequestReady = true;
        }
        return 0;
    }
    _body.append(buf, rdSize);
    if (_bodySize <= _body.size()) {
        _body.erase(_bodySize);
        _isBodyReady = true;
        _isRequestReady = true;
    }
    return 0;
}

void Client::parse()
{
    size_t space = 0;
    size_t pos = httpRequest.find("\r\n");
    request = httpRequest.substr(0, pos);
    // std::cout << "request = " << request << std::endl;
    httpRequest.erase(0, pos + 2);

    for (size_t i = 0; i < request.size(); i++)
        if (std::isspace(request[i]))
            space++;
    if (space == 2)
    {
        method = trim(request.substr(0, request.find_first_of(" ")));
        request.erase(0, request.find_first_of(" ") + 1);
        _path = trim(request.substr(0, request.find_first_of(" ")));// TODO handle ? var cases in _path
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
    std::cout << "method = " << method << std::endl;
    if (method == "POST") {
        multipart();
    }
    
    HTTPRequest::checkPath(this->_srv);
    //    std::cout << "method = " << method << std::endl;
    // std::cout << "_path = " << _path << std::endl;
    // std::cout << "version = " << version << std::endl;
    // for (std::map<std::string, std::string>::iterator it = httpHeaders.begin(); it !=  httpHeaders.end(); ++it)
    // {
    //     std::cout << "key = " << it->first << ", val = " << it->second << std::endl;
    // }
}

bool Client::sendResponse() {
    size_t sendSize = WRITE_BUFFER < _response.size() ? WRITE_BUFFER : _response.size();

    if (_response.empty() == true) {
        if (_isCgi == true) {
            char buf[sendSize];
            int rfd = read(_cgiPipeFd, buf, sendSize);
            if (rfd == -1) {
                return (0);
            }
            if (rfd == 0) {
                _cgiPipeFd = -1;
            }
            buf[rfd] = '\0';
            _response = buf;
        }
        _isResponseReady = false;
    }

    if (send(fd, _response.c_str(), sendSize, 0) == -1) {
        return (false); // TODO is send function return -1 seting EAGAIN in errno
    }
    _response.erase(0, sendSize);
    // _response.clear();
    return (_response.empty() && _cgiPipeFd == -1);
}

void Client::setResponse(const std::string &response) {
    _response = response;
    _isResponseReady = true;
}

const HTTPServer &Client::getSrv( void ) const {
    return (_srv);
};
