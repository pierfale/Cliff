#include "cliff/front_end/Reader.h"

using namespace cliff;

Reader::Reader(const char* filename) : _file(filename, std::ios::in), _current_filename(filename), _current_line(0), _current_column(0), _last_char_cr(false) {
	if(!_file.is_open())
		THROW(exception::FileNotFound, filename);
}

bool Reader::process_next(Syntax::Letter& output) {
	output = _file.get();



	if(output == '\r' || (output == '\n' && !_last_char_cr)) {
		_current_line++;
		_current_column = 0;
	}
	else {
		_current_column++;

		// manage the tabulation case
	}

	_last_char_cr = output == '\r';

	return !_file.eof();
}

const char* Reader::current_filename() const {
	return _current_filename;
}

unsigned int Reader::current_line() const {
	return _current_line;
}

unsigned int Reader::current_column() const {
		return _current_column;
}
