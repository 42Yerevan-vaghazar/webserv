/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 00:30:33 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/23 01:26:42 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_HPP
#define TYPES_HPP
#include "Libs.hpp"

class Types
{
    public:
        Types( void );
        ~Types();
    private:
        std::vector<std::string> ContentType;
        std::map<std::string, std::string> MimeTypes;
};

#endif
