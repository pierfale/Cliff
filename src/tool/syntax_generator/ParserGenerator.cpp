#include "cliff/tool/syntax_generator/ParserGenerator.h"

using namespace cliff;

void ParserGenerator::generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, SyntaxRepresentation& syntax_representation, std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list) {

	std::vector<Set> set_list;

	set_list.emplace_back();
	const SyntaxRepresentation::Rule& root_rule = syntax_representation.get_rule_by_symbol(output_syntax.get_symbol_from_name(Syntax::Root_symbol));
	set_list.back().item_list.emplace_back(root_rule.alternatives().front(), 0, output_syntax.get_symbol_from_name(Syntax::EOF_symbol));
	closure_procedure(ebnf_syntax, output_syntax, syntax_representation, set_list.back());

	construct_closure_set(ebnf_syntax, output_syntax, syntax_representation, set_list);
/*
	int cpt = 0;
	for(const Set& set : set_list) {
		std::cout << "====SET==== " << cpt++ << " (" << set.predecessor_set << ",\"" << (set.transition_symbol != nullptr ? set.transition_symbol->string() : "none") << "\"" << std::endl;
		for(const Item& item : set.item_list) {
			item.print(std::cout);
		}
	}
*/

	generate_parser(ebnf_syntax, output_syntax, syntax_representation, set_list, lexer_state_list);
}

void ParserGenerator::construct_closure_set(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, std::vector<Set>& output) {
	std::vector<Set> unchecked_set;
	std::swap(output, unchecked_set);

	while(!unchecked_set.empty()) {
		output.push_back(unchecked_set.back());
		Set& current_set = output.back();
		unchecked_set.pop_back();

		for(const Item& item : current_set.item_list) {
			std::vector<std::pair<SyntaxRepresentation::Symbol, bool>> following_symbol;
			item.following_symbols(following_symbol);
			for(const std::pair<SyntaxRepresentation::Symbol, bool>& symbol : following_symbol) {
				Set new_set;
				new_set.transition_symbol = &symbol.first.content();
				new_set.predecessor_set = output.size()-1;
				new_set.is_repetition = symbol.second;
				goto_procedure(ebnf_syntax, generated_syntax, syntax_representation, current_set, symbol.first.content(), new_set);
				if(std::find(std::begin(output), std::end(output), new_set) == std::end(output) && std::find(std::begin(unchecked_set), std::end(unchecked_set), new_set) == std::end(unchecked_set)) {
					unchecked_set.push_back(new_set);
				}
			}

		}
	}
}

void ParserGenerator::compute_first_of(const SyntaxRepresentation& syntax_representation, const std::vector<SyntaxRepresentation::Symbol>& list, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) {

	unsigned int cursor = 0;
	while(cursor < list.size()) {
		if(list[cursor].is_terminal() || syntax_representation.regular_expression_list().find(&list[cursor].content()) != std::end(syntax_representation.regular_expression_list())) {
			output.push_back(&list[cursor].content());
			break;
		}
		else {
			if(std::find(std::begin(history), std::end(history), &list[cursor].content()) != std::end(history))
				break;

			const SyntaxRepresentation::Rule& rule = syntax_representation.get_rule_by_symbol(list[cursor].content());
			history.push_back(&list[cursor].content());

			for(const SyntaxRepresentation::InlinedAlternative& alternative : rule.alternatives())
				compute_first_of(syntax_representation, alternative.sequence(), output, history);
		}

	}
}

void ParserGenerator::closure_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, Set& output_set) {
	Set unchecked_item_list;
	std::swap(output_set.item_list, unchecked_item_list.item_list);


	while(!unchecked_item_list.item_list.empty()) {
		Item current_item = unchecked_item_list.item_list.back();
		unchecked_item_list.item_list.pop_back();

		output_set.item_list.push_back(current_item);

		if(current_item.cursor < current_item.rule->sequence().size() && !current_item.rule->sequence()[current_item.cursor].is_terminal()) {
			std::vector<SyntaxRepresentation::Symbol> next_list;
			std::copy(std::begin(current_item.rule->sequence())+current_item.cursor+1, std::end(current_item.rule->sequence()), std::back_inserter(next_list));
			next_list.emplace_back(*current_item.next_token, true);

			std::vector<const TokenSymbol*> first_list;
			std::vector<const TokenSymbol*> history;
			compute_first_of(syntax_representation, next_list, first_list, history);

			const SyntaxRepresentation::Rule& rule = syntax_representation.get_rule_by_symbol(current_item.rule->sequence()[current_item.cursor].content());

			for(const SyntaxRepresentation::InlinedAlternative& alternative : rule.alternatives()) {
				for(const TokenSymbol* first_symbol : first_list) {
					Item new_item(alternative, 0, *first_symbol);
					if(std::find(std::begin(output_set.item_list), std::end(output_set.item_list), new_item) == std::end(output_set.item_list)
							&& std::find(std::begin(unchecked_item_list.item_list), std::end(unchecked_item_list.item_list), new_item) == std::end(unchecked_item_list.item_list))
						 {
						unchecked_item_list.item_list.push_back(new_item);
					}

				}
			}

		}



		/*
		std::vector<std::pair<SyntaxRepresentation::Symbol, bool>> following_symbol;
		current_item.following_symbols(following_symbol); // TODO remove following symbol

		std::vector<const TokenSymbol*> first_list;
		current_item.rule->first_after(syntax_representation, current_item.cursor+1, first_list);

		std::sort(std::begin(first_list), std::end(first_list));
		first_list.erase(std::unique(std::begin(first_list), std::end(first_list)), std::end(first_list));


		for(const std::pair<SyntaxRepresentation::Symbol, bool>& symbol : following_symbol) {
			if(!symbol.first.is_terminal()) {
				const SyntaxRepresentation::Rule& rule = syntax_representation.get_rule_by_symbol(symbol.first.content());

				for(const SyntaxRepresentation::InlinedAlternative& alternative : rule.alternatives()) {
					for(const TokenSymbol* first_symbol : first_list) {
						Item new_item(alternative, 0, *first_symbol);
						if(std::find(std::begin(output_set.item_list), std::end(output_set.item_list), new_item) == std::end(output_set.item_list)
								&& std::find(std::begin(unchecked_item_list.item_list), std::end(unchecked_item_list.item_list), new_item) == std::end(unchecked_item_list.item_list))
							 {
							unchecked_item_list.item_list.push_back(new_item);
							current_item.print(std::cout);
							std::cout << "=> ";
							new_item.print(std::cout);
						}

					}
					if(current_item.rule->is_epsilon_productive(syntax_representation, current_item.cursor, true)) {
						Item new_item(alternative, 0, *(current_item.next_token));
						if(std::find(std::begin(output_set.item_list), std::end(output_set.item_list), new_item) == std::end(output_set.item_list)
								&& std::find(std::begin(unchecked_item_list.item_list), std::end(unchecked_item_list.item_list), new_item) == std::end(unchecked_item_list.item_list))
							{
						   unchecked_item_list.item_list.push_back(new_item);
						   current_item.print(std::cout);
						   std::cout << "=> (eps) ";
						   new_item.print(std::cout);
					   }
					}
				}
			}
		}*/
	}
}

void ParserGenerator::goto_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, const Set& input_set, const TokenSymbol& symbol, Set& output_set) {
	for(const Item& item : input_set.item_list) {
		item.consume(symbol, output_set.item_list);
	}

	closure_procedure(ebnf_syntax, generated_syntax, syntax_representation, output_set);
	output_set.erase_duplicate();
}

void ParserGenerator::set_action_table_or_else(Syntax& output_syntax, unsigned int index, unsigned int action, unsigned int reduce_number, exception::UserMessage&& exception) {
	if(output_syntax.parser_action_table()[index] == action || output_syntax.parser_action_table()[index] == Syntax::Parser_unaccepting_state) {
		output_syntax.parser_action_table()[index] = action;
		output_syntax.parser_reduce_number()[index] = reduce_number;
	}
	else
		throw exception;
}

void ParserGenerator::set_lexer_accepting_state_or_else(Syntax& output_syntax, unsigned int index, unsigned int value, exception::UserMessage&& exception) {
	if(output_syntax.lexer_accepting_state_table()[index] == value || output_syntax.lexer_accepting_state_table()[index] == Syntax::Lexer_unaccepting_state) {
		output_syntax.lexer_accepting_state_table()[index] = value;
	}
	else
		throw exception;
}

void ParserGenerator::set_lexer_accepting_state_if_none(Syntax& output_syntax, unsigned int index, unsigned int value) {
	if(output_syntax.lexer_accepting_state_table()[index] == value || output_syntax.lexer_accepting_state_table()[index] == Syntax::Lexer_unaccepting_state) {
		output_syntax.lexer_accepting_state_table()[index] = value;
	}
}

void ParserGenerator::generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, const SyntaxRepresentation& syntax_representation, const std::vector<Set>& set_list, std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list) {
	output_syntax.set_parser_table(set_list.size(), syntax_representation.dummy_rule_name().size());
	output_syntax.set_lexer_parser_table(lexer_state_list.size(), set_list.size());

	std::vector<const TokenSymbol*> ignored_token;
	for(const auto& regular_expression : syntax_representation.regular_expression_list()) {
		if(!regular_expression.second.used()) {
			ignored_token.push_back(regular_expression.first);
		}
	}

	for(unsigned int set_index = 0; set_index < set_list.size(); set_index++) {
		for(auto it_symbol = output_syntax.begin_terminal(); it_symbol != output_syntax.end_terminal(); ++it_symbol) {
			output_syntax.parser_action_table()[(it_symbol-output_syntax.begin_terminal())*set_list.size()+set_index] = Syntax::Parser_unaccepting_state;
			output_syntax.parser_reduce_number()[(it_symbol-output_syntax.begin_terminal())*set_list.size()+set_index] = 0;
		}

		for(unsigned int i=0; i<lexer_state_list.size(); i++)
			output_syntax.lexer_accepting_state_table()[i*set_list.size()+set_index] = Syntax::Lexer_unaccepting_state;

		std::vector<const TokenSymbol*> accept_token_list;

		// TODO initialize lexer accepting state
		for(const Item& item : set_list[set_index].item_list) {
			int j;
			if(item.cursor == item.rule->sequence().size()) {

				if(*item.next_token == output_syntax.get_symbol_from_name(Syntax::EOF_symbol) && item.rule->rule_name() == output_syntax.get_symbol_from_name(Syntax::Root_symbol)) // Accept action
					output_syntax.parser_action_table()[output_syntax.index_of_symbol(*item.next_token)*set_list.size()+set_index] = Syntax::Parser_action_accept_mask;
				else {
					set_action_table_or_else(output_syntax, output_syntax.index_of_symbol(*item.next_token)*set_list.size()+set_index,
								Syntax::Parser_action_reduce_mask
								| (item.rule->flags() & SyntaxRepresentation::ListHead ? Syntax::Parser_action_dummy_state_mask : 0)
								| (item.rule->flags() & SyntaxRepresentation::ListTail ? Syntax::Parser_action_skip_state_mask : 0)
								| (item.rule->flags() & SyntaxRepresentation::ListReduce && syntax_representation.is_dummy_symbol_value(item.rule->sequence().back().content()) ? Syntax::Parser_action_replace_state_mask : 0)
								| (syntax_representation.is_dummy_symbol_value(item.rule->rule_name()) ?
									(std::distance(output_syntax.begin_non_terminal(),  output_syntax.end_non_terminal())+
									 std::distance(std::begin(syntax_representation.dummy_rule_name()), std::find(std::begin(syntax_representation.dummy_rule_name()), std::end(syntax_representation.dummy_rule_name()), item.rule->rule_name())))
									: output_syntax.index_of_symbol(item.rule->rule_name())),
							item.rule->sequence().size(),
							exception::UserMessage(exception::UserMessage::Error, "Parser : reduce/reduce conflict"));

					accept_token_list.push_back(item.next_token);
				}
			}
			else if(item.rule->sequence()[item.cursor].is_terminal() && (j = find_goto(ebnf_syntax, output_syntax, syntax_representation, set_list[set_index], item.rule->sequence()[item.cursor].content(), set_list)) != -1) {
				set_action_table_or_else(output_syntax, output_syntax.index_of_symbol(item.rule->sequence()[item.cursor].content())*set_list.size()+set_index,
						Syntax::Parser_action_shift_mask | j
						| ((item.rule->flags() & SyntaxRepresentation::ListReduce) && item.cursor > 0 && syntax_representation.is_dummy_symbol_value(item.rule->sequence()[item.cursor-1].content()) ? Syntax::Parser_action_replace_state_mask : 0),
						0,
						exception::UserMessage(exception::UserMessage::Error, "Parser : shift/reduce conflict"));

				accept_token_list.push_back(&item.rule->sequence()[item.cursor].content());

			}
		}

		for(auto it_symbol = output_syntax.begin_non_terminal(); it_symbol != output_syntax.end_non_terminal(); ++it_symbol) {
			int j;
			if((j = find_goto(ebnf_syntax, output_syntax, syntax_representation, set_list[set_index], *it_symbol, set_list)) != -1) {
				output_syntax.parser_goto_table()[std::distance(output_syntax.begin_non_terminal(), it_symbol)*set_list.size()+set_index] = j;
			}
			else {
				output_syntax.parser_goto_table()[std::distance(output_syntax.begin_non_terminal(), it_symbol)*set_list.size()+set_index] = Syntax::Parser_unaccepting_state;
			}
		}

		for(auto it_symbol = std::begin(syntax_representation.dummy_rule_name()); it_symbol != std::end(syntax_representation.dummy_rule_name()); ++it_symbol) {
			int j;
			if((j = find_goto(ebnf_syntax, output_syntax, syntax_representation, set_list[set_index], *it_symbol, set_list)) != -1) {
				output_syntax.parser_goto_table()[(std::distance(output_syntax.begin_non_terminal(),  output_syntax.end_non_terminal())+std::distance(std::begin(syntax_representation.dummy_rule_name()), it_symbol))*set_list.size()+set_index] = j;
			}
			else {
				output_syntax.parser_goto_table()[(std::distance(output_syntax.begin_non_terminal(),  output_syntax.end_non_terminal())+std::distance(std::begin(syntax_representation.dummy_rule_name()), it_symbol))*set_list.size()+set_index] = Syntax::Parser_unaccepting_state;
			}
		}

		for(const TokenSymbol* symbol : accept_token_list) {
			for(unsigned int lexer_state = 0; lexer_state < lexer_state_list.size(); lexer_state++) {
				for(const TokenSymbol* accepting_state : lexer_state_list[lexer_state]->accepting_state()) {
					if(*symbol == *accepting_state) {
						set_lexer_accepting_state_or_else(output_syntax, lexer_state*set_list.size()+set_index, std::distance(output_syntax.begin_terminal(), symbol),
														  exception::UserMessage(exception::UserMessage::Error, "Parser : lexer accepting state conflict : "+std::string(symbol->string())));
					}
				}
			}
		}

		for(const TokenSymbol* symbol : ignored_token) {
			for(unsigned int lexer_state = 0; lexer_state < lexer_state_list.size(); lexer_state++) {
				for(const TokenSymbol* accepting_state : lexer_state_list[lexer_state]->accepting_state()) {
					if(*symbol == *accepting_state) {
						set_lexer_accepting_state_if_none(output_syntax, lexer_state*set_list.size()+set_index, Syntax::Lexer_accepting_state_ignore);
					}
				}
			}
		}

		std::copy(std::begin(ignored_token), std::end(ignored_token), std::back_inserter(accept_token_list));

		for(unsigned int lexer_state = 0; lexer_state < lexer_state_list.size(); lexer_state++) {
			if(!reachable_lexing_state(*lexer_state_list[lexer_state], accept_token_list)) {
				output_syntax.lexer_accepting_state_table()[lexer_state*set_list.size()+set_index] |= Syntax::Lexer_disabled_path;
			}
		}

	}
}

int ParserGenerator::find_goto(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, const Set& input_set, const TokenSymbol& symbol, const std::vector<Set>& set_list) {
	Set result;
	goto_procedure(ebnf_syntax, generated_syntax, syntax_representation, input_set, symbol, result);

	for(unsigned int i=0; i<set_list.size(); i++) {
		if(result.item_list.size() == set_list[i].item_list.size() && std::is_permutation(std::begin(result.item_list), std::end(result.item_list), std::begin(set_list[i].item_list)))
			return i;
	}
	return -1;
}

bool ParserGenerator::reachable_lexing_state(const DeterministeFiniteAutomataNode& start_node, std::vector<const TokenSymbol*> possible_accepting_token) {
	std::stack<const DeterministeFiniteAutomataNode*> node_stack;
	std::vector<const DeterministeFiniteAutomataNode*> node_history;

	node_stack.push(&start_node);


	while(!node_stack.empty()) {
		const DeterministeFiniteAutomataNode* current_node = node_stack.top();
		node_stack.pop();

		for(const TokenSymbol* accepting_state : current_node->accepting_state()) {
			if(std::find(std::begin(possible_accepting_token), std::end(possible_accepting_token), accepting_state) != std::end(possible_accepting_token))
				return true;
		}
		for(auto& child_node : current_node->transitions()) {
			if(std::find(std::begin(node_history), std::end(node_history), child_node.second) == std::end(node_history)) {
				node_history.push_back(child_node.second);
				node_stack.push(child_node.second);
			}
		}
	}

	return false;
}
