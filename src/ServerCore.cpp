/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 01:47:02 by dmartiro          #+#    #+#             */
/*   Updated: 2023/10/23 23:57:56 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerCore.hpp"

int ServerCore::getClientBodySize( void )
{
	return (client_body_size);
}

void ServerCore::pushIndex(std::string const &fileNameExample)
{
	index.push_back(fileNameExample);
}

void ServerCore::pushMethods(std::string const &method)
{
	std::string capitalized = method;
	for(size_t i = 0; i < method.size(); i++)
		capitalized[i] = std::toupper(capitalized[i]);
	methods.push_back(capitalized);
}

std::vector<std::string> ServerCore::getIndexFiles( void ) const
{
	return (this->index);
}

std::vector<std::string> ServerCore::getMethods( void ) const
{
	return (this->methods);
}

void ServerCore::setRoot(std::string const &root)
{
	this->root = root;
}

std::string const &ServerCore::getRoot( void ) const
{
	return (this->root);
}

const char* ServerCore::findIndex(std::string const &filename) const
{
	size_t i = 0;
	for(; i < index.size(); i++)
		if (index[i] == filename)
			return (index[i].c_str());
	return (NULL);
}

const char* ServerCore::findMethod(std::string const &method) const
{
	size_t i = 0;
	for(; i < methods.size(); i++)
		if (methods[i] == method)
			return (methods[i].c_str());
	return (NULL);
}

void ServerCore::setAutoindex(std::string const &sw)
{
	(sw == "on") ? this->autoindex = true : this->autoindex = false;
}

void ServerCore::pushErrPage(int key, std::string const &errpage_filename)
{
	error_page.insert(std::make_pair(key, errpage_filename));
}

std::string ServerCore::getErrPage(int key) const
{
	std::string nill;
	std::map<int, std::string>::const_iterator it = error_page.find(key);
	if (it != error_page.end() && HTTPRequest::isExist(it->second))
			return (it->second);
	return (nill);
}

void ServerCore::setSize(std::string const &_bodySize)
{
	unsigned long long int toLong = std::strtoull(_bodySize.c_str(), NULL, 10);
	if (errno == ERANGE && toLong == ULLONG_MAX)
		this->client_body_size = 200;
	else
		this->client_body_size = toLong * 1048576 / 1;
}

bool ServerCore::getAutoindex( void ) const
{
	return (autoindex);
}

