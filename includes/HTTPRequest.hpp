/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 22:14:28 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/02 15:38:36 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQ_HPP
#define REQ_HPP
#include "Libs.hpp"
#include "HTTPResponse.hpp"
#define MAX_SIZE_REQUEST 1000

class Location;
class HTTPServer;
class HTTPRequest
{
	public:
		HTTPRequest( void );
		~HTTPRequest();
    public:
        std::string findInMap(std::string key);
        void showHeaders( void );
        std::string const &getMethod( void ) const;
        std::string const &getPath( void ) const;
        std::string const &getVersion( void ) const;
        std::string getHttpRequest() const;
        std::string getBody() const;
        bool isRequestReady() const;
        bool isResponseReady() const;
    public:
        static bool isDir(std::string const &filePath);
        static bool isFile(std::string const &filePath);
        static bool isExist(std::string const &filePath);
        static void charChange(std::string &str, char s, char d);
        static std::string lastChar(std::string const &str, char s);
        static void firstChar(std::string &str, char s);
        static std::string middle_slash(std::string const &s1, char s, std::string const &s2);
        static size_t slashes(std::string const &pathtosplit);
        static std::string ltrim(const std::string &str);
        static std::string rtrim(const std::string &str);
        static std::string trim(const std::string &str);
    protected:
        std::vector<std::string> pathChunking(std::string const &rPath);
        void checkPath(HTTPServer const &srv);
        int in(std::string const &method);
        void processing(HTTPServer &srv);
        void processing(sock_t fd);
    protected:
        std::vector<std::string> pathChunks;
        // enum PathInfo {ISDIR, ISFILE, NOTFOUND, FORBIDDEN};
        enum PathStatus{ISDIR, DIROFF, DIRON, ISFILE, NOTFOUND, FORBIDDEN, UNDEFINED};
        // PathStatus path_status(std::string const &checkPath);
        // PathStatus path_status(HTTPServer const &srv, std::string const &checkPath);
        // PathStatus path_status(const Location* location, std::string const &checkPath);
        PathStatus path_status(bool autoindex, std::string const &checkPath);
        PathStatus pathinfo;
    protected:
        size_t reqLineEnd;
        size_t bodyEnd;
		char *http;
        std::string httpRequest;
        std::string request;
        std::string method;
        std::string path;
        std::string realPath;
        std::string actualPath;
        std::string absolutePath;
        std::string reqStatus;
        std::string filename;
        std::string extension;
        std::string queryString;
        std::string version;
        std::string headers;
        std::string _body;
    protected:
        int statusCode;
    protected:
        std::map<std::string, std::string> httpHeaders;
    protected:
        std::string boundary;
        std::string boundaryEnd;
        std::string contentType;
        std::string transferEncoding;
        std::string type;
        unsigned long int bodySize;
    protected:
        const Location* location;
    protected:
        std::vector<std::string> methods;
        // std::map<std::string, void (HTTPRequest::*)(HTTPServer&)> methodsMap;
        // std::map<std::string, void (HTTPRequest::*)(sock_t)> contentMap;
    // protected:
    //     void get(HTTPServer &srv);
    //     void post(HTTPServer &srv);
    //     void delet(HTTPServer &srv);


    protected:
        std::string dir_content(std::string const &realPath);
    public:
        std::string const &getResponse( void );
    protected:
        std::string response;
    protected:
        bool _isHeaderReady;
        bool _isBodyReady;
        bool _isRequestReady;
        bool _isOpenConnection;
        bool _isResponseReady;
        int _bodySize;
        size_t _maxSizeRequest;

};



#endif