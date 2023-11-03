#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <errno.h>

#include "Server.hpp"
// #include "Server_tmp.hpp"
#include "Client.hpp"



#include "EvManager.hpp"
#include "Cgi.hpp"

int main(int ac, char **av, char **env) {
    try
    {
        Server server("127.0.0.1", 3000);
        server.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // Client test;
    // Cgi::execute("index.php", "php", env);
    // std::cout << server.get("/index.html") << std::endl;
}


// int main() {
//     int a = 1;
//     int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
//     if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0) {
//         std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
//     }
//     struct sockaddr_in addr;
//     memset(&addr , 0 , sizeof(struct sockaddr_in));
//     addr.sin_family = AF_INET;
//     addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     addr.sin_port = htons(3000);
//     std::cout << "addr.sin_port = " << addr.sin_port << std::endl;
//     std::cout << "addr.sin_addr.s_addr = " << addr.sin_addr.s_addr << std::endl;
//     if (bind(serverSocket, (sockaddr *)&addr, sizeof(addr)) == -1) {
//         perror("bind: ");
//     }
//     if (listen(serverSocket, 0) == -1) {
//         perror("listen: ");
//     };
//     while(true) {
//         int clientSocket = 0;
//         struct sockaddr_in addrClient;
//         socklen_t len = 100;
//         memset(&addrClient , 0 , sizeof(struct sockaddr_in));
//         std::cout << "before\n";
//         // clientSocket = accept(serverSocket, (sockaddr *)&addrClient, &len);
//         clientSocket = accept(serverSocket, NULL, NULL);
//         std::cout << "clientSocket = " << clientSocket << std::endl;
//         std::cout << "after\n";
//         std::cout << "addrClient.sin_port = " << addrClient.sin_port << std::endl;
//         std::cout << "addrClient.sin_addr.s_addr = " << addrClient.sin_addr.s_addr << std::endl;
//         if (clientSocket == -1) {
//             perror("accept: ");
//         }
//         char buf[2044];
//         std::cout << "recv\n";
//         if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
//             perror("recv :");
//         }
//         std::cout << "recv after\n";
//         std::cout << buf << std::endl;

//         const char* reply =
//             "HTTP/1.1 200 OK\r\n"
//             "Content-Type: text/html\r\n"
//             "Content-Length: 150\r\n"
//             "Connection: keep-alive\r\n"
//             "Server : webserv\r\n"
//             "\r\n"
//             "<!DOCTYPE html><html><head>  <title>Title of the document</title></head><body>  <h1>This is a heading</h1>  <p>This is a paragraph.</p></body></html>\n";

//         std::cout << "send\n";
//         if (send(clientSocket, reply, strlen(reply), 0) == -1)
//         {
//             perror("send :");
//         }
//         std::cout << "send after\n";
//         close(clientSocket);
//     }
//     close(serverSocket);
// }



// const char* reply =
//         "HTTP/1.1 201 OK\n"
//         "Content-Type: application/json\n"
//         "Content-Length: 2000\n"
//         "Keep - Alive: timeout=1, max=1\n"
//         "Accept-Ranges: 2000\n"
//         "Connection: keep-alive\n\n"
//         "data: {'mode':'global'}\n";
