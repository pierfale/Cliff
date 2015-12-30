#include "cliff/Core.h"

using namespace cliff;

int Core::execute(const ProgramOption& program_option) {



	//
	// Pre Compiler Pass
	//

	try {
		ModuleList<Module::Order::PreCompiler>::initialize();
		program_option.process(Module::Order::PreCompiler);
	}
	catch(const exception::UserMessage& e) {
		switch (e.level()) {
		case exception::UserMessage::Information:
			std::cout << "[Information] ";
			break;
		case exception::UserMessage::Warning:
			std::cout << "[Warning] ";
			break;
		case exception::UserMessage::Error:
			std::cout << "[Error] ";
			break;
		}
		std::cout << e.what() << std::endl;
	}

	return 0;
}
