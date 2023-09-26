// #include <iostream>
// #include <stdio.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <arpa/inet.h>
// #include <netinet/in.h>

// // int main() {
// //     int status;
// //     struct addrinfo hints;
// //     struct addrinfo *servinfo;  // will point to the results

// //     memset(&hints, 0, sizeof hints); // make sure the struct is empty
// //     hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
// //     hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
// //     hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

// //     if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
// //         fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
// //         exit(1);
// //     }

// //     // servinfo now points to a linked list of 1 or more struct addrinfos

// //     // ... do everything until you don't need servinfo anymore ....

// //     freeaddrinfo(servinfo); // free the linked-list
// //     // // IPv4:






// //     // char ip4[INET_ADDRSTRLEN];  // space to hold the IPv4 string
// //     // struct sockaddr_in sa;      // pretend this is loaded with something

// //     // inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);

// //     // printf("The IPv4 address is: %s\n", ip4);


// //     // // IPv6:

// //     // char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
// //     // struct sockaddr_in6 sa6;    // pretend this is loaded with something

// //     // inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

// //     // printf("The address is: %s\n", ip6);
// //     // struct in6_addr ia6 = IN6ADDR_ANY_INIT;
// //     // int s;
// //     // struct addrinfo hints;
// //     // struct addrinfo *res;

// //     // // do the lookup
// //     // // [pretend we already filled out the "hints" struct]
// //     // getaddrinfo("www.example.com", "http", &hints, &res);

// //     // // again, you should do error-checking on getaddrinfo(), and walk
// //     // // the "res" linked list looking for valid entries instead of just
// //     // // assuming the first one is good (like many of these examples do).
// //     // // See the section on client/server for real examples.

// //     // s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
// //     // int socketfd = socket(AF_INET, SOCK_STREAM, 0);
// //     // std::cout << socketfd << std::endl;
// // }

// // int main() {
// //     std::string tmp;
// //     while (true)
// //     {
// //         std::cout << "first";
// //         std::cin >> tmp;
// //         std::cout << tmp;
// //         std::cout << "second";
// //         std::cin >> tmp;
// //     }
// // }


// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <errno.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <netdb.h>
// #include <arpa/inet.h>
// #include <sys/wait.h>
// #include <signal.h>

// #define PORT "3490"  // the port users will be connecting to

// #define BACKLOG 10   // how many pending connections queue will hold

// void sigchld_handler(int s)
// {
//     // waitpid() might overwrite errno, so we save and restore it:
//     int saved_errno = errno;

//     while(waitpid(-1, NULL, WNOHANG) > 0);

//     errno = saved_errno;
// }


// // get sockaddr, IPv4 or IPv6:
// void *get_in_addr(struct sockaddr *sa)
// {
//     if (sa->sa_family == AF_INET) {
//         return &(((struct sockaddr_in*)sa)->sin_addr);
//     }

//     return &(((struct sockaddr_in6*)sa)->sin6_addr);
// }

// int main(void)
// {
//     int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
//     struct addrinfo hints, *servinfo, *p;
//     struct sockaddr_storage their_addr; // connector's address information
//     socklen_t sin_size;
//     struct sigaction sa;
//     int yes=1;
//     char s[INET6_ADDRSTRLEN];
//     int rv;

//     memset(&hints, 0, sizeof hints);
//     hints.ai_family = AF_UNSPEC;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_flags = AI_PASSIVE; // use my IP

//     if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
//         fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
//         return 1;
//     }

//     // loop through all the results and bind to the first we can
//     for(p = servinfo; p != NULL; p = p->ai_next) {
//         if ((sockfd = socket(p->ai_family, p->ai_socktype,
//                 p->ai_protocol)) == -1) {
//             perror("server: socket");
//             continue;
//         }

//         if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
//                 sizeof(int)) == -1) {
//             perror("setsockopt");
//             exit(1);
//         }

//         if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
//             close(sockfd);
//             perror("server: bind");
//             continue;
//         }

//         break;
//     }

//     freeaddrinfo(servinfo); // all done with this structure

//     if (p == NULL)  {
//         fprintf(stderr, "server: failed to bind\n");
//         exit(1);
//     }

//     if (listen(sockfd, BACKLOG) == -1) {
//         perror("listen");
//         exit(1);
//     }

//     sa.sa_handler = sigchld_handler; // reap all dead processes
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = SA_RESTART;
//     if (sigaction(SIGCHLD, &sa, NULL) == -1) {
//         perror("sigaction");
//         exit(1);
//     }

//     printf("server: waiting for connections...\n");

//     while(1) {  // main accept() loop
//         sin_size = sizeof their_addr;
//         new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
//         if (new_fd == -1) {
//             perror("accept");
//             continue;
//         }

//         inet_ntop(their_addr.ss_family,
//             get_in_addr((struct sockaddr *)&their_addr),
//             s, sizeof s);
//         printf("server: got connection from %s\n", s);

//         if (!fork()) { // this is the child process
//             close(sockfd); // child doesn't need the listener
//             if (send(new_fd, "Hello, world!", 13, 0) == -1)
//                 perror("send");
//             close(new_fd);
//             exit(0);
//         }
//         close(new_fd);  // parent doesn't need this
//     }

//     return 0;
// }

// class Entity
// {
//     public:
//     Entity() = default;
//     Entity(const Entity&) = default;
//     // Entity &operator=(const Entity&);
//        void print() {};
// };

// #include <iostream>
// int main()
// {
//     Entity *ptr;
//     Entity a;
//     Entity b;
//     b = a;
//     b = Entity(a);
//     ptr = &a;
//     ptr->print();
//     std::cout << "barev\n";
// }

// #include <iostream>
// int main()
// {
//     int a;
//     int b;
//     int c;


//     c = a = b;
//     a + b + c;
//     char *c;
//     while(true)
//     {
//         std::cin >> c;
//         std::cout << c << "asfsssdfhasbdukaasdfasasfasfsafasfasfasfasfasfasfasfasfsafydcbgscuvydfvbhfdbvhbdfjvfd" << '\n';
//     }
//     std::cout << "barev\n";
// }


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int main() {

    int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr , 0 , sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3000);
    std::cout << "addr.sin_port = " << addr.sin_port << std::endl;
    std::cout << "addr.sin_addr.s_addr = " << addr.sin_addr.s_addr << std::endl;
    if (bind(serverSocket, (sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind: ");
    }
    if (listen(serverSocket, 0) == -1) {
        perror("listen: ");
    };
    struct sockaddr_in addrClient;
    socklen_t len = 100;
    memset(&addrClient , 0 , sizeof(struct sockaddr_in));
    int clientSocket = accept(serverSocket, (sockaddr *)&addrClient, &len);
    if (clientSocket == -1) {
        perror("accept: ");
    }
    char buf[2044];
    if (recv(clientSocket, buf, sizeof(buf), 0) == -1) {
        perror("recv :");
    }
    std::cout << buf << std::endl;
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
    shutdown(serverSocket, SHUT_RDWR);
    close(serverSocket);
}