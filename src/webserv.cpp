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

std::string file(std::string const &filename);

int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    try
    {   
        ServerManager mgn(ac == 2 ? av[1] : DFLT);
        // for(size_t i = 0; i < mgn.)
        ///////////////////////////////////IN PARSING//////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
            // HTTPServer srv;
            // srv.setIp("127.0.0.1");
            // srv.setPort("3000");
            // srv.setRoot("www/server1/");
            // srv.setSize("200mb");
            // srv.setAutoindex("on");
            // srv.pushIndex("index.html");
            // srv.pushErrPage(404, "www/server1/error_pages/404.html");
            // // srv.pushMethods("GET");
            // // srv.pushMethods("POST");
            // srv.push_serverName("google.com");
            // srv.setAutoindex("on");
            
            
            // Location location("/images");
            // location.setRoot("www/pictures");
            // location.pushIndex("index.html");
            // location.setAutoindex("on");
            // srv.push(location.getLocation(), location);

            
            // Location location1("/images/result/post");
            // location1.setRoot("www/pictures1/");
            // location1.setAutoindex("off");
            // srv.push(location1.getLocation(), location1);


            // Location location2("/images/result/post/breakdown");
            // location2.setRoot("www/pictures2/");
            // location2.setAutoindex("off");            
            // srv.push(location2.getLocation(), location2);
            
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////


        
        // srv.up(mgn[0]);
        // mgn.push_back(srv);
        std::cout << "mgn.size() = " << mgn.size() << std::endl;
        for (size_t i = 0; i < mgn.size(); i++)
        {
            mgn[i].up(mgn);
        }
        
        mgn.start();
    //     FD_SET(srv.getfd(), &rd);
    //     maxfd = srv.getfd();

    //     std::string response;
    //     fd_set tmp_rd;
    //     fd_set tmp_wr;
    //     while (1)
    //     {
    //         FD_ZERO(&tmp_rd);
    //         FD_ZERO(&tmp_wr);
    //         tmp_rd = rd;
    //         tmp_wr = wr;
    //         int io = select(maxfd + 1, &tmp_rd, &tmp_wr, NULL, NULL);

    //         if (io > 0)
    //         {
    //             if (FD_ISSET(srv.getfd(), &tmp_rd))
    //             {
    //                 //new socket for client
    //                 sock_t cl = srv.accept();
    //                 if (cl > 0)
    //                 {
    //                     std::cout << "new socket for client is created: " << cl << std::endl;
    //                     Client client(cl);
    //                     srv.push(cl, client);
    //                     FD_SET(cl, &rd);
    //                     maxfd = std::max(maxfd, cl);
    //                 }
    //             }

    //             for (size_t i = 0; i <= maxfd; i++)
    //             {
    //                 if (FD_ISSET(i, &tmp_rd))
    //                 {
    //                     Client *client = srv.getClient(i);
    //                     if (client)
    //                     {
    //                         client->appendRequest(srv);
    //                         response = "HTTP/1.1 200 OK\r\n";
    //                         response += "Date: Mon, 27 Jul 2009 12:28:53 GMT";
    //                         response += "Server: Apache/2.2.14 (Win32)\r\n";
    //                         response += "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n";
    //                         // response += "Content-Length: 2048\r\n";
    //                         response += "Content-Type: text/html\r\n";
    //                         response += "Connection: keep-alive\r\n";
    //                         response += "\r\n";
    //                         response += client->file("www/server1/index.html");
    //                         // response += client->getResponse();
    //                         FD_SET(client->getFd(), &wr);
    //                     }
    //                 }
    //                 if (FD_ISSET(i, &tmp_wr))
    //                 {
    //                     Client *client = srv.getClient(i);
    //                     sock_t clfd = client->getFd();
    //                     int snd = send(client->getFd(), response.c_str(), response.size(), 0);
    //                     close(client->getFd());
    //                     FD_CLR(client->getFd(), &wr);
    //                     FD_CLR(client->getFd(), &rd);
    //                     srv.removeClient(client->getFd());
    //                     if (clfd == maxfd)
    //                         maxfd -= 1;
    //                 }
    //             }
    //         }
    //     }
    }
    catch(std::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
   
}





// 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
// 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
// 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


// struct sockaddr
//   {
//     __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
//     char sa_data[14];		/* Address data.  */
//   };

// struct sockaddr_in
//   {
//     __SOCKADDR_COMMON (sin_);
//     in_port_t sin_port;			/* Port number.  */
//     struct in_addr sin_addr;		/* Internet address.  */
//     /* Pad to size of `struct sockaddr'.  */
//     unsigned char sin_zero[sizeof (struct sockaddr) - __SOCKADDR_COMMON_SIZE - sizeof (in_port_t) - sizeof (struct in_addr)];
//   };



// struct addrinfo
// {
//   int ai_flags;			/* Input flags.  */
//   int ai_family;		/* Protocol family for socket.  */
//   int ai_socktype;		/* Socket type.  */
//   int ai_protocol;		/* Protocol for socket.  */
//   socklen_t ai_addrlen;		/* Length of socket address.  */
//   struct sockaddr *ai_addr;	/* Socket address for socket.  */
//   char *ai_canonname;		/* Canonical name for service location.  */
//   struct addrinfo *ai_next;	/* Pointer to next in list.  */
// };
