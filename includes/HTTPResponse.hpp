/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maharuty <maharuty@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 01:26:17 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/05 22:08:48 by maharuty         ###   ########.fr       */
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
        std::string file(std::string const &filename);
        std::string getResponse( void );
        std::string const &getReserve404( void ) const;
        std::string const &getReserve403( void ) const;
        std::string const &getReserve(int statusCode) const;
        std::unordered_map<std::string, std::string> &getResponseHeader();
    protected:
        std::string response;
        std::unordered_map<std::string, std::string> _responseHeader;
    private:
        std::string reserve;
};

#endif