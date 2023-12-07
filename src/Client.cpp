/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 10:29:55 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/05 21:39:49 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Client::Client( void )
// {
//     this->rd = 0;
// }

Client::Client(sock_t clfd, sock_t srfd, HTTPServer &srv) : _srv(srv)
{
    this->fd = clfd;
    this->serverFd = srfd;
    this->rd = 0;
}

// Client::Client(sock_t clfd)
// {
//     this->fd = clfd;
//     this->serverFd = -1;
// }

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

// const HTTPRequest &Client::getRequest() const {
//     return (_request);
// }

// HTTPResponse &Client::getResponse() {
//     return (_response);
// };

// std::string &Client::getResponse() {
//     return (_response);
// }

int Client::receiveRequest() {
    char buf[READ_BUFFER];
    int rdSize = recv(fd, buf, sizeof(buf) - 1, 0);
    // std::cout << "rdSize = " << rdSize << std::endl;
    if (rdSize == -1) { // TODO Checking the value of errno is strictly forbidden after a read or a write operation.
        if (_maxSizeRequest == 5) { // TODO client request caused infinit loop  change with time
            return -1;
        } else {
            _maxSizeRequest++;
            return (0);
        }
    }
    if (rdSize == 0) {  // TODO close tab. send response?
        return (-1);
    }
    buf[rdSize] = '\0';
    if (_isHeaderReady == false) {
        httpRequest += buf;
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

        std::string tmp = httpRequest.substr(headerEndPos + 3);
        httpRequest.erase(headerEndPos);
        if (_bodySize != 0) {
            _body = tmp;
            std::cout << "tmp = " << tmp << std::endl;
        } else {
            _isBodyReady = true;
            _isRequestReady = true;
        }
        // TODO  parse header
        return 0;
    }
    if (_bodySize <= _body.size()) {   // TODO check body length to do so
        _body.erase(_bodySize);
        _isBodyReady = true;
        _isRequestReady = true;
        return 0;
    }
    _body += buf;
    return 0;
}

void Client::parse()
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
        path = trim(request.substr(0, request.find_first_of(" ")));// TODO handle ? var cases in path
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
    HTTPRequest::checkPath(this->_srv);
    //    std::cout << "method = " << method << std::endl;
    // std::cout << "path = " << path << std::endl;
    // std::cout << "version = " << version << std::endl;
    // for (std::map<std::string, std::string>::iterator it = httpHeaders.begin(); it !=  httpHeaders.end(); ++it)
    // {
    //     std::cout << "key = " << it->first << ", val = " << it->second << std::endl;
    // }
}

bool Client::sendResponse() {
    size_t sendSize = WRITE_BUFFER < _response.size() ? WRITE_BUFFER : _response.size();
    if (send(fd, _response.c_str(), sendSize, 0) == -1) {
        perror("send :");  //TODO shoud be removed before submission
        exit(1);
    }
    _response.erase(0, sendSize);
    // _response.clear();
    // _isResponseReady = false;
    return (_response.empty());
}

void Client::setResponse(const std::string &response) {
    _response = response;
    _isResponseReady = true;
}

void Client::appendRequest(HTTPServer &srv)
{
    size_t space = 0;
    bool reqLineFound = 0;
    bool headersFound = 0;
    http = new char[READ_BUFFER];
    while (1)
    {
        if ((rd = recv(fd, http, sizeof(READ_BUFFER), 0)) > 0)
        {
            http[rd] = '\0';
            httpRequest.append(http, rd);
            if ((reqLineEnd = httpRequest.find_first_of("\r\n")) != std::string::npos && !reqLineFound)
            {
                request = httpRequest.substr(0, reqLineEnd);
                for (size_t i = 0; i < request.size(); i++)
                    if (std::isspace(request[i]))
                        space++;
                if (space == 2)
                {
                    method = trim(request.substr(0, request.find_first_of(" ")));
                    request.erase(0, request.find_first_of(" ") + 1);
                    path = trim(request.substr(0, request.find_first_of(" ")));
                    request.erase(0, request.find_first_of(" ") + 1);
                    version = trim(request.substr(0, request.find("\r\n")));
                    reqLineFound = 1;
                    httpRequest.erase(0, reqLineEnd + 2);
                }
            }
            if ((bodyEnd = httpRequest.find("\r\n\r\n")) != std::string::npos && !headersFound)
            {
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
                httpRequest.erase(0, bodyEnd + 4);
                headersFound = 1;
            }
        }
        else
            break;
    }
    delete [] http;
    if (reqLineFound && headersFound)
    {        
        reqLineFound = false;
        headersFound = false;
        // Client::processing(srv);
    }
}

// void Client::processing(HTTPServer &srv)
// {
//     if (in(method) && version == "HTTP/1.1")
//     {
//         HTTPRequest::checkPath(srv);
//         HTTPRequest::processing(srv);
//     }
// }
