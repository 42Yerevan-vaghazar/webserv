/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPCoreException.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 22:53:23 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/02 21:52:23 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_CORE_EXCEPTIONS_HPP
#define HTTP_CORE_EXCEPTIONS_HPP
#include "Libs.hpp"

class HTTPCoreException : public std::exception
{
	public:
		HTTPCoreException(const char *msg);
		virtual ~HTTPCoreException() throw();
		virtual const char* what( void ) const throw();
	private:
		std::string err;
};

#endif