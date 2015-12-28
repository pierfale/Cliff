#include "cliff/ProgramOption.h"
#include "cliff/EntryBoundRegister.h"

using namespace cliff;

ProgramOption::ProgramOption(int argc, const char** argv) : _argc(argc), _argv(argv) {

}

const char* ProgramOption::get_option(const char* reduced_name, const char* full_name) const {

}

const char* ProgramOption::get_next_argument(const char* iterator) const {

}


void ProgramOption::process(Module::Order stage) const {
	auto it_stage = EntryBoundRegister::_binds.find(stage);

	if(it_stage != std::end(EntryBoundRegister::_binds)) {
		for(unsigned int i=1; i<_argc; i++) {
			if(_argv[i][0] != '-') {

			}
			else if(_argv[i][0] == '-' && _argv[i][1] != '-') {
				auto it_module = it_stage->second._reduced_binds.find(_argv[i]+1);
				if(it_module != std::end(it_stage->second._reduced_binds))
					it_module->second->execute(Iterator(*this, _argv+i));
			}
			else if(_argv[i][0] == '-' && _argv[i][1] == '-') {
				auto it_module = it_stage->second._full_binds.find(_argv[i]+2);
				if(it_module != std::end(it_stage->second._full_binds))
					it_module->second->execute(Iterator(*this, _argv+i));
			}
		}
	}
}

//
//	ProgramOption::Iterator
//

ProgramOption::Iterator::Iterator(const ProgramOption& model, const char** current_argument) : _model(model), _current_argument(current_argument) {

}

ProgramOption::Iterator& ProgramOption::Iterator::next_argument_or_else(std::string& error_message) {
	_current_argument++;

	if(_current_argument >= _model._argv+_model._argc)
		THROW(exception::BadLineCommand, error_message);

	return *this;
}

const char* ProgramOption::Iterator::require_text_argument_or_else(std::string& error_message) {
	if((*_current_argument)[0] == '-')
		THROW(exception::BadLineCommand, error_message);

	return *_current_argument;
}
