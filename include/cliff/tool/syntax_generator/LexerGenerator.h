#ifndef _CLIFF_SYNTAX_GENERATOR_LEXER_GENERATOR_H
#define _CLIFF_SYNTAX_GENERATOR_LEXER_GENERATOR_H

#include <stack>

#include "cliff/front_end/Syntax.h"
#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/Automata.h"
#include "cliff/shared/MemoryContainer.h"


namespace cliff {

	class LexerGenerator {

	public:
		static void generate_lexer(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, Syntax& output_syntax, SyntaxRepresentation& syntax_representation, std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list, DeterministeFiniteAutomataNode& dfa_start);

	private:
		LexerGenerator() = delete;

		static void get_terminal_content(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, std::vector<const char*>& symbols_content);
		static void create_dfa(const Syntax& ebnf_syntax, const NonDeterministeFiniteAutomataNode& start_nfa_node, DeterministeFiniteAutomataNode& start_dfa_node);
		static bool epsilon_closure(const NonDeterministeFiniteAutomataNode& nfa_node, std::vector<const NonDeterministeFiniteAutomataNode*>& output_list, LetterRange range);
		static void reduce_dfa(const Syntax& ebnf_syntax, DeterministeFiniteAutomataNode& start_node);
		static void segment_output_range(const NonDeterministeFiniteAutomataNode& nfa_node, std::map<LetterRange, std::vector<const NonDeterministeFiniteAutomataNode*>>& output_list);
		static bool is_equal_transition(const DeterministeFiniteAutomataNode* node_1, const DeterministeFiniteAutomataNode* node_2);
		static void generate_lexer(Syntax& output_syntax, const DeterministeFiniteAutomataNode& start_node, std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list);
	};
}
#endif
