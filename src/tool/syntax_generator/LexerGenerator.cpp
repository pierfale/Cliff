#include "cliff/tool/syntax_generator/LexerGenerator.h"

using namespace cliff;

void LexerGenerator::generate_lexer(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, Syntax& output_syntax) {
	MemoryContainer<NonDeterministeFiniteAutomataNode> nfa_memory;
	NonDeterministeFiniteAutomataNode nfa_start_node(nfa_memory);
	std::vector<Automata::Symbol> alphabet;
	create_regular_expression_nfa(ebnf_syntax, syntax_tree, nfa_start_node, alphabet, output_syntax);
	for(auto it_symbol = output_syntax.begin_terminal(); it_symbol != output_syntax.end_terminal(); ++it_symbol) {
		if(*it_symbol != output_syntax.get_symbol_from_name(Syntax::EOF_symbol)) {
			NonDeterministeFiniteAutomataNode* current_node = &nfa_start_node;
			for(unsigned str_index = 0; str_index < it_symbol->size(); str_index++) {
				current_node = &current_node->create_output_node(it_symbol->string()[str_index]);
				alphabet.push_back(it_symbol->string()[str_index]);
			}
			current_node->add_accepting_state(*it_symbol);
		}
	}


	std::cout << "Non Derterministe Finite Automata : " << std::endl;
	nfa_start_node.print(std::cout);
	std::cout << std::endl;

	std::sort(std::begin(alphabet), std::end(alphabet));
	alphabet.erase(std::unique(std::begin(alphabet), std::end(alphabet)), std::end(alphabet));

	MemoryContainer<DeterministeFiniteAutomataNode> dfa_memory;
	DeterministeFiniteAutomataNode dfa_start_node(dfa_memory);
	create_dfa(ebnf_syntax, nfa_start_node, dfa_start_node, alphabet);
	std::cout << "Derterministe Finite Automata : " << std::endl;
	dfa_start_node.print(std::cout);
	std::cout << std::endl;

	reduce_dfa(ebnf_syntax, dfa_start_node, alphabet);

	std::cout << "Reduced Derterministe Finite Automata : " << std::endl;
	dfa_start_node.print(std::cout);
	std::cout << std::endl;

	generate_lexer(output_syntax, dfa_start_node);
}


NonDeterministeFiniteAutomataNode& LexerGenerator::create_regular_expression_nfa(const Syntax& ebnf_syntax, const AbstractSyntaxTree& current_tree_node, NonDeterministeFiniteAutomataNode& start_automata_node, std::vector<Automata::Symbol>& alphabet, Syntax& output_syntax) {
	if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name("rule") && current_tree_node.children()[2]->type() == ebnf_syntax.get_symbol_from_name("regular_expression")) {
		NonDeterministeFiniteAutomataNode& last_node = create_regular_expression_nfa(ebnf_syntax, *current_tree_node.children()[2], start_automata_node.create_output_node(Automata::Epsilon), alphabet, output_syntax);
		last_node.add_accepting_state(output_syntax.get_symbol_from_name(current_tree_node.children()[0]->content()));
		return last_node;
	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name("regular_expression_letter_list")) {
		NonDeterministeFiniteAutomataNode* current_automata_node = &start_automata_node;
		for(const AbstractSyntaxTree* node : current_tree_node.children()) {
			 current_automata_node = &current_automata_node->create_output_node(node->content()[0]);

			 if(std::find(std::begin(alphabet), std::end(alphabet), node->content()[0]) == std::end(alphabet))
				 alphabet.push_back(node->content()[0]);
		}
		return *current_automata_node;

	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name("regular_expression_alternative")) {
		NonDeterministeFiniteAutomataNode& last_node = start_automata_node.create_node();
		for(const AbstractSyntaxTree* node : current_tree_node.children()) {
			NonDeterministeFiniteAutomataNode& last_alt_node = create_regular_expression_nfa(ebnf_syntax, *node, start_automata_node.create_output_node(Automata::Epsilon), alphabet, output_syntax);
			 last_alt_node.create_output_transition_with(last_node, Automata::Epsilon);
		}
		return last_node;
	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name("regular_expression_repetition")) {
		NonDeterministeFiniteAutomataNode& start_automata_node_inner = start_automata_node.create_output_node(Automata::Epsilon);

		NonDeterministeFiniteAutomataNode& end_automata_node_inner = create_regular_expression_nfa(ebnf_syntax, *(current_tree_node.children()[0]), start_automata_node_inner, alphabet, output_syntax);
		NonDeterministeFiniteAutomataNode& last_node = end_automata_node_inner.create_output_node(Automata::Epsilon);

		start_automata_node.create_output_transition_with(last_node, Automata::Epsilon);
		end_automata_node_inner.create_output_transition_with(start_automata_node_inner, Automata::Epsilon);

		return last_node;
	}
	else if(current_tree_node.type() == ebnf_syntax.get_symbol_from_name("regular_expression")) {
		NonDeterministeFiniteAutomataNode* current_start_automata_node = &start_automata_node;
		for(auto it_node = std::begin(current_tree_node.children()); it_node != std::end(current_tree_node.children()); ++it_node) {
			NonDeterministeFiniteAutomataNode& current_end_automata_node = create_regular_expression_nfa(ebnf_syntax, **it_node, *current_start_automata_node, alphabet, output_syntax);
			current_start_automata_node = &current_end_automata_node;
		}
		return *current_start_automata_node;
	}
	else {
		for(const AbstractSyntaxTree* node : current_tree_node.children()) {
			create_regular_expression_nfa(ebnf_syntax, *node, start_automata_node, alphabet, output_syntax);
		}
		return *((NonDeterministeFiniteAutomataNode*)nullptr);
	}
}

void LexerGenerator::create_dfa(const Syntax& ebnf_syntax, const NonDeterministeFiniteAutomataNode& start_node, DeterministeFiniteAutomataNode& start_dfa_node, const std::vector<Automata::Symbol>& alphabet) {
	std::map<DeterministeFiniteAutomataNode*, std::vector<const NonDeterministeFiniteAutomataNode*>> _state_correspondence;
	std::vector<const NonDeterministeFiniteAutomataNode*> epsilon_closure_list;
	std::stack<DeterministeFiniteAutomataNode*> dfa_stack;
	// Start state
	_state_correspondence.insert(std::make_pair(&start_dfa_node, std::vector<const NonDeterministeFiniteAutomataNode*>()));
	auto start_it = _state_correspondence.find(&start_dfa_node);
	start_it->second.push_back(&start_node);
	epsilon_closure(start_node, epsilon_closure_list);
	start_it->second.insert(std::end(start_it->second), std::begin(epsilon_closure_list), std::end(epsilon_closure_list));
	dfa_stack.push(&start_dfa_node);

	while(!dfa_stack.empty()) {
		DeterministeFiniteAutomataNode* current_dfa_state = dfa_stack.top();
		dfa_stack.pop();

		for(Automata::Symbol letter : alphabet) {
			bool found = false;
			epsilon_closure_list.clear();

			for(const NonDeterministeFiniteAutomataNode* nfa_state : _state_correspondence[current_dfa_state])
				found = epsilon_closure(*nfa_state, epsilon_closure_list, letter) || found;

			 if(!epsilon_closure_list.empty() && found) {
				auto exist_it = std::end(_state_correspondence);

				for(auto computed_state_it = std::begin(_state_correspondence); computed_state_it != std::end(_state_correspondence); ++computed_state_it) {
					if(std::is_permutation (std::begin(epsilon_closure_list), std::end(epsilon_closure_list), std::begin(computed_state_it->second))) {
						exist_it = computed_state_it;
						break;
					}
				}
				if(exist_it == std::end(_state_correspondence)) {

					DeterministeFiniteAutomataNode& new_node = current_dfa_state->create_output_node(letter);
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
					current_dfa_state->create_output_transition_with(*exist_it->first, letter);
				}
			 }
		}

	}
}

bool LexerGenerator::epsilon_closure(const NonDeterministeFiniteAutomataNode& nfa_node, std::vector<const NonDeterministeFiniteAutomataNode*>& output_list, Automata::Symbol letter) {
	std::stack<std::pair<const NonDeterministeFiniteAutomataNode*, bool>> state_stack;
	state_stack.push(std::make_pair(&nfa_node, false));

	bool found = false;

	while(!state_stack.empty()) {
		const NonDeterministeFiniteAutomataNode* current_state = state_stack.top().first;
		bool f = state_stack.top().second;
		state_stack.pop();

		for(const auto& output_state : current_state->transitions()) {
			if((output_state.first == Automata::Epsilon || (!f && output_state.first == letter)) && std::find(std::begin(output_list), std::end(output_list), output_state.second) == std::end(output_list)) {
				if(output_state.first == letter) {
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

void LexerGenerator::reduce_dfa(const Syntax& ebnf_syntax, DeterministeFiniteAutomataNode& start_node, const std::vector<Automata::Symbol>& alphabet) {
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

	std::vector<Automata::Symbol> transition_1(node_1->transitions().size());
	std::vector<Automata::Symbol> transition_2(node_2->transitions().size());

	for(auto& transition : node_1->transitions()) {
		transition_1.push_back(transition.first);
	}


	for(auto& transition : node_2->transitions()) {
		transition_2.push_back(transition.first);
	}

	return std::is_permutation(std::begin(transition_1), std::end(transition_1), std::begin(transition_2))
		&& std::is_permutation(std::begin(node_1->accepting_state()), std::end(node_1->accepting_state()), std::begin(node_2->accepting_state()));
}

void LexerGenerator::generate_lexer(Syntax& output_syntax, const DeterministeFiniteAutomataNode& start_node) {
	std::stack<const DeterministeFiniteAutomataNode*> node_stack;
	node_stack.push(&start_node);
	std::vector<const DeterministeFiniteAutomataNode*> node_list;
	node_list.push_back(&start_node);

	while(!node_stack.empty()) {
		const DeterministeFiniteAutomataNode* current_node = node_stack.top();
		node_stack.pop();

		for(auto& child_node : current_node->transitions()) {
			if(std::find(std::begin(node_list), std::end(node_list), child_node.second) == std::end(node_list)) {
				node_list.push_back(child_node.second);
				node_stack.push(child_node.second);
			}
		}
	}

	output_syntax.set_lexer_table(node_list.size());

	for(unsigned int state_index=0; state_index<node_list.size(); state_index++) {
		for(unsigned int char_index=0; char_index<Syntax::Direct_letter_range; char_index++) {
			output_syntax.lexer_table()[state_index*Syntax::Direct_letter_range+char_index] = Syntax::Lexer_state_error;
		}

		for(auto& transition : node_list[state_index]->transitions()) {
			if(transition.first < Syntax::Direct_letter_range) {
				output_syntax.lexer_table()[state_index*Syntax::Direct_letter_range+transition.first] = std::find(std::begin(node_list), std::end(node_list), transition.second)-std::begin(node_list);
			}
			else {
				// TODO UTF-8 character
			}
		}

		// TODO if several accepting on same state, eval priority.
		if(node_list[state_index]->accepting_state().size() > 1) {
			std::cout << "multiple accepting state :";
			for(auto s : node_list[state_index]->accepting_state())
				std::cout << " " << s->string();
			std::cout << std::endl;
		}
		else if(node_list[state_index]->accepting_state().size() == 1) {
			output_syntax.lexer_accepting_state()[state_index] = output_syntax.index_of_symbol(*node_list[state_index]->accepting_state()[0]);
		}
		else
			output_syntax.lexer_accepting_state()[state_index] = Syntax::Lexer_unaccepting_state;
	}

}
