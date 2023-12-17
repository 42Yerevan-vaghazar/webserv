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
    _cgiPipeFd = -1;
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

std::string const &HTTPResponse::getResponse( void ) const
{
    return (_response);
}

std::string const &HTTPResponse::getReserve404( void ) const
{
    return (reserve);
}

std::string const &HTTPResponse::getReserve403( void ) const
{
    return (reserve);
}


std::string const &HTTPResponse::getReserve(int statusCode) const
{
    return (reserve);
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
            _response += it->first;
            _response += ": ";
            _response += it->second;
            _response += "\r\n";
    }
    _response += "\r\n";

}

void HTTPResponse::setCgiPipeFd(int fd) {
    _cgiPipeFd = fd;
};