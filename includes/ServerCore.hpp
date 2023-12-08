/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerCore.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42yerevan.am    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 01:49:46 by dmartiro          #+#    #+#             */
/*   Updated: 2023/10/24 09:59:17 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SRV_CORE_HPP
#define SRV_CORE_HPP
#include "Libs.hpp"
#include "HTTPRequest.hpp"

class HTTPRequest;

struct ServerCore
{
    public:
        std::string const &getRoot( void ) const;
        std::vector<std::string> getIndexFiles( void ) const;
        std::vector<std::string> getMethods( void ) const;
        int getClientBodySize( void );
        bool getAutoindex( void ) const;
    public:
        const char* findIndex(std::string const &filename) const;
        const char* findMethod(std::string const &method) const;
    public:
        void pushIndex(std::string const &fileNameExample);
        void pushMethods(std::string const &method);
        void pushErrPage(int, std::string const &errpage_filename);
        std::string getErrPage(int key) const;
        void setRoot(std::string const &root);
        void setAutoindex(std::string const &sw);
        void setSize(std::string const &_bodySize);
    protected:
        std::string root;                                       // [root]               www/server1/
        std::vector<std::string> index;                         // [index]              index.html barev.html index.htm ....
        std::vector<std::string> methods;                       // [allow_methods]      GET | POST | DELETE
        std::map<int, std::string> error_page;                  // [error_page]         404 [root]/error_pages/404.html 
        bool autoindex;                                         // [autoindex]          on (true) | off (false)
        unsigned long long int client_body_size;                // [client_body_size]   200m -> 200.000.000byte -> 200mb
};
#endif