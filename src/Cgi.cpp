#include "Cgi.hpp"

std::map<std::string, std::string> Cgi::_env;


int Cgi::execute(const Client &client) {
    char **argv = new char *[3];
    argv[0] = "php"; //TODO
    argv[1] = const_cast<char *>(client.getPath().c_str());
    argv[2] = NULL;


    int fd[2];

    if (pipe(fd) == -1) {
        delete [] argv;
        throw std::runtime_error(std::string("pipe:") + strerror(errno));
    }

    int pid = fork();
    if (pid == -1) {
        delete [] argv;
        throw std::runtime_error(std::string("fork:") + strerror(errno));
    }
    if (pid == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        std::cout << "execve = " << execve(client.getCgiPath().c_str(), argv, Cgi::initEnv(client)) << std::endl;
        perror("execve: ");
        exit(1);
    }
    delete [] argv;
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) != 0) {
            throw std::runtime_error("execute: failed");
        }
    }
    close(fd[1]);
    return (fd[0]);
};

char **Cgi::initEnv(Client const &client)
{
    char *pwd;

    pwd = getcwd(NULL, 0);
    _env["SERVER_NAME"] = "webserv";
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["SERVER_SOFTWARE"] = "Webserv";
    _env["REDIRECT_STATUS"] = "true";
    //_env["UPLOAD_DIR"] = pwd + (std::string)"/" + client.getUploadDir();
    _env["CONTENT_LENGTH"] = client.findInMap("content-length");
    // _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["CONTENT_TYPE"] = client.findInMap("content-type");
    _env["PATH_INFO"] = client.getPath();
    _env["REQUEST_METHOD"] = client.getMethod();
    _env["QUERY_STRING"] = client.getQueryString();
    //_env["REMOTE_ADDR"] = this->header["host"];
    //_env["SCRIPT_NAME"] = findscript(this->header["uri"]);
    //_env["SCRIPT_FILENAME"] = std::string(pwd) + "/" + this->cont->getFile();
    _env["SERVER_PORT"] = client.getServerPort();
    //_env["ORIGIN"] = this->header["origin"];
    // std::map<std::string, std::string>::iterator it = _env.begin();

    // while (it != _env.end()) {
    //     std::cout << it->first << " " << it->second << std::endl;
    //     ++it;
    // }
	free(pwd);

    char **envp = new char *[_env.size() + 1];

	int i = 0;

	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it)
	{
		envp[i++] = strdup((it->first + "=" + it->second).c_str());
	}

	envp[i] = NULL;
	return envp;
};