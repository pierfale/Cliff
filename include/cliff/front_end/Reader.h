#ifndef _CLIFF_READER_H
#define _CLIFF_READER_H

#include <fstream>

#include "cliff/front_end/FrontChain.h"

namespace cliff {

	class Reader {

	public:
		Reader(const char* filename);

		bool process_next(Syntax::Letter& output);

	private:
		std::ifstream _file;

	};
}

#endif
