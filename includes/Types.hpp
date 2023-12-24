/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 00:30:33 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/24 17:11:56 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_HPP
#define TYPES_HPP
#include "Libs.hpp"

class Types
{
    public:
        static Types &GetInstance(const std::string& value);
        static std::map<std::string, std::string> MimeTypes;
    private:
        Types( void );
        ~Types();
        static Types singleton_;
};

#endif
