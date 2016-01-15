#include "cliff/tool/syntax_generator/ParserGenerator.h"

using namespace cliff;

void ParserGenerator::generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, SyntaxRepresentation& syntax_representation) {

	std::vector<Set> set_list;
	set_list.emplace_back();
	const SyntaxRepresentation::Rule& root_rule = syntax_representation.get_rule_by_symbol(output_syntax.get_symbol_from_name(Syntax::Root_symbol));
	set_list.back().item_list.emplace_back(root_rule.alternatives().front(), 0, output_syntax.get_symbol_from_name(Syntax::EOF_symbol));
	closure_procedure(ebnf_syntax, output_syntax, syntax_representation, set_list.back());

	construct_closure_set(ebnf_syntax, output_syntax, syntax_representation, set_list);

	for(const Set& set : set_list) {
		std::cout << "====SET==== (" << set.predecessor_set << ",\"" << (set.transition_symbol != nullptr ? set.transition_symbol->string() : "none") << "\"" << std::endl;
		for(const Item& item : set.item_list) {
			item.print(std::cout);
		}
	}

	generate_parser(ebnf_syntax, output_syntax, syntax_representation, set_list);
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

void ParserGenerator::closure_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, Set& output_set) {
	Set unchecked_item_list;
	std::swap(output_set.item_list, unchecked_item_list.item_list);


	while(!unchecked_item_list.item_list.empty()) {
		Item current_item = unchecked_item_list.item_list.back();
		unchecked_item_list.item_list.pop_back();

		output_set.item_list.push_back(current_item);

		std::vector<std::pair<SyntaxRepresentation::Symbol, bool>> following_symbol;
		current_item.following_symbols(following_symbol);

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
							unchecked_item_list.item_list.push_back(new_item);

					}

					if(current_item.rule->is_epsilon_productive(syntax_representation, current_item.cursor)) {
						Item new_item(alternative, 0, *(current_item.next_token));
						if(std::find(std::begin(output_set.item_list), std::end(output_set.item_list), new_item) == std::end(output_set.item_list)
								&& std::find(std::begin(unchecked_item_list.item_list), std::end(unchecked_item_list.item_list), new_item) == std::end(unchecked_item_list.item_list))
							unchecked_item_list.item_list.push_back(new_item);
					}
				}
			}
		}
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

void ParserGenerator::generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, const SyntaxRepresentation& syntax_representation, const std::vector<Set>& set_list) {
	output_syntax.set_parser_table(set_list.size(), syntax_representation.dummy_rule_name().size());

	for(unsigned int set_index = 0; set_index < set_list.size(); set_index++) {
		for(auto it_symbol = output_syntax.begin_terminal(); it_symbol != output_syntax.end_terminal(); ++it_symbol) {
			output_syntax.parser_action_table()[(it_symbol-output_syntax.begin_terminal())*set_list.size()+set_index] = Syntax::Parser_unaccepting_state;
			output_syntax.parser_reduce_number()[(it_symbol-output_syntax.begin_terminal())*set_list.size()+set_index] = 0;
		}
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
				}
			}
			else if(item.rule->sequence()[item.cursor].is_terminal() && (j = find_goto(ebnf_syntax, output_syntax, syntax_representation, set_list[set_index], item.rule->sequence()[item.cursor].content(), set_list)) != -1) {
				set_action_table_or_else(output_syntax, output_syntax.index_of_symbol(item.rule->sequence()[item.cursor].content())*set_list.size()+set_index,
						Syntax::Parser_action_shift_mask | j
						| ((item.rule->flags() & SyntaxRepresentation::ListReduce) && item.cursor > 0 && syntax_representation.is_dummy_symbol_value(item.rule->sequence()[item.cursor-1].content()) ? Syntax::Parser_action_replace_state_mask : 0),
						0,
						exception::UserMessage(exception::UserMessage::Error, "Parser : shift/reduce conflict"));
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
