#pragma once
#include <string>

std::string fileToString(std::string const &fileName);
bool readFromFd(int fd, std::string &str);
bool writeInFd(int fd, std::string &str);