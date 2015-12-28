#ifndef _CLIFF_LEXER_GENERATOR_H
#define _CLIFF_LEXER_GENERATOR_H

#include <stack>

#include "cliff/front_end/Syntax.h"
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/Automata.h"
#include "cliff/shared/MemoryContainer.h"

namespace cliff {

	class LexerGenerator {

	public:
		static void generate_lexer(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, Syntax& output_syntax);

	private:
		LexerGenerator() = delete;

		static NonDeterministeFiniteAutomataNode& create_regular_expression_nfa(const Syntax& ebnf_syntax, const AbstractSyntaxTree& current_tree_node, NonDeterministeFiniteAutomataNode& start_automata_node, std::vector<Automata::Symbol>& alphabet, Syntax& output_syntax);
		static void create_dfa(const Syntax& ebnf_syntax, const NonDeterministeFiniteAutomataNode& start_nfa_node, DeterministeFiniteAutomataNode& start_dfa_node, const std::vector<Automata::Symbol>& alphabet);
		static bool epsilon_closure(const NonDeterministeFiniteAutomataNode& nfa_node, std::vector<const NonDeterministeFiniteAutomataNode*>& output_list, Automata::Symbol letter = Automata::Epsilon);
		static void reduce_dfa(const Syntax& ebnf_syntax, DeterministeFiniteAutomataNode& start_node, const std::vector<Automata::Symbol>& alphabet);
		static bool is_equal_transition(const DeterministeFiniteAutomataNode* node_1, const DeterministeFiniteAutomataNode* node_2);
		static void generate_lexer(Syntax& output_syntax, const DeterministeFiniteAutomataNode& start_node);
	};
}
#endif
