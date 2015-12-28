#include "cliff/EntryBoundRegister.h"

using namespace cliff;

std::map<Module::Order, EntryBoundRegister::BindOption> EntryBoundRegister::_binds;

void EntryBoundRegister::bind_module_to_option(EntryModule* module, const char* reduced_name, const char* full_name, Module::Order stage) {
	auto it_stage = _binds.find(stage);

	if(it_stage == std::end(_binds)) {
		_binds.insert(std::make_pair(stage, BindOption()));
		it_stage = _binds.find(stage);
	}

	if(reduced_name != nullptr)
		it_stage->second._reduced_binds.insert(std::make_pair(reduced_name, module));

	if(full_name != nullptr)
		it_stage->second._full_binds.insert(std::make_pair(full_name, module));
}

void EntryBoundRegister::bind_module(EntryModule* module, Module::Order stage) {

}
