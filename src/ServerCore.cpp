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

ServerCore::ServerCore() {
	_autoindex = false;
	_redirect = false;
	client_body_size = 0;
};


int ServerCore::getClientBodySize( void ) const
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

const char* ServerCore::findIndex(std::string const &filename) const // TODO useless
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
	(sw == "on") ? this->_autoindex = true : this->_autoindex = false;
	if (sw == "on")
		this->_autoindex = true;
	else if (sw == "off")
		this->_autoindex = false;
}

void ServerCore::pushErrPage(int key, std::string const &errpage_filename)
{
	error_page.insert(std::make_pair(key, errpage_filename));
}

std::string ServerCore::getErrPage(int key) const
{
	std::string nill;
	std::map<int, std::string>::const_iterator it = error_page.find(key);
	if (it != error_page.end())
		return (it->second);
	return (nill);
}

void ServerCore::setSize(std::string const &bodySize)
{
	char *ptr;
	unsigned long int toLong = std::strtoul(bodySize.c_str(), &ptr, 10);
	if (*ptr != '\0') {
		throw std::logic_error("client_body_size out of range unsigned long int max");
	}
	if (errno == ERANGE && toLong == ULLONG_MAX)
		this->client_body_size = 200;
	else
		this->client_body_size = toLong * 1048576 / 1;
}

void ServerCore::setUploadDir(std::string const &upload_dir)
{
	_uploadDir = upload_dir;
}

bool ServerCore::getAutoindex( void ) const
{
	return (_autoindex);
}

void ServerCore::setRedirection(int status, std::string redirectPath)
{
	_redirections.insert(std::make_pair(status, redirectPath));
}

void ServerCore::setR(bool status)
{
	_redirect = status;
}

std::map<int, std::string> const &ServerCore::getRedirection( void ) const
{
	return (_redirections);
}

std::string ServerCore::getRedirection(int status) const
{
	std::string nill;
	std::map<int, std::string>::const_iterator it = _redirections.find(status);
	if (it != _redirections.end())
		return (it->second);
	return (nill);
}

void ServerCore::setCgi(std::string cgiName, std::string cgiPath)
{
	_cgis.insert(std::make_pair(cgiName, cgiPath));
}

std::pair<std::string, std::string> ServerCore::getCgi( std::string const &cgiType) const {
	std::string nill;
	std::map<std::string, std::string>::const_iterator it = _cgis.find(cgiType);
	if (it != _cgis.end())
		return (std::make_pair(it->first, it->second));
	return (std::make_pair("", ""));
};