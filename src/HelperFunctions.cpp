#include <string>
#include <fstream>
#include <sstream>
#include <HelperFunctions.hpp>
#include <unistd.h>
#include <iostream>
#include "DefaultSetup.hpp"


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
    char buf[READ_BUFFER];
    int readSize = read(fd, buf, READ_BUFFER);

    if (readSize == -1) {
        return (true);
    }
    if (readSize == 0) {
        return (true);
    }
    str.append(buf, readSize);
    return (false);
};

bool writeInFd(int fd, std::string &str) {
    size_t writeSize = str.size() < WRITE_BUFFER ?  str.size() : WRITE_BUFFER;

    int readSize = write(fd, str.c_str(), writeSize);
    if (readSize == -1) {
        return (false);
    }
    str.erase(0, readSize);
    return (str.empty());
};

long double	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
