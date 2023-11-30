/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 09:23:04 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/04 15:16:22 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_HPP
#define CORE_HPP
#include "Libs.hpp"

struct IListener
{
	protected:
		virtual void setIp(std::string const &ipv) = 0;
		virtual void setPort(std::string const &port) = 0;
		virtual const char* getIp( void ) const = 0;
		virtual const char* getPort( void ) const = 0;
        virtual uint32_t getNIp( void ) const = 0;
		virtual uint16_t getNPort( void ) const = 0;
	protected:
        std::string ip;
        std::string port;
    protected:
        uint16_t n_port;
        uint32_t n_ip;

};



#endif