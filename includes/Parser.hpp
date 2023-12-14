/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmartiro <dmartiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 00:25:27 by dmartiro          #+#    #+#             */
/*   Updated: 2023/12/13 17:13:43 by dmartiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP
#include "Libs.hpp"
#include "ServerManager.hpp"

class Location;
class HTTPServer;
class HTTPRequest;
class ServerManager;

class Parser
{
    private:
    enum p_type
    {
        WORD, // general type of token for CONTEXT && DIRECTIVE
        CONTEXT, // for server and location contexts
        OPENBRACE, // {
        CLOSEBRACE, // }
        DIRECTIVE, // property for server
        DIRECTIVE_VALUE, // value for property
        SEMICOLON, // ;
        CONTEXT_PREFIX, //For location context
        SEPARATOR // \a
    };
    public:
        Parser(const char *confFile);
        ~Parser();
    public:
        void start(ServerManager &mgn);
    private:
        std::string context_keyword(std::string const &context_token);
    private:
        size_t contextWord(std::string const &config, size_t p);
        void removeUnprintables(std::vector<std::string> &tmp_ctx);
        bool isWord(char s);
        int context_wrap(std::string const &server);
        void tolower(std::string &s);
        void addWord(std::string config, size_t *i);
        void addToken(char s, p_type type);
        void clean( void );
        void scheme( void );
        void semantic_analysis( void );
        void intermediate_code_generation( void );
        void syntax_analysis( void );
        void fill_servers(ServerManager &mgn);
        void remove_spaces(std::string &tmp_text);
    private:
        std::fstream IO;
        std::map<std::string, void (Parser::*)(std::string &, std::string &, HTTPServer &)>directives;
        struct Token
        {
            p_type type;
            std::string token;   
        };
        std::list<Token> tokens;
        std::stack<std::string> braces;
        std::string config;
        std::vector<std::string> server_ctx;
    private:
        void create_server(ServerManager &mgn, std::list<Token>::iterator& ch);
        void location(std::list<Token>::iterator& next, HTTPServer& srv);
        void directive(std::list<Token>::iterator& next, Location& loc); //Location directives
        void directive(std::list<Token>::iterator& next, HTTPServer& srv); // Server directives
        void make_pair(size_t i, std::list<Token>::iterator& node, HTTPServer &srv);
    private:
        void d_listen(std::string &d_key, std::string &d_val, HTTPServer &srv);
        void d_root(std::string &d_key, std::string &d_val, HTTPServer &srv);
        void d_server_name(std::string &d_key, std::string &d_val, HTTPServer &srv);
        void d_index(std::string &d_key, std::string &d_val, HTTPServer &srv);
        void d_autoindex(std::string &d_key, std::string &d_val, HTTPServer &srv);
        void d_methods(std::string &d_key, std::string &d_val, HTTPServer &srv);
        void d_err_page(std::string &d_key, std::string &d_val, HTTPServer &srv);
        void d_body_size(std::string &d_key, std::string &d_val, HTTPServer &srv);

};
typedef std::map<std::string, void (Parser::*)(std::string &, std::string &, HTTPServer &)>::iterator Func;

// server, location
//root, index, autoindex, error_page, methods, client_body_size, server_name, listen

#endif