/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 23:57:39 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/28 21:18:04 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"

size_t longestMatch(std::string const &s1, std::string const &s2);

HTTPServer::HTTPServer( void )
{
    //defualt initializations
    this->port = DEFAULT_HTTP_PORT;
    this->ip = DEFAULT_MASK;
}

HTTPServer::~HTTPServer()
{
    
}

void HTTPServer::setPort(std::string const &port)
{
    if (port.size() > 5)
        throw HTTPCoreException("Port: is to long");
    for(size_t i = 0; i < port.size(); i++)
        if (!std::isdigit(port[i]))
            throw HTTPCoreException("Port: Non digit character");
    this->n_port = (uint16_t)std::atoi(port.c_str());
    this->port = port;
}

const char* HTTPServer::getPort( void ) const
{
    return (port.c_str());
}

const char* HTTPServer::getIp( void ) const
{
    return (this->ip.c_str());
}

uint16_t HTTPServer::getNPort( void ) const
{
    return (this->n_port);
}

uint32_t HTTPServer::getNIp( void ) const
{
    return (this->n_ip);
}

void HTTPServer::setIp(std::string const &ipv)
{
    size_t n = 0;
    size_t k = 0;
    std::string octet;
    
    if (ipv.size() > 15)
        throw HTTPCoreException("Ip: Syntax Error");
    for(size_t i = 0; i <= ipv.size(); i++)
    {
        if (std::isdigit(ipv[i]))
        {
            n++;
            octet += ipv[i];
        }
        else if (ipv[i] == '.' || i >= ipv.size())
        {
            if (ipv[i] == '.')
                k++;
            if (n > 3 || std::atoi(octet.c_str()) > 255)
                throw HTTPCoreException("Octet: > 255");
            octet.clear();
            n = 0;
        }
        else
            throw HTTPCoreException("Ip: Syntax Error");

    }
    if (k != 3)
        throw HTTPCoreException("Dots: syntax error");
    this->ip = ipv;
}

void HTTPServer::push(std::string const &prefix, Location locationDirective)
{
    this->locations.insert(std::make_pair(prefix, locationDirective));
}


void HTTPServer::push_serverName(std::string const &srvName)
{
    std::vector<std::string>::iterator it = std::find(ServerName.begin(), ServerName.end(), srvName);
    if (it == ServerName.end())
        ServerName.push_back(srvName);
}

const Location* HTTPServer::find(std::string const &prefix) const
{
    std::string path = prefix;
    size_t sl = HTTPRequest::slashes(path);
    for(size_t i = 0; i <= sl; i++)
    {
        std::map<std::string, Location>::const_iterator route = locations.find(path);
        if (route != locations.end())
            return (&route->second);
        path = path.substr(0, path.find_last_of("/"));
    }
    return (NULL);
}




std::vector<std::string> const &HTTPServer::getServerNames( void ) const
{
    return (ServerName);
}

sock_t HTTPServer::getfd( void ) const
{
    return (this->fd);
}

void HTTPServer::up(ServerManager &mgn)
{
    if (!mgn.used(this))
    {
        const char* givenIp = ip.c_str();
        const char* givenPort = port.c_str();
        Tcp::setup(givenIp, givenPort);
        Tcp::createSocket();
        Tcp::bindSocket();
        Tcp::listenSocket();
        mgn.setmax(fd);
        std::cout << givenIp <<  ":" << givenPort << std::endl;
        freeaddrinfo(addrList);
    }
    else
        std::cout << "{Already:used}" << std::endl;
    
}

void HTTPServer::push(sock_t clFd, Client &clt)
{
    clnt.insert(std::make_pair(clFd, clt));
}

int HTTPServer::pop(sock_t clFd)
{
    std::map<sock_t, Client>::iterator it = clnt.find(clFd);
    if (it != clnt.end())
    {
        clnt.erase(it);
        return (0);
    }
    return (-1);
}

bool HTTPServer::exist(sock_t fd)
{
    return (clnt.find(fd) != clnt.end());
}


bool HTTPServer::operator==(HTTPServer const &sibling)
{
    if (std::strcmp(this->getIp(), sibling.getIp()) == 0 \
        && std::strcmp(this->getPort(),sibling.getPort()) == 0)
      return (true);  
    return (false);
}

Client* HTTPServer::getClient(sock_t fd)
{
    std::map<sock_t, Client>::iterator it = clnt.find(fd);
    if (it != clnt.end())
        return (&it->second);
    return (NULL);
}

void HTTPServer::removeClient(sock_t fd)
{
    std::map<sock_t, Client>::iterator it = clnt.find(fd);
    if (it != clnt.end())
        clnt.erase(it);
    return ;
}

const Location* HTTPServer::findMatching(std::string const &realPath) const
{
    std::map<std::string, Location>::const_iterator loc;
    std::map<std::string, Location>::const_iterator match;
    size_t longestMatchSize = 0;
    size_t currentMatch = 0;
    for(loc = locations.begin(); loc != locations.end(); loc++)
    {
        currentMatch = longestMatch(loc->first, realPath);
        if (longestMatchSize < currentMatch)
        {
            match = loc;
            longestMatchSize = currentMatch;
        }
        else if (longestMatchSize == currentMatch)
        {
            if (match->first < loc->first)
                match = loc;
        }
    }
    return (&match->second);
}

size_t longestMatch(std::string const &s1, std::string const &s2)
{
    size_t match = 0;
    for(size_t i = 0; i < s1.size(); i++)
    {
        if (s1[i] != s2[i])
            break;
        match++;
    }
    return (match);
}




//ServerCore////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
/***************************************************************** */

// int HTTPServer::getClientBodySize( void )
// {
// 	return (client_body_size);
// }

// void HTTPServer::pushIndex(std::string const &fileNameExample)
// {
// 	index.push_back(fileNameExample);
// }

// void HTTPServer::pushMethods(std::string const &method)
// {
// 	std::string capitalized = method;
// 	for(size_t i = 0; i < method.size(); i++)
// 		capitalized[i] = std::toupper(capitalized[i]);
// 	methods.push_back(capitalized);
// }

// std::vector<std::string> HTTPServer::getIndexFiles( void ) const
// {
// 	return (this->index);
// }

// std::vector<std::string> HTTPServer::getMethods( void ) const
// {
// 	return (this->methods);
// }

// void HTTPServer::setRoot(std::string const &root)
// {
// 	this->root = root;
// }

// std::string const &HTTPServer::getRoot( void ) const
// {
// 	return (this->root);
// }

// const char* HTTPServer::findIndex(std::string const &filename) const
// {
// 	size_t i = 0;
// 	for(; i < index.size(); i++)
// 		if (index[i] == filename)
// 			return (index[i].c_str());
// 	return (NULL);
// }

// const char* HTTPServer::findMethod(std::string const &method) const
// {
// 	size_t i = 0;
// 	for(; i < methods.size(); i++)
// 		if (methods[i] == method)
// 			return (methods[i].c_str());
// 	return (NULL);
// }

// const char* HTTPServer::findErrorPage(std::string const &status_code)
// {
// 	std::map<std::string, std::string>::iterator it = error_page.find(status_code);
// 	if (it != error_page.end())
// 		return (it->second.c_str());
// 	return (NULL);
// }

// void HTTPServer::setAutoindex(std::string const &sw)
// {
// 	(sw == "on") ? this->autoindex = 1 : this->autoindex = 0;
// }

// void HTTPServer::pushErrPage(std::string const &key, std::string const &errpage_filename)
// {
// 	error_page.insert(std::make_pair(key, errpage_filename));
// }

// void HTTPServer::setSize(std::string const &bodySize)
// {
// 	unsigned long long int toLong = std::strtoull(bodySize.c_str(), NULL, 10);
// 	if (errno == ERANGE && toLong == ULLONG_MAX)
// 		this->client_body_size = 200;
// 	else
// 		this->client_body_size = toLong * 1048576 / 1;
// }

// bool HTTPServer::getAutoindex( void ) const
// {
// 	return (autoindex);
// }