/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPCoreException.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 22:57:21 by dmartiro          #+#    #+#             */
/*   Updated: 2023/10/25 23:17:13 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPCoreException.hpp"

HTTPCoreException::HTTPCoreException(const char* msg) : err(msg)
{
	
}

HTTPCoreException::~HTTPCoreException() throw()
{
}

const char* HTTPCoreException::what( void ) const throw()
{
	return (this->err.c_str());
}