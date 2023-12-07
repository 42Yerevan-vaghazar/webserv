/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 01:28:07 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/28 01:29:33 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse( void )
{
    
}

HTTPResponse::~HTTPResponse()
{
    
}

std::string HTTPResponse::file(std::string const &filename)
{
    std::fstream f;
    std::string tmp;
    std::string content;
    f.open(filename.c_str());
    if (f.is_open())
    {
        while (std::getline(f, tmp, '\n'))
        {
            content += tmp;
            tmp.clear();
        }
    }
    return (content);
}

std::string const &HTTPResponse::getResponse( void ) const
{
    return (response);
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
