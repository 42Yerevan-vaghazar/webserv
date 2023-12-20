#include <string>
#include <fstream>
#include <sstream>
#include <HelperFunctions.hpp>
#include <unistd.h>
#include <iostream>

#define READ_SIZE 2000
#define WRITE_SIZE 2000

std::string fileToString(std::string const &fileName) {
    std::ifstream file(fileName);
	std::string fileContent;

    if (file.is_open()) {
		std::ostringstream stream;
		
		stream << file.rdbuf();
		fileContent = stream.str();
    }  else {
		throw std::logic_error("can not open file");
	}
    file.close();
	return fileContent;
}

bool readFromFd(int fd, std::string &str) {
    char buf[READ_SIZE];
    int readSize = read(fd, buf, READ_SIZE);
    if (readSize == -1) {
        return (false);
    }
    if (readSize == 0) {
        return (true);
    }
    str.append(buf, readSize);
    // std::cout << "str = " << str << std::endl;
    return (false);
};

bool writeInFd(int fd, std::string &str) {

    int readSize = write(fd, str.c_str(), WRITE_SIZE);
    if (readSize == -1) {
        return (false);
    }
    str.erase(0, WRITE_SIZE);
    return (str.empty());
};