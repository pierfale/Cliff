#ifndef _CLIFF_PARSER_H
#define _CLIFF_PARSER_H

#include <stack>

#include "cliff/front_end/Syntax.h"
#include "cliff/front_end/FrontChain.h"

#include "cliff/shared/MemoryContainer.h"
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/front_end/Lexer.h"

namespace cliff {

	class Parser {

	public:
		Parser(const Syntax& syntax, Lexer& input);

		AbstractSyntaxTree& execute(MemoryContainer<AbstractSyntaxTree>& tree_memory);

	private:
		const Syntax& _syntax;
		Lexer& _input;

	};
}

#endif
