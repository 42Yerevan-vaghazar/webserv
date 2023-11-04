#pragma once

#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
class Cgi
{
    public:
        static std::string execute(const std::string &filePath, const std::string &progName, int fd, char **env) {
            std::string progPath = "/usr/bin/" + progName;
            std::string htmlFilePath = "./tmp/index.html";
            char **argv = new char *[3];
            argv[0] = (char *)progPath.c_str(); //TODO
            argv[1] = (char *)filePath.c_str();
            argv[2] = NULL;

            int pid = fork();
            if (pid == -1) {
                throw std::runtime_error(std::string("fork: fork failed") + strerror(errno));
            }
            if (pid == 0) {
                dup2(fd, 1);
                close(fd);
                if (progName == "php" || progName == "python") {
                    std::cerr << "execve = " << execve(progPath.c_str(), argv, env) << std::endl; // TODO dont print
                }
                exit(1);
            }
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                throw std::runtime_error(std::string("waitpid: ") + strerror(errno));
            };
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) != 0) {
                    throw std::runtime_error(progName + std::string(": failed"));
                }
            }
            delete [] argv;
            return (htmlFilePath);
        };
    private:
        Cgi();
        Cgi(const Cgi &rhs);
        Cgi &operator=(const Cgi &rhs);
        ~Cgi();
};