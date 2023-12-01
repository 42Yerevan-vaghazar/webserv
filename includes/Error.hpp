    #pragma once
    
    class Error : public std::exception
    {
        public:
            Error(int statusCode, const std::string &errMessage)
                : _statusCode(statusCode), _errMessage(errMessage) {};
            Error(const Error& rhs) {
                _statusCode = rhs._statusCode;
                _errMessage = rhs._errMessage;
            };
            ~Error() throw() {};

            const char * what() const throw() {
                return _errMessage.c_str();
            }

            int getStatusCode() const {
                return (_statusCode);
            }

        private:
            Error() : _statusCode(0) {};
            Error& operator=(const Error& rhs);
        private:
            int _statusCode;
            std::string _errMessage;
    };