#include "cliff/front_end/Reader.h"

using namespace cliff;

Reader::Reader(const char* filename) : _file(filename, std::ios::in) {
	if(!_file.is_open())
		THROW(exception::FileNotFound, filename);
}

bool Reader::process_next(Syntax::Letter& output) {
	output = _file.get();
	return !_file.eof();
}
