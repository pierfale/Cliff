#ifndef _CLIFF_PROGRAM_OPTION_H
#define _CLIFF_PROGRAM_OPTION_H

#include <map>
#include <cstring>

#include "cliff/shared/Module.h"
#include "cliff/shared/Exception.h"

namespace cliff {

	class Option {

	};

	class ProgramOption {

		friend class Core;

	public:
		ProgramOption(int argc, const char** argv);

		const char* get_option(const char* reduced_name, const char* full_name) const;
		const char* get_next_argument(const char* iterator) const;


		class Iterator {

			friend class ProgramOption;

		public:
			Iterator& next_argument_or_else(std::string& error_message);
			const char* require_text_argument_or_else(std::string& error_message);

		private:
			Iterator(const ProgramOption& model, const char** args);

			const ProgramOption& _model;
			const char** _current_argument;

		};

	private:
		int _argc;
		const char** _argv;

		void process(Module::Order stage) const;

	};

}
#endif
