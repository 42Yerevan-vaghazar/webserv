#pragma once
#include <string>
#include <sys/time.h>

std::string fileToString(std::string const &fileName);
bool readFromFd(int fd, std::string &str);
bool writeInFd(int fd, std::string &str);
long double	get_current_time(void);
