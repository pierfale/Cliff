#ifndef _CLIFF_REGULAR_EXPRESSION_REPRESENTATION_H
#define _CLIFF_REGULAR_EXPRESSION_REPRESENTATION_H

#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/MemoryContainer.h"
#include "cliff/shared/Automata.h"
#include "cliff/tool/syntax_generator/SyntaxNodeName.h"

namespace cliff {

	class RegularExpressionRepresentation {

	public:
		RegularExpressionRepresentation(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, const TokenSymbol& symbol_name);

		NonDeterministeFiniteAutomataNode& start_node();
		const NonDeterministeFiniteAutomataNode& start_node() const;


	private:
		NonDeterministeFiniteAutomataNode& construct(const Syntax& ebnf_syntax, const AbstractSyntaxTree& current_tree_node, NonDeterministeFiniteAutomataNode& current_automata_node);

		MemoryContainer<NonDeterministeFiniteAutomataNode> _memory;
		NonDeterministeFiniteAutomataNode _start_node;

	};
}
#endif
