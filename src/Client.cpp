#include "Client.hpp"

Client::Client() : _fd(0), _bodySize(BODY_LIMIT), _isRequestReady(false), _isHeaderReady(false),
    _isBodyReady(false), _isOpenConnection(true), _isResponseReady(false), _maxSizeRequest(0) {
    memset(&_addrClient , 0 , sizeof(struct sockaddr));
};

Client::~Client() {
    // close(_fd);
};

bool Client::operator<(const Client& rhs) const
{
    return _fd < rhs._fd;
}

int Client::getFd() const {
    return (_fd);
}

void Client::setFd(const int fd) {
    _fd = fd;
}

void Client::closeFd() const {
    close(_fd);
}

struct sockaddr_in &Client::getAddr() {
    return (_addrClient);
}

void Client::setAddr(const struct sockaddr_in &addrClient) {
    _addrClient = addrClient;
}

std::string Client::getHttpRequest() {
    return (_httpRequest);
}

std::string Client::getBody() {
    return (_body);
}

socklen_t &Client::getAddrLen() {
    return (_sockLen);
}

void Client::setResponse(const std::string &response) {
    _response = response;
    _isResponseReady = true;
}

int Client::receiveMessage() {
    char buf[READ_BUFFER];
    int rdSize = recv(_fd, buf, sizeof(buf) - 1, 0);

    // std::cout << "rdSize = " << rdSize << std::endl;
    if (rdSize == -1) { // TODO Checking the value of errno is strictly forbidden after a read or a write operation.
        if (_maxSizeRequest == 1000) { // TODO client request caused infinit loop  change with time
            return -1;
        }
        _maxSizeRequest++;
    }
     if (rdSize == 0) {  // TODO close tab. send response?
        return (-1);
    }
    buf[rdSize] = '\0';
    if (_isHeaderReady == false) {
        _httpRequest += buf;
        size_t headerEndPos = _httpRequest.find("\n\r\n");

        if (headerEndPos == std::string::npos) {
            return 0;
        }
        _isHeaderReady = true;
        size_t pos = _httpRequest.find("Content-Length: ");
        std::cout << "pos = " << pos << std::endl;
        if (pos == std::string::npos) {
            _bodySize = 0;
        } else {
            _bodySize = std::stoi(_httpRequest.substr(_httpRequest.find("Content-Length: ") + strlen("Content-Length: "), 10));  // TODO check this
        }

        std::string tmp = _httpRequest.substr(headerEndPos + 3);
        _httpRequest.erase(headerEndPos);
        if (_bodySize != 0) {
            _body = tmp;
            std::cout << "tmp = " << tmp << std::endl;
        }
        // TODO  parse header
        return 0;
    }
    if (_bodySize <= _body.size()) {   // TODO check body length to do so
        _body.erase(_bodySize);
        _isBodyReady = true;
        _isRequestReady = true;
        return 0;
    }
    _body += buf;
    return 0;
}

bool Client::sendMessage() {
    size_t sendSize = WRITE_BUFFER < _response.size() ? WRITE_BUFFER : _response.size();
    if (send(_fd, _response.c_str(), sendSize, 0) == -1) {
        perror("send :");
        exit(1);
    }
    _response.erase(0, sendSize);
    // _response.clear();
    // _isResponseReady = false;
    return (_response.empty());
}

bool Client::isRequestReady() const {
    return (_isRequestReady);
}

bool Client::isResponseReady() const {
    return (_isResponseReady);
}