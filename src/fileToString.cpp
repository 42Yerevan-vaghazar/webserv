#include <string>
#include <fstream>
#include <sstream>

std::string fileToString(std::string const &fileName) {
    std::ifstream file(fileName);
	std::string fileContent;

    if (file.is_open()) {
		std::ostringstream stream;
		
		stream << file.rdbuf();
		fileContent = stream.str();
    }  else {
		throw std::logic_error("can not open file"); // TODO replace with Error exception
	}
    file.close();
	return fileContent;
}