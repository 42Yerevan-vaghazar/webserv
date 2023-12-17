/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 01:38:14 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/17 01:35:23 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::string const &location)
{
	this->location = location;
}

Location::~Location()
{
	
}

// void Location::setRedirection(int status, std::string redirectPath)
// {
// 	redirection.insert(std::make_pair(status, redirectPath));
// }

// std::map<int, std::string> const &Location::getRedirection( void ) const
// {
// 	return (redirection);
// }

std::string const &Location::getLocation( void ) const
{
	return (this->location);
}

// std::string Location::getRedirection(int status) const
// {
// 	std::string nill;
// 	std::map<int, std::string>::const_iterator it = redirection.find(status);
// 	if (it != redirection.end())
// 		return (it->second);
// 	return (nill);
// }