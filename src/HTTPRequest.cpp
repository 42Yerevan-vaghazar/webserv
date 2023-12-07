/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:14:54 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/05 21:31:31 by maharuty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "HTTPServer.hpp"


// class HTTPServer;
HTTPRequest::HTTPRequest( void )
{
    reqLineEnd = 0;
    bodyEnd = 0;
    bodySize = 0;
    statusCode = 0;
    location = NULL;
    _maxSizeRequest = 0;
    _bodySize = 0;
    // methodsMap["GET"] = &HTTPRequest::get;
    // methodsMap["POST"] = &HTTPRequest::post;
    // methodsMap["DELETE"] = &HTTPRequest::delet;
    //boundary = "&"; // !IMPORTANT: if GET request: the boundary is (&) else if POST request: boundary is read from (Headers)
    // methods.push_back("GET");
    // methods.push_back("POST");
    // methods.push_back("DELETE");
}

HTTPRequest::~HTTPRequest()
{
}

std::string const &HTTPRequest::getMethod( void ) const
{
    return (method);
}

std::string HTTPRequest::getBody() const
{
    return (_body);
}

std::string const &HTTPRequest::getPath( void ) const
{
    return (path);
}

std::string const &HTTPRequest::getVersion( void ) const
{
    return (version);
}

std::string HTTPRequest::getHttpRequest() const {
    return (httpRequest);
}

bool HTTPRequest::isRequestReady() const {
    return (_isRequestReady);
}

bool HTTPRequest::isResponseReady() const {
    return (_isResponseReady);
}

std::string HTTPRequest::rtrim(const std::string &str)
{
    size_t end = str.find_last_not_of(" \r\n\t\f\v");
    return (end == std::string::npos ? "" : str.substr(0, end + 1));
}

std::string HTTPRequest::ltrim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \r\n\t\f\v");
    return (start == std::string::npos ? str : str.substr(start));
}

std::string HTTPRequest::trim(const std::string &str)
{
    return (ltrim(rtrim(str)));
}

// void HTTPRequest::processing(sock_t fd)
// {
// }

void HTTPRequest::charChange(std::string &str, char s, char d)
{
    for(size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == s)
            str[i] = d;
    }
}

std::string HTTPRequest::findInMap(std::string const &key)
{
    std::map<std::string, std::string>::iterator in = httpHeaders.find(key);
    if (in != httpHeaders.end())
        return (in->second);
    std::string nill;
    return (nill);
}

// void HTTPRequest::get(HTTPServer &srv)
// {
//     // std::cout << "GET method" << std::endl;
// }

// void HTTPRequest::post(HTTPServer &srv)
// {
//     if (!(contentType = findInMap("Content-Type")).empty())
//     {
//         type = trim(contentType.substr(0, contentType.find(";")));
//         // HTTPRequest::contentReceiveMethod(fd);
//     }
//     else if (!(transferEncoding = findInMap("Transfer-Encoding")).empty())
//     {
//         std::cout << "Data transfers chunck by chunk" << std::endl;
//     }
// }

// void HTTPRequest::delet(HTTPServer &srv)
// {
//     std::cout << "method is DELETE" << std::endl;
// }

// void HTTPRequest::multipart(sock_t fd)
// {
//     std::cout << "multipart/form-data" << std::endl;
//     std::cout << httpRequest << std::endl;
//     contentType.erase(0, contentType.find(";")+1);
//     boundary = "--" + contentType.substr(contentType.find("=")+1);
//     boundaryEnd = boundary + "--";
//     std::stringstream iss(httpRequest);
//     std::vector<std::string> content;
//     std::string line;
//     std::string get_next_line;
//     while (std::getline(iss, get_next_line))
//     {
//         if (trim(get_next_line) == boundary || trim(get_next_line) == boundaryEnd)
//         {
//             if (!line.empty())
//             {
//                 // content.push_back(line);
//                 line.erase();
//             }
//         }
//         else
//             line += get_next_line + "\r\n";
//     }
// }

void HTTPRequest::showHeaders( void )
{
    std::map<std::string, std::string>::iterator it;
    for(it = httpHeaders.begin(); it != httpHeaders.end(); it++)
    {
        std::cout << it->first << " = " << it->second << std::endl;
    }
}

void HTTPRequest::lastChar(std::string &str, char s)
{
    if (!str.empty())
    {
        size_t lst = str.size() - 1;
        if (str[lst] == s)
            str.erase(lst);
    }
}

void HTTPRequest::firstChar(std::string &str, char s)
{
    if (!str.empty())
        if (str[0] != s)
            str = s + str;
}

int HTTPRequest::in(std::string const &method)
{
    std::vector<std::string>::iterator it = std::find(methods.begin(), methods.end(), method);
    if (it != methods.end())
        return (1);
    return (0);
}

// void HTTPRequest::processing(HTTPServer &srv)
// {
//     std::map<std::string, void(HTTPRequest::*)(HTTPServer&)>::iterator function = methodsMap.find(method);
//     if (function != methodsMap.end())
//        (this->*(function->second))(srv);
// }

std::string HTTPRequest::dir_content(std::string const &realPath)
{
    DIR* odir = opendir(realPath.c_str());
    std::string directoryContent;
    if (odir)
    {
        struct dirent* each;
        while ((each = readdir(odir)) != NULL)
        {
            std::string d_f_name = "<a href=\"" + std::string(each->d_name) + "\">" + std::string(each->d_name) + "</a><br>";
            directoryContent.insert(0, d_f_name);
        }
        return (directoryContent);
    }
    return (directoryContent);
}

std::string const &HTTPRequest::getResponse( void )
{
    return (response);
}

void HTTPRequest::checkPath(HTTPServer const &srv)
{
    size_t use = 0;
    std::string possibleRoot = srv.getRoot();
    if ((use = path.find_first_of("?")) != std::string::npos)
    {
        queryString = path.substr(use+1);
        realPath = path.substr(0, use);
    }
    else
        realPath = path;
    
    if ((location = srv.find(realPath)))
    {
        possibleRoot = location->getRoot();
        lastChar(possibleRoot, '/');
        actualPath = possibleRoot;
    }
    else
    {
        lastChar(possibleRoot, '/');
        actualPath = possibleRoot + realPath;
    }

    
    std::cout << "+_+_+_+_+_+_+_+_+" << std::endl;
    std::cout << "Query : " << (!queryString.empty() ? queryString : "no query") << std::endl;
    std::cout << "RealPath : " << (!realPath.empty() ? realPath : "no path") << std::endl;
    std::cout << "ActualPath : " << (!actualPath.empty() ? actualPath : "no actual path") << std::endl;
    std::cout << "+_+_+_+_+_+_+_+_+" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

HTTPRequest::PathStatus HTTPRequest::path_status(bool autoindex, std::string const &checkPath)
{
    if (isExist(checkPath))
    {
        if (isDir(checkPath))
        {
            if (autoindex == true)
                return (ISDIR);
            else
                return (DIROFF);
        }
        else if (isFile(checkPath))
            return (ISFILE);
    }
    return (NOTFOUND);
}

size_t HTTPRequest::slashes(std::string const &pathtosplit)
{
    size_t count = 0;
    for(size_t i = 0; i < pathtosplit.size(); i++)
        if (pathtosplit[i] == '/')
            count++;
    return (count);
}


// HTTPRequest::PathStatus HTTPRequest::path_status(HTTPServer const &srv, std::string const &checkPath)
// {
//     if (path_status(checkPath) == PathStatus::ISDIR)
//         if (srv.getAutoindex() == true)
//             return (PathStatus::DIRON);
//     return (PathStatus::DIROFF);
// }

// HTTPRequest::PathStatus HTTPRequest::path_status(const Location* location, std::string const &checkPath)
// {
//     if (path_status(checkPath) == PathStatus::ISDIR)
//         if (location->getAutoindex() == true)
//             return (PathStatus::DIRON);
//     return (PathStatus::DIROFF);
// }

// HTTPRequest::PathStatus HTTPRequest::path_status(std::string const &checkPath)
// {
//     if (isExist(checkPath))
//     {
//         if (isDir(checkPath))
//             return (PathStatus::ISDIR);
//         else if (isFile(checkPath))
//             return (PathStatus::ISFILE);
//     }
//     return (PathStatus::NOTFOUND);
// }


bool HTTPRequest::isDir(const std::string& filePath) {
    struct stat file;
    if (stat(filePath.c_str(), &file) != 0)
        return false;
    return S_ISDIR(file.st_mode);
}

bool HTTPRequest::isFile(const std::string& filePath) {
    struct stat file;
    if (stat(filePath.c_str(), &file) != 0)
        return false;
    return S_ISREG(file.st_mode);
}

bool HTTPRequest::isExist(std::string const &filePath)
{
    struct stat existing;
    return (stat(filePath.c_str(), &existing) == 0);
}
