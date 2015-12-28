#include "cliff/Core.h"

using namespace cliff;

int Core::execute(const ProgramOption& program_option) {



	//
	// Pre Compiler Pass
	//
	ModuleList<Module::Order::PreCompiler>::initialize();
	program_option.process(Module::Order::PreCompiler);

	return 0;
}
