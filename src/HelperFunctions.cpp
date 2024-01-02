#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include "DefaultSetup.hpp"
#include "HelperFunctions.hpp"


std::string fileToString(std::string const &fileName) {
    std::ifstream file(fileName.c_str());
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
    // std::cout << "readSize = " << readSize << std::endl;
    if (readSize == -1) {
        return (true);
    }
    if (readSize == 0) {
        return (true);
    }
    str.append(buf, readSize);
    return (false);
};

std::ofstream wofs("writeInFd.log");

bool writeInFd(int fd, std::string &str) {
    if (str.size() != 0) {
        size_t writeSize = str.size() < WRITE_BUFFER ?  str.size() : WRITE_BUFFER;

        int res = write(fd, str.c_str(), writeSize);
        wofs << "res = " << res << std::endl;
        if (res == -1) {
            return (false);
        }
        str.erase(0, res);
    }
    return (str.empty());
};

long double	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

std::string my_to_string(size_t num) {
    std::stringstream str;
    str << num;
    return (str.str());
};