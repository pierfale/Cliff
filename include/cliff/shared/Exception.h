#ifndef _CLIFF_EXCPTION_H
#define _CLIFF_EXCPTION_H

#include <stdexcept>
#include <sstream>
#include <csignal>

namespace cliff {

	namespace exception {

#ifdef _WIN32
#define LAUNCH_BREAKPOINT //__debugbreak();
#else
#define LAUNCH_BREAKPOINT
#endif

#define	THROW(TYPE, ...) {\
			LAUNCH_BREAKPOINT;\
			TYPE __exception(__VA_ARGS__);\
			__exception.set_context(__FILE__, __PRETTY_FUNCTION__, __LINE__, #TYPE);\
			throw __exception;\
		}

		class Exception : public std::exception {

		public:
			Exception();
			Exception(const std::string& message);
			void set_context(const char* file, const char* function, unsigned int line, const char* exception_type) throw ();
			void set_message(const std::string& message) throw ();

			virtual const char* what() const throw();

		private:
			const char* _file;
			const char* _function;
			unsigned int _line;
			const char* _exception_type;
			std::string _message;
		};

		class UserMessage : public Exception {


		public:
			enum Level {
				Information,
				Warning,
				Error
			};

			UserMessage(Level level, const std::string& message) : Exception(message), _level(level) {

			}

			Level level() const {
				return _level;
			}

		private:
			Level _level;
		};


		class BadLineCommand : public Exception {

		public:
			BadLineCommand(std::string& message) {
				set_message(message);
			}
		};


		class FileNotFound : public Exception {

		public:
			FileNotFound(const std::string& filename) {
				set_message("Unable to open file \""+filename+"\". Does the file exist?");
			}

			FileNotFound(const char* filename) {
				set_message("Unable to open file \""+std::string(filename)+"\". Does the file exist?");
			}
		};

		class OutOfRange : public Exception {

		public:
			OutOfRange(int min_boundary, int max_boundary, int value) {
				set_message("Index "+std::to_string(value)+" out of range ["+std::to_string(min_boundary)+";"+std::to_string(max_boundary)+"[");
			}

		};

		class ElementNotFound : public Exception {

		public:
			ElementNotFound(const std::string& element) {
				set_message("Unable to find element \""+element+"\"");
			}

		};

	}
}
#endif
