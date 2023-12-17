#include "Cgi.hpp"

std::map<std::string, std::string> Cgi::_env;

 #include <unistd.h>
int Cgi::execute(const Client &client) {
    char **argv = new char *[3];
    std::string argv1 = getcwd(NULL, 0) + std::string("/") + client.getSrv().getCgi(client.getExtension()).second;
    argv[0] = const_cast<char *>(argv1.c_str());
    std::string argv2 =  getcwd(NULL, 0) + std::string("/") + client.getPath();
    argv[1] = const_cast<char *>(argv2.c_str());
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
    char **envp = Cgi::initEnv(client);
    // int i = 0;
    // while (envp[i]) {
    //     printf("%s\n", envp[i++]);
    // }

    if (pid == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        std::cout << "execve = " << execve(argv[0], argv, envp) << std::endl;
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
    const HTTPServer &srv = client.getSrv();

    // client.showHeaders();
    pwd = getcwd(NULL, 0);
    // _env["SERVER_NAME"] = client.findInMap("Host");
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    // _env["SERVER_SOFTWARE"] = "Webserv";
    _env["REDIRECT_STATUS"] = "true";
    // _env["UPLOAD_DIR"] = pwd + std::string("/") + std::string("data_base");
    _env["CONTENT_LENGTH"] = client.findInMap("content-length");
    // _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    // _env["CONTENT_TYPE"] = "";
    // _env["PATH_INFO"] = "/Users/vaghazar/Desktop/webserv/CGI_Interpreters/php-cgi-mac";
    // _env["REQUEST_METHOD"] = client.getMethod();
    // _env["QUERY_STRING"] = client.getQueryString();
    // _env["REMOTE_ADDR"] = ;
    // _env["SCRIPT_NAME"] = std::string(pwd) + "/" + "www/server1/index.php";
    // _env["SCRIPT_FILENAME"] = std::string(pwd) + "/" + "www/server1/";
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