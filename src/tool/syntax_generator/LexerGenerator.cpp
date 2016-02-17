#include "cliff/tool/syntax_generator/LexerGenerator.h"

using namespace cliff;

void LexerGenerator::generate_lexer(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, Syntax& output_syntax, SyntaxRepresentation& syntax_representation,
									std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list, DeterministeFiniteAutomataNode& dfa_start) {
	MemoryContainer<NonDeterministeFiniteAutomataNode> nfa_memory;
	NonDeterministeFiniteAutomataNode nfa_start_node(nfa_memory);

	// Terminal literal string
	std::vector<const char*> symbols_content;
	get_terminal_content(ebnf_syntax, syntax_tree, symbols_content);
	for(const char* content : symbols_content) {
		NonDeterministeFiniteAutomataNode* current_node = &nfa_start_node;
		const TokenSymbol& symbol = output_syntax.get_symbol_from_name(content);
		for(unsigned str_index = 0; str_index < symbol.size(); str_index++) {
			current_node = &current_node->create_output_node(content[str_index]);
		}
		current_node->add_accepting_state(symbol);

	}

	// Regular expression
	for(auto& regular_expression : syntax_representation.regular_expression_list()) {
		nfa_start_node.create_output_transition_with(regular_expression.second.start_node(), LetterRange::Epsilon_range);
	}

	// Ignore sequence

	/*
	for(const std::pair<const TokenSymbol*, RegularExpressionRepresentation>& regular_expression : syntax_representation.regular_expression_list()) {

	}*/


	std::cout << "Non Derterministe Finite Automata : " << std::endl;
	nfa_start_node.print(std::cout);
	std::cout << std::endl;



	create_dfa(ebnf_syntax, nfa_start_node, dfa_start);
	/*std::cout << "Derterministe Finite Automata : " << std::endl;
	dfa_start.print(std::cout);
	std::cout << std::endl;
*/
	reduce_dfa(ebnf_syntax, dfa_start);
/*
	std::cout << "Reduced Derterministe Finite Automata : " << std::endl;
	dfa_start.print(std::cout);
	std::cout << std::endl;
*/
	generate_lexer(output_syntax, dfa_start, lexer_state_list);
}

void LexerGenerator::get_terminal_content(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, std::vector<const char*>& symbols_content) {
	if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule_terminal")) { // TODO use syntax_representation
		if(syntax_tree.content() == nullptr) {
			syntax_tree.print(std::cout);
		}

		symbols_content.emplace_back(syntax_tree.content());
	}
	else {
		for(const AbstractSyntaxTree* child : syntax_tree.children())
			get_terminal_content(ebnf_syntax, *child, symbols_content);
	}
}

void LexerGenerator::create_dfa(const Syntax& ebnf_syntax, const NonDeterministeFiniteAutomataNode& start_node, DeterministeFiniteAutomataNode& start_dfa_node) {
	std::map<DeterministeFiniteAutomataNode*, std::vector<const NonDeterministeFiniteAutomataNode*>> _state_correspondence;
	std::vector<const NonDeterministeFiniteAutomataNode*> epsilon_closure_list;
	std::stack<DeterministeFiniteAutomataNode*> dfa_stack;
	// Start state
	auto start_it =_state_correspondence.emplace(std::piecewise_construct, std::forward_as_tuple(&start_dfa_node), std::forward_as_tuple()).first;
	start_it->second.push_back(&start_node);
	LetterRange eps_range;
	epsilon_closure(start_node, epsilon_closure_list, eps_range);
	start_it->second.insert(std::end(start_it->second), std::begin(epsilon_closure_list), std::end(epsilon_closure_list));
	dfa_stack.push(&start_dfa_node);

	while(!dfa_stack.empty()) {
		DeterministeFiniteAutomataNode* current_dfa_state = dfa_stack.top();
		dfa_stack.pop();

		std::map<LetterRange, std::vector<const NonDeterministeFiniteAutomataNode*>> range_list;

		for(const NonDeterministeFiniteAutomataNode* nfa_state : _state_correspondence[current_dfa_state])
			segment_output_range(*nfa_state, range_list);

		for(const auto& range : range_list) {
			bool found = false;
			epsilon_closure_list.clear();

			for(const NonDeterministeFiniteAutomataNode* nfa_state : _state_correspondence[current_dfa_state])
				found = epsilon_closure(*nfa_state, epsilon_closure_list, range.first) || found;
			 if(!epsilon_closure_list.empty() && found) {
				auto exist_it = std::end(_state_correspondence);

				for(auto computed_state_it = std::begin(_state_correspondence); computed_state_it != std::end(_state_correspondence); ++computed_state_it) {
                    if(epsilon_closure_list.size() == computed_state_it->second.size() && std::is_permutation (std::begin(epsilon_closure_list), std::end(epsilon_closure_list), std::begin(computed_state_it->second))) {
						exist_it = computed_state_it;
						break;
					}
				}
				if(exist_it == std::end(_state_correspondence)) {

					DeterministeFiniteAutomataNode& new_node = current_dfa_state->create_output_node(range.first);
					for(const NonDeterministeFiniteAutomataNode* nfa_state : epsilon_closure_list) { //TODO remove duplicate
						if(nfa_state->is_accepting_state()) {
							for(const TokenSymbol* accepting_state : nfa_state->accepting_state()) {
								new_node.add_accepting_state(*accepting_state);
							}
						}
					}

					_state_correspondence.insert(std::make_pair(&new_node, std::vector<const NonDeterministeFiniteAutomataNode*>()));
					start_it = _state_correspondence.find(&new_node);
					start_it->second.insert(std::end(start_it->second), std::begin(epsilon_closure_list), std::end(epsilon_closure_list));
					dfa_stack.push(&new_node);
				}
				else {
					current_dfa_state->create_output_transition_with(*exist_it->first, range.first);
				}
			 }
		}
	}
}

void LexerGenerator::segment_output_range(const NonDeterministeFiniteAutomataNode& nfa_node, std::map<LetterRange, std::vector<const NonDeterministeFiniteAutomataNode*>>& output_list) {


	for(const std::pair<LetterRange, NonDeterministeFiniteAutomataNode*>& transition : nfa_node.transitions()) {
		if(transition.first.is_epsilon()) {
			segment_output_range(*transition.second, output_list);
		}
		else {
			LetterRange remain(transition.first);

			for(std::map<LetterRange, std::vector<const NonDeterministeFiniteAutomataNode*>>::iterator it_range = std::begin(output_list); it_range != std::end(output_list);) {
				LetterRange intersection;
				transition.first.intersection(it_range->first, intersection);
				if(!intersection.is_epsilon() && intersection != it_range->first) {
					// old segment
					LetterRange old_range(it_range->first);
					old_range -= intersection;
					std::vector<const NonDeterministeFiniteAutomataNode*> old_vector(it_range->second);
					if(!old_range.is_epsilon())
						output_list.emplace(std::piecewise_construct, std::forward_as_tuple(old_range), std::forward_as_tuple(old_vector));

					// new segment
					remain -= intersection;

					// intesection
					auto it_new = output_list.emplace(std::piecewise_construct, std::forward_as_tuple(intersection), std::forward_as_tuple()).first;
					it_new->second.push_back(transition.second);
					std::copy(std::begin(it_range->second), std::end(it_range->second), std::back_inserter(it_new->second));

					it_range = output_list.erase(it_range);
				}
				else {
					++it_range;
					remain -= intersection;
				}
			}

			if(!remain.is_epsilon()) {
				auto it_new = output_list.emplace(std::piecewise_construct, std::forward_as_tuple(remain), std::forward_as_tuple()).first;
				it_new->second.push_back(transition.second);
			}
		}
	}
}

bool LexerGenerator::epsilon_closure(const NonDeterministeFiniteAutomataNode& nfa_node, std::vector<const NonDeterministeFiniteAutomataNode*>& output_list, LetterRange range) {
	std::stack<std::pair<const NonDeterministeFiniteAutomataNode*, bool>> state_stack;
	state_stack.push(std::make_pair(&nfa_node, false));

	bool found = false;

	while(!state_stack.empty()) {
		const NonDeterministeFiniteAutomataNode* current_state = state_stack.top().first;
		bool f = state_stack.top().second;
		state_stack.pop();

		for(const auto& output_state : current_state->transitions()) {
			LetterRange intersection;
			output_state.first.intersection(range, intersection);
			if((output_state.first.is_epsilon() || (!f && !intersection.is_epsilon())) && std::find(std::begin(output_list), std::end(output_list), output_state.second) == std::end(output_list)) {
				if(!intersection.is_epsilon()) {
					found = true;
					state_stack.push(std::make_pair(output_state.second, true));
					output_list.push_back(output_state.second);
				}
				else {
					state_stack.push(std::make_pair(output_state.second, f));
					if(f)
						output_list.push_back(output_state.second);
				}
			}
		}
	}

	return found;
}

void LexerGenerator::reduce_dfa(const Syntax& ebnf_syntax, DeterministeFiniteAutomataNode& start_node) {
	std::vector<std::vector<DeterministeFiniteAutomataNode*>> partitions;
	partitions.push_back(std::vector<DeterministeFiniteAutomataNode*>());
	partitions.push_back(std::vector<DeterministeFiniteAutomataNode*>());

	std::stack<DeterministeFiniteAutomataNode*> node_stack;
	node_stack.push(&start_node);
	std::vector<DeterministeFiniteAutomataNode*> node_history;
	node_history.push_back(&start_node);


	// separate accepting and non-accepting state
	while(!node_stack.empty()) {
		DeterministeFiniteAutomataNode* current_node = node_stack.top();
		node_stack.pop();

		if(current_node->is_accepting_state())
			partitions[0].push_back(current_node);
		else
			partitions[1].push_back(current_node);

		for(auto& child_node : current_node->transitions()) {
			if(std::find(std::begin(node_history), std::end(node_history), child_node.second) == std::end(node_history)) {
				node_stack.push(child_node.second);
				node_history.push_back(child_node.second);
			}
		}
	}

	if(partitions[0].size() == 0 || partitions[1].size() == 0)
		return;

	bool update = true;

	while(update) {
		update = false;
		for(std::vector<DeterministeFiniteAutomataNode*>& partition : partitions) {
			if(partition.size() == 1)
				continue;

			auto it_split = std::end(partition);
			for(auto it_node = std::begin(partition)+1; it_node != std::end(partition); ++it_node) {
				if(!is_equal_transition(partition[0], *it_node)) {
					it_split = it_node;
					break;
				}
			}

			if(it_split != std::end(partition)) {
				std::vector<DeterministeFiniteAutomataNode*> new_vector;
				std::copy(it_split, std::end(partition), std::back_inserter(new_vector));
				partition.erase(it_split, std::end(partition));
				partitions.push_back(new_vector);
				update = true;
				break;
			}
		}
	}

	std::vector<DeterministeFiniteAutomataNode*> remain_node;
	for(std::vector<DeterministeFiniteAutomataNode*>& partition : partitions) {
		remain_node.push_back(partition[0]);
	}

	for(std::vector<DeterministeFiniteAutomataNode*>& partition : partitions) {
		if(partition.size() == 1)
			continue;

		std::vector<DeterministeFiniteAutomataNode*> removed_node;
		std::copy(std::begin(partition)+1, std::end(partition), std::back_inserter(removed_node));

		for(DeterministeFiniteAutomataNode* node : remain_node) {
			for(auto& transition  : node->transitions()) {
				if(std::find(std::begin(removed_node), std::end(removed_node), transition.second) != std::end(removed_node)) {
					node->change_transition(transition.first, *partition[0]);
				}
			}
		}

		// TODO delete unused node ?
	}
}

bool LexerGenerator::is_equal_transition(const DeterministeFiniteAutomataNode* node_1, const DeterministeFiniteAutomataNode* node_2) {
	if(node_1->transitions().size() != node_2->transitions().size()) {
		return false;
	}

	std::vector<LetterRange> transition_1(node_1->transitions().size());
	std::vector<LetterRange> transition_2(node_2->transitions().size());

	for(auto& transition : node_1->transitions()) {
		transition_1.push_back(transition.first);
	}


	for(auto& transition : node_2->transitions()) {
		transition_2.push_back(transition.first);
	}

	return transition_1.size() == transition_2.size() && node_1->accepting_state().size() == node_2->accepting_state().size()
			&& std::is_permutation(std::begin(transition_1), std::end(transition_1), std::begin(transition_2))
			&& std::is_permutation(std::begin(node_1->accepting_state()), std::end(node_1->accepting_state()), std::begin(node_2->accepting_state()));
}

void LexerGenerator::generate_lexer(Syntax& output_syntax, const DeterministeFiniteAutomataNode& start_node, std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list) {
	std::stack<const DeterministeFiniteAutomataNode*> node_stack;
	node_stack.push(&start_node);

	lexer_state_list.push_back(&start_node);

	LetterRange direct_letter_range(0, Syntax::Direct_letter_range-1);

	while(!node_stack.empty()) {
		const DeterministeFiniteAutomataNode* current_node = node_stack.top();
		node_stack.pop();

		for(auto& child_node : current_node->transitions()) {
			if(std::find(std::begin(lexer_state_list), std::end(lexer_state_list), child_node.second) == std::end(lexer_state_list)) {
				lexer_state_list.push_back(child_node.second);
				node_stack.push(child_node.second);
			}
		}
	}

	output_syntax.set_lexer_table(lexer_state_list.size());

	for(unsigned int state_index=0; state_index<lexer_state_list.size(); state_index++) {

		for(unsigned int char_index=0; char_index<Syntax::Direct_letter_range; char_index++) {
			output_syntax.lexer_table()[state_index*Syntax::Direct_letter_range+char_index] = Syntax::Lexer_state_error;
		}

		for(auto& transition : lexer_state_list[state_index]->transitions()) {
			LetterRange output_direct_range;
			direct_letter_range.intersection(transition.first, output_direct_range);
			std::vector<Letter> letter_list;
			output_direct_range.to_letter_list(letter_list);
			for(Letter letter : letter_list) {
				output_syntax.lexer_table()[state_index*Syntax::Direct_letter_range+letter] = std::distance(std::begin(lexer_state_list), std::find(std::begin(lexer_state_list), std::end(lexer_state_list), transition.second));
			}

			//TODO UTF-8 character

		}
	}

}
