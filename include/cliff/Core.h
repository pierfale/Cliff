#ifndef _CLIFF_CORE_H
#define _CLIFF_CORE_H

#include "cliff/ProgramOption.h"
#include "cliff/shared/Module.h"

namespace cliff {

	class Core {

	public:

		static int execute(const ProgramOption& program_option);

	};
}
#endif
