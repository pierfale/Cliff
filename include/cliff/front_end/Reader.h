#ifndef _CLIFF_READER_H
#define _CLIFF_READER_H

#include <fstream>

#include "cliff/front_end/FrontChain.h"

namespace cliff {

	class Reader {

	public:
		Reader(const char* filename);

		bool process_next(Syntax::Letter& output);

		const char* current_filename() const;
		unsigned int current_line() const;
		unsigned int current_column() const;


	private:
		std::ifstream _file;
		const char* _current_filename;
		unsigned int _current_line;
		unsigned int _current_column;

		bool _last_char_cr;

	};
}

#endif
