/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 16:03:26 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/24 23:19:31 by dmartiro         ###   ########.fr       */
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
    private:
        std::string location;
};
