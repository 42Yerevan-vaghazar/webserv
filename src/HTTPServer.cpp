/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 23:57:39 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/12 21:59:18 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPServer.hpp"
#include <unordered_map>
#include "HelperFunctions.hpp"

size_t longestMatch(std::string const &s1, std::string const &s2);

HTTPServer::HTTPServer( void )
{
    //defualt initializations
    this->port = DEFAULT_HTTP_PORT;
    this->ip = DEFAULT_MASK;
    methodsMap["GET"] = &HTTPServer::get;
    methodsMap["POST"] = &HTTPServer::post;
    methodsMap["DELETE"] = &HTTPServer::del;
    //boundary = "&"; // !IMPORTANT: if GET request: the boundary is (&) else if POST request: boundary is read from (Headers)
    methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");
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
        throw HTTPCoreException("The entered IP address does not follow the correct syntax.");
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
                throw HTTPCoreException("The entered IP address does not follow the correct syntax.");
            octet.clear();
            n = 0;
        }
        else
            throw HTTPCoreException("The entered IP address does not follow the correct syntax.");

    }
    if (k != 3)
        throw HTTPCoreException("The entered IP address does not follow the correct syntax.");
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
    // if (!mgn.used(this))
    // {
        const char* givenIp = ip.c_str();
        const char* givenPort = port.c_str();
        Tcp::setup(givenIp, givenPort);
        Tcp::createSocket();
        Tcp::bindSocket();
        Tcp::listenSocket();
        // mgn.setmax(fd);  // TODO delete line
        std::cout << givenIp <<  ":" << givenPort << std::endl;
        freeaddrinfo(addrList);
    // }
    // else
    //     std::cout << "{Already:used}" << std::endl;
    
}

void HTTPServer::push(sock_t clFd, Client *clt)
{
    clnt.insert(std::make_pair(clFd, clt));
}

int HTTPServer::pop(sock_t clFd)
{
    std::map<sock_t, Client*>::iterator it = clnt.find(clFd);
    if (it != clnt.end())
    {
        delete it->second;
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

bool HTTPServer::operator==(sock_t fd)
{
    if (clnt.find(fd) != clnt.end()) {
      return (true);
    } 
    return (false);
}

Client* HTTPServer::getClient(sock_t fd)
{
    std::map<sock_t, Client*>::iterator it = clnt.find(fd);
    if (it != clnt.end())
        return (it->second);
    return (NULL);
}

void HTTPServer::removeClient(sock_t fd)
{
    std::map<sock_t, Client*>::iterator it = clnt.find(fd);
    if (it != clnt.end())
        clnt.erase(it);  // TODO delete object before erase
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

// void HTTPServer::setSize(std::string const &_bodySize)
// {
// 	unsigned long long int toLong = std::strtoull(_bodySize.c_str(), NULL, 10);
// 	if (errno == ERANGE && toLong == ULLONG_MAX)
// 		this->client_body_size = 200;
// 	else
// 		this->client_body_size = toLong * 1048576 / 1;
// }

// bool HTTPServer::getAutoindex( void ) const
// {
// 	return (autoindex);
// }


std::string HTTPServer::get(Client &client) {
    const std::string &path = client.getPath();
    std::unordered_map<std::string, std::string> headerContent;
    std::string  fileName;

    std::cout << "path = " << path << std::endl;
    // TODO unrecognized types "application/octet- stream"
    if(path[path.size() - 1] == '/')
    {
        fileName = "www/server1/index.html";  //TODO - remove hardcode should be default page from config  
        client.addHeader(std::pair<std::string, std::string>("Content-Type", "text/html")); //TODO - remove hardcode

    } else {
        // if ()
        // client.addHeader(std::pair<std::string, std::string>("Content-Type", "application/octet- stream")); //TODO - remove hardcode
        fileName = path;
    }
    // TODO check is method allowed. 405
    // TODO Content-Length is not defined in case post method called 411
    // TODO valid request line 412
    // TODO body is large 413
    // TODO The URI requested is long  414
    // TODO header is large 431
    // std::cout << "fileName = " << fileName << std::endl;
    if (access(fileName.c_str(), F_OK) == 0) {   // TODO check permission to read
        std::string fileContent = fileToString(fileName);
        client.addHeader(std::pair<std::string, std::string>("Content-Length", std::to_string(fileContent.size())));
        client.buildHeader();
        return (client.getResponse() + fileContent);
    } else {
        throw ResponseError(404, "not found");
        // TODO automate it   404, 405, 408, 411, 412, 413, 414, 431, 500, 501, 505, 503, 507, 508
    }
    return ("");
};

std::string HTTPServer::post(Client &client) {
    
    std::cout << "\n--- in Post function \n" << std::endl;
    // TODO if cgi exstention detected go through cgi and give body as stdin else get files
    // TODO if multipart data not detected throw precondition failed
    const std::unordered_map<std::string, std::string> &uploadedFiles = client.getUploadedFiles();
    std::unordered_map<std::string, std::string>::const_iterator it = uploadedFiles.cbegin();
    // std::cout << "filename = " << fileName << std::endl;
    for (; it != uploadedFiles.cend(); ++it) {
        const std::string &fileName = it->first;
        const std::string &fileContent = it->second;
        std::ofstream ofs("./www/server1/data_base/" + fileName);
        
        if (ofs.is_open() == false) {
            throw ResponseError(507 , "Insufficient Storage");
        }
        ofs << fileContent;
        if (ofs.good() == false) {
            throw ResponseError(507 , "Insufficient Storage");
        }
        ofs.close();
    }
    std::string response;
    client.addHeader(std::pair<std::string, std::string>("content-type", "text/plain"));
    client.buildHeader();
    response = client.getResponse();
    response += "ok";
    return (response);
};

std::string HTTPServer::del(Client &client) {
    std::string response;
    if (std::remove(client.getPath().c_str()) == -1) {
        throw ResponseError(404, "not found");
    };
    return (response);
};

std::string HTTPServer::processing(Client &client)
{
    std::map<std::string, std::string(HTTPServer::*)(Client&)>::iterator function = methodsMap.find(client.getMethod());
    if (function != methodsMap.end())
       return ((this->*(function->second))(client));
    throw ResponseError(405, "Method Not Allowed");
    return ("");
}

bool	directory_listing(std::string path)
{
	DIR					*opened_dir;
	dirent				*dir_struct;
	std::string			table;
	std::string			name;
	struct stat			buf;
	struct tm			*timeinfo;
	char				time_buf[100];
	std::string 		relPath;
	std::string 		displayPath;
	
	if (path != "." && path != ".." && path[0] != '/' && (path[0] != '.' && path[1] != '/') && (path[0] != '.' && path[1] != '.' && path[2] != '/')) {
        relPath = "./" + path + "/";
        displayPath = "./" + path;
    } else {
		relPath = path + "/";
		displayPath = path;
	}
	opened_dir = opendir(relPath.c_str());
	
	table += "<!DOCTYPE html><html><head><title>";
	table += "Index of ";
	table += displayPath;
	table += "</title>";
	table += "<style>";
	table += ".box>* {flex: 33.33%;}";
	table += ".box {display: flex; flex-wrap: wrap; width: 75%;}";
	table += "</style></head>";
	table += "<body><h1>";
	table += "Index of ";
	table += displayPath;
	table += "</h1><hr><pre class=\"box\">";

	dir_struct = readdir(opened_dir);
	while ((dir_struct = readdir(opened_dir)) != NULL)
	{
		name = dir_struct->d_name;
		if (name != ".")
		{
			table += "<a href=\"";
			table += name;
			table += "\">";
			table += name;
			table += "</a>";
			//std::cout << "name = " << name << std::endl;
			if (stat((relPath + name).c_str(), &buf) == 0)
			{
				//std::cout << "name.c_str() = " << name.c_str() << std::endl;
				table += "<span>";
				timeinfo = localtime(&(buf.st_mtime));
				strftime(time_buf, 100, "%d-%b-%Y %H:%S", timeinfo);
				table += time_buf;
				table += "</span><span>";
				if (dir_struct->d_type == DT_DIR)
					table += "-";
				table += "</span>";
			}
			table += "<br>";
		}
	}
	table += "</pre><hr></body></html>";
	closedir(opened_dir);
    std::cout << table << std::endl;
    return true;
}