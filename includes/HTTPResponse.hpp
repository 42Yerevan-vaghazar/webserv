/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 01:26:17 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/28 01:29:45 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP
#include "Libs.hpp"
#include <unordered_map>

class HTTPResponse
{
    public:
        HTTPResponse( void );
        ~HTTPResponse();
    public:
        std::string const getResponse( void ) const;
        std::unordered_map<std::string, std::string> &getResponseHeader();
        void addHeader(const std::pair<std::string, std::string> &);
        void buildHeader();
        void setCgiPipeFd(int fd);
        std::string &getResponseBody();
        void setBody(const std::string &body);
        bool isResponseReady() const;
        bool &isResponseReady();
        bool isStarted() const;
        void setStartStatus(bool);
    protected:
        std::string _header;
        std::string _responseBody;
        std::unordered_map<std::string, std::string> _responseHeader;
        bool _isResponseReady;
        bool _isStarted;
    private:
        std::string reserve;
};

#endif