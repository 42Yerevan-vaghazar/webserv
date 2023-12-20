#include "Cgi.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

std::map<std::string, std::string> Cgi::_env;

int Cgi::execute(const Client &client) {
    char **argv = new char *[3];
    const std::string &argv1 = client.getSrv().getCgi(client.getExtension()).second;
    argv[0] = const_cast<char *>(argv1.c_str());
    const std::string &argv2 =  client.getPath();
    argv[1] = const_cast<char *>(argv2.c_str());
    argv[2] = NULL;
    int pipe_from_child[2];
    int pipe_to_child[2];
    if (pipe(pipe_from_child) == -1 || pipe(pipe_to_child)) {
        delete [] argv;
        throw ResponseError(500, "Internal Server Error");
    }

    fcntl(pipe_to_child[1], F_SETFL, O_NONBLOCK, O_CLOEXEC);
	fcntl(pipe_from_child[0], F_SETFL, O_NONBLOCK, O_CLOEXEC);

    write(pipe_to_child[1], client.getRequestBody().c_str(), client.getRequestBody().size());
    close(pipe_to_child[1]);
    int pid = fork();

    if (pid == -1) {
        delete [] argv;
        throw ResponseError(500, "Internal Server Error");
    }
    char **envp = Cgi::initEnv(client);

    if (pid == 0) {
        dup2(pipe_from_child[1], 1);
        close(pipe_from_child[0]);
        close(pipe_from_child[1]);
        dup2(pipe_to_child[0], 0);
        close(pipe_to_child[0]);
        int res = execve(argv[0], argv, envp);
        perror("execve: ");
        exit(res);
    }
    close(pipe_from_child[1]);
    delete [] argv;
    int status;
    usleep(100);
    // sleep(5);
    // std::cout << "status = " << status << std::endl;
    // TODO move to generate response function
    // std::cout << "kill = " << kill(pid, SIGKILL) << std::endl;
    // std::cout << "kill = " << kill(pid, SIGUSR1) << std::endl;  // TODO throw if everything is fine
    waitpid(pid, &status, 0);
    // std::cout << "status = " << status << std::endl;
    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) != 0) {
            std::cout << "WEXITSTATUS(status) = " << WEXITSTATUS(status) << std::endl;
            throw ResponseError(500, "Internal Server Error");
        }
    }
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL) {
        throw ResponseError(508, "Loop Detected");
    }
    return (pipe_from_child[0]);
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