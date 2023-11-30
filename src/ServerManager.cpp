/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 00:05:52 by dmartiro          #+#    #+#             */
/*   Updated: 2023/11/18 15:19:40 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"
#include "EvManager.hpp"

void ServerManager::start() {
    EvManager::start();

    for (int i = 0; i < this->size(); ++i) {
        EvManager::addEvent((*this)[i].getfd(), EvManager::read);
    }

    while(true) {
        std::pair<EvManager::Flag, int> event = EvManager::listen();
        // std::cout << "event.second = " << event.second << std::endl;
        // std::cout << "event.first = " << event.first << std::endl;
        for (int i = 0; i < this->size(); ++i) {
            if ((*this)[i].getfd() == event.second) {
                sock_t fd = accept((*this)[i].getfd(), 0, 0);
                // std::cout << "\n_serverSocket\n" << std::endl;
                fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
                Client *client = new Client(fd, (*this)[i].getfd());  // TODO delete in destructor
                // if (client.getFd() == -1) {  // TODO is it needed
                //     throw std::runtime_error(std::string("accept: ") + strerror(errno));
                // }
                EvManager::addEvent(fd, EvManager::read);
                (*this)[i].push(fd, client);
                continue ;
            }
        }
        // // std::cout << "else\n";
        Client *client;
        for (int i = 0; i < this->size(); ++i) {
            client = (*this)[i].getClient(event.second);
            if (client) {
                break;
            }
        }
        // if (client == NULL) {  TODO probably it never works

        // }
        // if (event.first == EvManager::eof) {
        //     // std::cout << "\nEV_EOF\n" << std::endl;
        //     closeConnetcion(client->getFd());
        // } else if (event.first == EvManager::read) {
        //     // std::cout << "\nEVFILT_READ\n" << std::endl;
        //     if (client->getHttpRequest().empty()) {
        //         EvManager::addEvent(client->getFd(), EvManager::write);
        //     }
        //     if (client->receiveMessage() == -1) {
        //         closeConnetcion(client->getFd());
        //     }
        //     if (client->isRequestReady()) {
        //         client->parsing();
        //         client->setResponse(generateResponse(client->getHttpRequest(), client->getBody(), client->getHeaders()));
        //     }
        // } else if (client->isResponseReady() && event.first == EvManager::write) {
        //     // std::cout << "\nEVFILT_WRITE\n" << std::endl;
        //     // TODO send response little by little
        //     if (client->sendMessage() == true) {
        //         closeConnetcion(client->getFd());
        //     }
        // } else if (client->isResponseReady() == false) {
        //     if (client->receiveMessage() == -1) {
        //         closeConnetcion(client->getFd());
        //     }
        //     if (client->isRequestReady()) {
        //         client->parsing();
        //         client->setResponse(generateResponse(client->getHttpRequest(), client->getBody(), client->getHeaders()));
        //     }
        // }
    }
};

bool ServerManager::closeConnetcion(sock_t fd) {
    EvManager::delEvent(fd, EvManager::read);
    EvManager::delEvent(fd, EvManager::write);
    close(fd);
    getServerByClientSocket(fd)->removeClient(fd);
    return (true);
};

ServerManager::ServerManager(std::string const &configfile)
{
    (void)configfile;
}

ServerManager::~ServerManager()
{
    
}



/*************************************************************
Finding correct HTTPServer funtions based on ::ServerManager::
**************************************************************/
HTTPServer *ServerManager::getServerBySocket(sock_t fd)
{
    for(size_t i = 0; i < this->size(); i++)
    {
        if (fd == (*this)[i].getfd())
            return (&(*this)[i]);
    }
    return (NULL);
}

HTTPServer *ServerManager::getServerByClientSocket(sock_t fd)
{
    for(size_t i = 0; i < this->size(); i++)
    {
        if ((*this)[i].getClient(fd))
            return (&(*this)[i]);
    }
    throw std::logic_error("getServerByClientSocket");
    return (NULL);
}

int ServerManager::used(HTTPServer *srv) const
{
    if (!this->empty())
    {
        for(size_t i = 0; i < this->size(); i++)
            if (std::strcmp((*this)[i].getPort(), srv->getPort()) == 0)
                return (-1);
    }
    return (0);
}



sock_t ServerManager::findServerBySocket(sock_t issetfd) 
{
    if (issetfd == -1)
        return (-1);
    for(size_t i = 0; i < this->size(); i++)
    {
        HTTPServer server = (*this)[i];
        if (issetfd == server.getfd())
            return (server.getfd());
    }
    return (-1);
}


sock_t ServerManager::findClientBySocket(sock_t issetfd)
{
    if (issetfd == -1)
        return (-1);
    for(size_t i = 0; i < this->size(); i++)
    {
        Client* client = (*this)[i].getClient(issetfd);
        if (client)
            return (client->getFd());
    }
    return (-1);
}



/*******************************************************************
Select Multiplexing  I/O Helper funtions based on ::ServerManager::
*******************************************************************/

int ServerManager::isServer(sock_t fd)
{
    return (0);
}

int ServerManager::isClient(sock_t fd)
{
    return (0);
}

// void ServerManager::push(HTTPServer const &srv)
// {
//     srvs.push_back(srv);
// }