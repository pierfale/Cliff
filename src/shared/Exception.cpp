#include "cliff/shared/Exception.h"

using namespace cliff;
using namespace exception;

Exception::Exception() : _file(nullptr), _function(nullptr), _line(0), _exception_type(nullptr), _message() {

}

Exception::Exception(const std::string& message) : _file(nullptr), _function(nullptr), _line(0), _exception_type(nullptr), _message(message) {

}

void Exception::set_context(const char* file, const char* function, unsigned int line, const char* exception_type)  throw() {
	_file = file;
	_function = function;
	_line = line;
	_exception_type = exception_type;
}

void Exception::set_message(const std::string& message) throw() {
	_message = message;
}

const char* Exception::what() const throw() {
	std::ostringstream stream;

	if(_file != nullptr && _function != nullptr && _exception_type != nullptr) {
		stream << "[" << _exception_type << "] " << (_message.empty() ? "no user message" : _message) << "\n";
		stream << "At " << _function << "\n" << "File \"" << _file << "\" line " << _line << "\n";
	}
	else {
		stream << (_message.empty() ? "no user message" : _message) << "\n";
	}

	return stream.str().c_str();
}
