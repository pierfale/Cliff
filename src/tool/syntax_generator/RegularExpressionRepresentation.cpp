#include "cliff/tool/syntax_generator/RegularExpressionRepresentation.h"

using namespace cliff;

RegularExpressionRepresentation::RegularExpressionRepresentation(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, const TokenSymbol& symbol_name)
	: _memory(), _start_node(_memory){

	assert(syntax_tree.type() == ebnf_syntax.get_symbol_from_name(SyntaxNodeName::RegularExpression));

	NonDeterministeFiniteAutomataNode& final_node = construct(ebnf_syntax, syntax_tree, _start_node);
	final_node.add_accepting_state(symbol_name);
}

NonDeterministeFiniteAutomataNode& RegularExpressionRepresentation::construct(const Syntax& ebnf_syntax, const AbstractSyntaxTree& current_tree_node, NonDeterministeFiniteAutomataNode& current_automata_node) {
	if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name(SyntaxNodeName::RegularExpressionLetter)) {
		assert(std::strlen(current_tree_node.content()) == 1);
		return current_automata_node.create_output_node(LetterRange(current_tree_node.content()[0]));

	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name(SyntaxNodeName::RegularExpressionRange)) {
		if(std::strcmp(current_tree_node.children()[0]->content(), "[") == 0) { // TODO different between [ and ^]

		}
		else if(std::strcmp(current_tree_node.children()[0]->content(), "[^") == 0) {
			LetterRange range(LetterRange::Min_letter, LetterRange::Max_letter);
			for(auto it_child = std::begin(current_tree_node.children())+1; it_child != std::end(current_tree_node.children())-1; ++it_child) {
				if((*it_child)->children().size() == 1)
					range -= LetterRange((*it_child)->children()[0]->content()[0]);
				else if((*it_child)->children().size() == 3)
					range -= LetterRange((*it_child)->children()[0]->content()[0], (*it_child)->children()[2]->content()[0]);
			}

			return current_automata_node.create_output_node(range);
		}
	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name(SyntaxNodeName::RegularExpressionAlternative)) {
		NonDeterministeFiniteAutomataNode& last_node = current_automata_node.create_node();
		for(const AbstractSyntaxTree* node : current_tree_node.children()) {
			NonDeterministeFiniteAutomataNode& last_alt_node = construct(ebnf_syntax, *node, current_automata_node.create_output_node(LetterRange::Epsilon_range));
			 last_alt_node.create_output_transition_with(last_node, LetterRange::Epsilon_range);
		}
		return last_node;
	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name(SyntaxNodeName::RegularExpressionRepetition)) {
		NonDeterministeFiniteAutomataNode& start_automata_node_inner = current_automata_node.create_output_node(LetterRange::Epsilon_range);

		NonDeterministeFiniteAutomataNode& end_automata_node_inner = construct(ebnf_syntax, *(current_tree_node.children()[0]), start_automata_node_inner);
		NonDeterministeFiniteAutomataNode& last_node = end_automata_node_inner.create_output_node(LetterRange::Epsilon_range);

		current_automata_node.create_output_transition_with(last_node, LetterRange::Epsilon_range);
		end_automata_node_inner.create_output_transition_with(start_automata_node_inner, LetterRange::Epsilon_range);
		return last_node;
	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name(SyntaxNodeName::RegularExpression)) {
		NonDeterministeFiniteAutomataNode* current_start_automata_node = &current_automata_node;
		for(auto it_node = std::begin(current_tree_node.children()); it_node != std::end(current_tree_node.children()); ++it_node) {
			NonDeterministeFiniteAutomataNode& current_end_automata_node = construct(ebnf_syntax, **it_node, *current_start_automata_node);
			current_start_automata_node = &current_end_automata_node;
		}
		return *current_start_automata_node;
	}
	else {
		throw exception::Exception("Unknown node \""+std::string(current_tree_node.type().string())+"\"");
	}
}

NonDeterministeFiniteAutomataNode& RegularExpressionRepresentation::start_node() {
	return _start_node;
}

const NonDeterministeFiniteAutomataNode& RegularExpressionRepresentation::start_node() const {
	return _start_node;
}
