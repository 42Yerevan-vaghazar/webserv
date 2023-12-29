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
	this->_location = location;
}


Location &Location::operator=(Location const &rhs) {
	if (this != &rhs) {
		ServerCore::operator=(rhs);
		_location = rhs._location;
		_nestedLocations = rhs._nestedLocations;
	}
	return (*this);
};

Location &Location::operator=(ServerCore const &rhs) {
	if (this != &rhs) {
		ServerCore::operator=(rhs);
		error_page.clear();
		_cgis.clear();
		_redirections.clear();
	}
	return (*this);
};

Location::~Location()
{
	
}

std::string const &Location::getLocation( void ) const
{
	return (this->_location);
}
