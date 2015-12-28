#ifndef _CLIFF_PARSER_H
#define _CLIFF_PARSER_H

#include "cliff/front_end/Syntax.h"
#include "cliff/shared/MemoryContainer.h"
#include "cliff/shared/AbstractSyntaxTree.h"

namespace cliff {

	class Parser {

	public:
		Parser(const Syntax& syntax);

		AbstractSyntaxTree& execute(const std::vector<Token>& input, MemoryContainer<AbstractSyntaxTree>& tree_memory);

	private:
		const Syntax& _syntax;

	};
}

#endif
