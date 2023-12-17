/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 16:03:26 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/02 00:22:58 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Libs.hpp"
#include "ServerCore.hpp"


class Location : public ServerCore
{
    public:
        Location(std::string const &location);
        ~Location();
    public:
        std::string const &getLocation( void ) const;
        // void setRedirection(int status, std::string redirectPath);
        // std::map<int, std::string> const &getRedirection( void ) const;
        // std::string getRedirection(int status) const;
    private:
        // bool redirect;
        // std::map<int, std::string> redirection;
        std::string location;
        std::map<std::string, Location> nestedLocations;
};
