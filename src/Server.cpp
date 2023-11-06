#include "Server.hpp"

Server::Server(const std::string &ipAddress, int port)
            : _port(port), _ipAddress(ipAddress) {};

Server::~Server() {};

bool Server::start() {
    int a = 1;
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof(int)) < 0) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
    }
    // std::cout << "sockaddr_in = " << sizeof(struct sockaddr_in) << std::endl;
    // std::cout << "sockaddr_storage = " << sizeof(struct sockaddr_storage) << std::endl;
    // fcntl(_serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    struct sockaddr_in addr; // TODO sockaddr_storage
    memset(&addr , 0 , sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(_ipAddress.c_str());
    addr.sin_port = htons(_port);
    std::cout << "addr.sin_port = " << addr.sin_port << std::endl;
    std::cout << "addr.sin_addr.s_addr = " << addr.sin_addr.s_addr << std::endl;

    if (bind(_serverSocket, (sockaddr *)&addr, sizeof(addr)) == -1) {
        std::runtime_error(std::string("bind: ") + strerror(errno));
    }

    if (listen(_serverSocket, 0) == -1) {
        std::runtime_error(std::string("listen: ") + strerror(errno));
    };
    eventLoop();
    return (true);
};

void Server::eventLoop() {
    EvManager::start();
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    EvManager::addEvent(_serverSocket, EvManager::read);
    while(true) {
        std::pair<EvManager::Flag, int> event = EvManager::listen();
        // std::cout << "event.second = " << event.second << std::endl;
        // std::cout << "event.first = " << event.first << std::endl;
        if (event.second == _serverSocket) {
            // std::cout << "\n_serverSocket\n" << std::endl;
            Client client;
            // client.setFd(accept(_serverSocket, (struct sockaddr *)&client.getAddr(), &client.getAddrLen()));
            client.setFd(accept(_serverSocket, 0, 0));
            std::cout << _serverSocket << std::endl;
            // if (client.getFd() == -1) {  // TODO is it needed
            //     throw std::runtime_error(std::string("accept: ") + strerror(errno));
            // }
            fcntl(client.getFd(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
            EvManager::addEvent(client.getFd(), EvManager::read);
            _clients[client.getFd()] = client;
        } else {
            // std::cout << "else\n";
            std::map<int, Client>::iterator itClient = _clients.find(event.second);
            if (itClient == _clients.end()) {
                // std::cout << "\n\n\n\n\n\ncontinue\n\n\n\n\\n\n" << std::endl;
                continue ;
                // throw std::runtime_error(std::string("find: client is not found") + strerror(errno));  // is it true?
            }
            Client &client = itClient->second;
            // std::cout << client.getHttpRequest() << std::endl;;
            if (event.first == EvManager::eof) {
                // std::cout << "\nEV_EOF\n" << std::endl;
                closeConnetcion(client.getFd());
            } else if (event.first == EvManager::read) {
                // std::cout << "\nEVFILT_READ\n" << std::endl;
                if (client.getHttpRequest().empty()) {
                    EvManager::addEvent(client.getFd(), EvManager::write);
                }
                if (client.receiveMessage() == -1) {
                    closeConnetcion(client.getFd());
                }
                if (client.isRequestReady()) {
                    client.setResponse(generateResponse(client.getHttpRequest(), client.getBody()));
                }
            } else if (client.isResponseReady() && event.first == EvManager::write) {
                // std::cout << "\nEVFILT_WRITE\n" << std::endl;
                // TODO send response little by little
                if (client.sendMessage() == true) {
                    closeConnetcion(client.getFd());
                }
            } else if (client.isResponseReady() == false) {
                if (client.receiveMessage() == -1) {
                    closeConnetcion(client.getFd());
                }
                if (client.isRequestReady()) {
                    client.setResponse(generateResponse(client.getHttpRequest(), client.getBody()));
                }
            }
        }
    }
}

std::string Server::generateResponse(const std::string &httpRequest, const std::string &body) {
    size_t pos = httpRequest.find(' ');
    pos += 1;
    // std::cout << "httpRequest = " << httpRequest << std::endl;
    std::string response;
    if (httpRequest[0] == 'P') {
        response = post(httpRequest.substr(pos, httpRequest.find(' ', pos) - pos), body);
    } else if (httpRequest[0] == 'G') {
        std::string filePath;
        filePath += httpRequest.substr(pos, httpRequest.find(' ', pos) - pos);
        filePath = "./www" + filePath;
        size_t contentTypePos = filePath.rfind(".");
        std::string contentType = filePath.substr(contentTypePos + 1);
        std::cout << "filePath = " << filePath << std::endl;
        if (filePath == "./www/") {
            response = get("www/index.html", contentType);
        } else {
            response = get(filePath, contentType);
        }
    } else if (httpRequest[0] == 'D') {
        response = del(httpRequest.substr(pos, httpRequest.find(' ', pos) - pos));
    }
    // std::cout << "response = " << response << std::endl;
    // TODO send response little by little
    return (response);
}

std::string Server::get(const std::string &fileName, const std::string  &contentType) {
    std::unordered_map<std::string, std::string> headerContent;
    std::string response;
    int statusCode = 200;
    std::string status = " OK";
    std::cout << "contentType = " <<  contentType << std::endl;
    if (contentType == "png"){
        headerContent["Content-Type"] = "image/png";
    } else {
        headerContent["Content-Type"] = "text/html";
    }
    response += "HTTP/1.1 ";
    std::cout << "fileName = " << fileName << std::endl;
    if (access(fileName.c_str(), R_OK) == 0) {   // TODO check permission to read
        std::string fileContent;
        std::ostringstream stream;
        std::ifstream ifs(fileName);
        if (ifs.is_open() == false) {
            throw std::logic_error("can not open file");
        }
        stream << ifs.rdbuf();
        fileContent = stream.str();
        headerContent["Content-Length"] = std::to_string(fileContent.size());
        std::ofstream ofs("test.png");
        if (ofs.is_open() == false) {
            throw std::logic_error("Error opening file");
        }
        ofs << fileContent;
        // TODO if not html cgi works here to generate fileContent
        response += std::to_string(statusCode);
        response +=  status;
        response +=  "\r\n";

        for (std::unordered_map<std::string, std::string>::iterator it = headerContent.begin();
            it != headerContent.end(); ++it) {
                response += it->first;
                response += ": ";
                response += it->second;
                response += "\r\n";
        }
        response +=  "\n";
        response +=  fileContent;
    } else {
        // TODO automate it   404, 405, 411, 412, 413, 414, 431, 500, 501, 505, 503, 507, 508
        std::string fileContent;
        std::ifstream ifs("./error_pages/404.html");
        if (ifs.is_open() == false) {
            throw std::logic_error("can not open file");
        }
        std::getline(ifs, fileContent, '\0');
        headerContent["Content-Length"] = std::to_string(fileContent.size());
        response += std::to_string(404);
        response += "not found";
        response += "\r\n";

        for (std::unordered_map<std::string, std::string>::iterator it = headerContent.begin();
            it != headerContent.end(); ++it) {
                response += it->first;
                response += ": ";
                response += it->second;
                response += "\r\n";
        }
        response +=  "\n";
        response +=  fileContent;
    }
    return (response);
};

std::string Server::post(const std::string &fileName, const std::string &body) {
    std::string response;
    response += PROTOCOL;
    response += " 200 ";
    response += "OK";
    _data.push_back(body);
    // ofs << body;
    return (response);
};

std::string Server::del(const std::string &fileName) {
    std::string response;
    response += "HTTP/1.1 ";
    response += "200 ";
    response += "OK";
    std::remove(fileName.c_str());
    return (response);
};

bool Server::closeConnetcion(int fd) {
    EvManager::delEvent(fd, EvManager::read);
    EvManager::delEvent(fd, EvManager::write);
    close(fd);
    _clients.erase(fd);
    return (true);
};