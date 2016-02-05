#ifndef _CLIFF_FRONT_CHAIN_H
#define _CLIFF_FRONT_CHAIN_H

#include <istream>

#include "cliff/front_end/Syntax.h"
#include "cliff/shared/MemoryContainer.h"
#include "cliff/shared/AbstractSyntaxTree.h"

namespace cliff {

	template<typename Previous, typename Current, typename Data>
	class FrontUnit {

	public:
		FrontUnit(Previous& previous) : _previous(previous) {

		}

		Data& next_data(unsigned int current_state) {

		}

		void process_all() {
			while(Current::process_next());
		}

	private:
		Previous& _previous;
	};
}
#endif
