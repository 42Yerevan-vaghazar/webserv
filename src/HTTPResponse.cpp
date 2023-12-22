/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 01:28:07 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/12 21:56:56 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse( void )
{
    _responseHeader["server"] = "webserv";
    _isResponseReady = false;
    _isStarted = false;
}

HTTPResponse::~HTTPResponse()
{
    
}

// std::string HTTPResponse::file(std::string const &filename)
// {
//     std::fstream f;
//     std::string tmp;
//     std::string content;
//     f.open(filename.c_str());
//     if (f.is_open())
//     {
//         while (std::getline(f, tmp, '\n'))
//         {
//             content += tmp;
//             tmp.clear();
//         }
//     }
//     return (content);
// }

std::string const HTTPResponse::getResponse( void ) const
{
    return (_header + _responseBody);
}

std::unordered_map<std::string, std::string> &HTTPResponse::getResponseHeader() {
    return (_responseHeader);
}

void HTTPResponse::addHeader(const std::pair<std::string, std::string> &pair) {
    _responseHeader[pair.first] = pair.second;
}


void HTTPResponse::buildHeader() {
    for (std::unordered_map<std::string, std::string>::iterator it = _responseHeader.begin();
        it != _responseHeader.end(); ++it) {
            _header += it->first;
            _header += ": ";
            _header += it->second;
            _header += "\r\n";
    }
    _header += "\r\n";

}

std::string &HTTPResponse::getResponseBody() {
    return (_responseBody);
};

void HTTPResponse::setBody(const std::string &body) {
    _responseBody = body;
    _isResponseReady = true;
}

bool HTTPResponse::isResponseReady() const {
    return (_isResponseReady);
}

bool &HTTPResponse::isResponseReady() {
    return (_isResponseReady);
}

bool HTTPResponse::isStarted() const {
    return (_isStarted);
}

void HTTPResponse::setStartStatus(bool is) {
    _isStarted = is;
}