/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 01:14:58 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/28 21:27:04 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libs.hpp"
#include "ServerManager.hpp"
#include "Cgi.hpp"

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;
    try
    {   
        // Cgi::execute("www/server1/index.php", "php", env, "barev");
        // char buf[2000];
        // buf[read(3, buf, 2000)] = '\0';
        
        // std::cout << buf;
        ServerManager mgn(ac == 2 ? av[1] : DFLT);
        std::cout << "mgn.size() = " << mgn.size() << std::endl;
        for (size_t i = 0; i < mgn.size(); i++)
        {
            mgn[i].up(mgn);
        }
        mgn.start();
    }
    catch(std::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
   
}