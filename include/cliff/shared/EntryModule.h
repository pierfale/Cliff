#ifndef _ENTRY_MODULE_H
#define _ENTRY_MODULE_H

#include "cliff/shared/Module.h"
#include "cliff/ProgramOption.h"

namespace cliff {

	class EntryModule : public Module {

	public:
		virtual void execute(ProgramOption::Iterator option_caller) = 0;

	};
}

#endif
