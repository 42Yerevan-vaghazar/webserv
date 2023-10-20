// #include <iostream>
// #include <string>
// #include <cstring>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>

// int main() {
//     // Create a socket
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

//     // Configure the server address
//     struct sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(8080); // Port number
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     // Bind the socket to the address
//     bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

//     // Listen for incoming connections
//     listen(serverSocket, 5);

//     std::cout << "Server is running on port 8080..." << std::endl;

//     while (true) {
//         // Accept a connection
//         int clientSocket = accept(serverSocket, NULL, NULL);

//         // Receive data from the client
//         char buffer[1024];
//         memset(buffer, 0, sizeof(buffer));
//         recv(clientSocket, buffer, sizeof(buffer), 0);

//         // Check if the request contains form data
//         std::string request(buffer);
//         size_t pos = request.find("\r\n\r\n");
//         if (pos != std::string::npos) {
//             std::string formData = request.substr(pos + 4);
            
//             // Process the form data (in this example, we simply print it)
//             std::cout << "Received Form Data:\n" << formData << std::endl;
//         }

//         // Simple HTTP response with a form
//         std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 140\r\n\r\n<html><body><form method='POST' action='/submit'><input type='text' name='name'><br><input type='submit' value='Submit'></form></body></html>";

//         // Send the response to the client
//         send(clientSocket, response.c_str(), response.size(), 0);

//         // Close the client socket
//         close(clientSocket);
//     }

//     // Close the server socket (this code is unreachable in this example)
//     close(serverSocket);

//     return 0;
// }
