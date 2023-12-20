/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tcp.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 21:32:40 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/02 23:00:20 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCP_HPP
#define TCP_HPP
#include "Libs.hpp"
class Tcp
{
	public:
		Tcp( void );
		~Tcp();
	public:
		sock_t accept( void );
	protected:
        const char* pton(uint32_t ipv) const;
		void setup(const char* ip, const char* port);
		void createSocket( void );
		void bindSocket( void );
		void listenSocket( void );
	protected:
		int _fd;
		int backlog;
	protected:
		struct addrinfo rules;
		struct addrinfo* addrList;
		struct sockaddr_in *Socket;
		struct sockaddr *SocketAddress;
		struct sockaddr_storage clntAddr;
};

#endif