#pragma once

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

class Cgi
{
    public:
        static std::string execute(const std::string &filePath, const std::string &progName, char **env) {
            std::string progPath = "/usr/bin/" + progName;
            std::string htmlPath = "./tmp/index.html";
            char **argv = new char *[3];
            argv[0] = (char *)progPath.c_str(); //TODO
            argv[1] = (char *)filePath.c_str();
            argv[2] = NULL;

            int pid = fork();
            if (pid == -1) {
                throw std::runtime_error("fork: fork failed");
            }
             if (pid == 0) {
                int fd = open(htmlPath.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0664);
                if (fd == -1) {
                    throw std::runtime_error("open: can not open");
                }
                dup2(fd, 1);
                close(fd);
                if (progName == "php" || progName == "python") {
                    std::cout << "execve = " << execve(progPath.c_str(), argv, env) << std::endl;
                }
                exit(1);
            }
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) != 0) {
                    throw std::runtime_error("execute: failed");
                }
            }
            delete [] argv;
            return (htmlPath);
        };
    private:
        Cgi();
        Cgi(const Cgi &rhs);
        Cgi &operator=(const Cgi &rhs);
        ~Cgi();
    private:
        std::string _prog;

};