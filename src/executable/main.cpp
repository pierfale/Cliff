#include "cliff/ProgramOption.h"
#include "cliff/Core.h"
using namespace cliff;

int main(int argc, const char** argv) {
	ProgramOption program_option(argc, argv);

	return Core::execute(program_option);
}
