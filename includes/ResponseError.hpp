    #pragma once
    #include <iostream>

    class ResponseError : public std::exception
    {
        public:
            ResponseError(int statusCode, const std::string &errMessage)
                : _statusCode(statusCode), _errMessage(errMessage) {};
            ResponseError(const ResponseError& rhs) {
                _statusCode = rhs._statusCode;
                _errMessage = rhs._errMessage;
            };
            ~ResponseError() throw() {};

            const char * what() const throw() {
                return _errMessage.c_str();
            }

            int getStatusCode() const {
                return (_statusCode);
            }

        private:
            ResponseError() : _statusCode(0) {};
            ResponseError& operator=(const ResponseError& rhs);
        private:
            int _statusCode;
            std::string _errMessage;
    };