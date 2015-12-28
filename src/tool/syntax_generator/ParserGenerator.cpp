#include "cliff/tool/syntax_generator/ParserGenerator.h"

using namespace cliff;

void ParserGenerator::generate_parser(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, Syntax& output_syntax) {
	RuleList rule_list;
	const char* main_rule = nullptr;
	construct_rule_list(ebnf_syntax, output_syntax, syntax_tree, rule_list, &main_rule);

	std::vector<Rule> init_rule;
	init_rule.emplace_back(output_syntax.get_symbol_from_name(Syntax::Root_symbol));
	init_rule[0].word_list.emplace_back(false, output_syntax.get_symbol_from_name(main_rule));
	rule_list.insert(std::make_pair(&output_syntax.get_symbol_from_name(Syntax::Root_symbol), init_rule));

	for(auto& rule : rule_list) {
		std::cout << "Rule \"" << rule.first->string() << "\"" << std::endl;
		for(auto& rule_alt : rule.second) {
			std::cout << " :=";
			for(const Word& word : rule_alt.word_list) {
				std::cout << " " << word.content.string();
			}
			std::cout << std::endl;
		}
	}

	std::vector<Set> set_list;
	set_list.emplace_back();
	set_list.back().item_list.emplace_back(init_rule[0], 0, output_syntax.get_symbol_from_name(Syntax::EOF_symbol));
	closure_procedure(ebnf_syntax, output_syntax, rule_list, set_list.back());

	construct_closure_set(ebnf_syntax, output_syntax, rule_list, set_list);

	for(const Set& set : set_list) {
		std::cout << "====SET==== (" << set.predecessor_set << ",\"" << (set.transition_symbol != nullptr ? set.transition_symbol->string() : "none") << "\")" << std::endl;
		for(const Item& closure : set.item_list) {
			for(unsigned int i=0; i<closure.cursor; i++) {
				std::cout << closure.rule.word_list[i].content.string() << " ";
			}
			std::cout << "* ";
			for(unsigned int i=closure.cursor; i<closure.rule.word_list.size(); i++) {
				std::cout << closure.rule.word_list[i].content.string() << " ";
			}
			std::cout << ", " << closure.next_token.string() << std::endl;
		}
	}

	generate_parser(ebnf_syntax, output_syntax, rule_list, set_list);
}

void ParserGenerator::construct_rule_list(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& syntax_tree, RuleList& rule_list, const char** main_rule) {
	if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule")) {
		if(*main_rule == nullptr) {
			*main_rule = syntax_tree.children()[0]->content();
		}

		const TokenSymbol& rule_symbol = generated_syntax.get_symbol_from_name(syntax_tree.children()[0]->content());
		rule_list.insert(std::make_pair(&rule_symbol, std::vector<Rule>()));
		std::vector<Rule>& vec = rule_list.find(&rule_symbol)->second;

		for(const AbstractSyntaxTree* child : syntax_tree.children()[2]->children()) {
			vec.emplace_back(rule_symbol);
			construct_rule(ebnf_syntax, generated_syntax, *child, vec.back());
		}
	}
	else {
		for(const AbstractSyntaxTree* child : syntax_tree.children())
			construct_rule_list(ebnf_syntax, generated_syntax, *child, rule_list, main_rule);
	}
}

void ParserGenerator::construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& syntax_tree, Rule& rule) {
	if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule_definition")) {
		for(const AbstractSyntaxTree* child : syntax_tree.children()) {
			const TokenSymbol& word_symbol = generated_syntax.get_symbol_from_name(child->content());
			if(child->type() == ebnf_syntax.get_symbol_from_name("rule_non_terminal")) {
				rule.word_list.emplace_back(false, word_symbol);
			}
			else if(child->type() == ebnf_syntax.get_symbol_from_name("rule_terminal")) {
				rule.word_list.emplace_back(true, word_symbol);
			}
		}
	}
	else {
		for(const AbstractSyntaxTree* child : syntax_tree.children())
			construct_rule(ebnf_syntax, generated_syntax, *child, rule);
	}
}

void ParserGenerator::construct_closure_set(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, std::vector<Set>& output) {
	std::vector<Set> unchecked_set;
	std::swap(output, unchecked_set);

	while(!unchecked_set.empty()) {
		output.push_back(unchecked_set.back());
		Set& current_set = output.back();
		unchecked_set.pop_back();


		for(const Item& item : current_set.item_list) {
			if(item.cursor < item.rule.word_list.size()) {
				Set new_set;
				new_set.transition_symbol = &item.rule.word_list[item.cursor].content;
				new_set.predecessor_set = output.size()-1;

				goto_procedure(ebnf_syntax, generated_syntax, rule_list, current_set, item.rule.word_list[item.cursor].content, new_set);

				if(std::find(std::begin(output), std::end(output), new_set) == std::end(output) && std::find(std::begin(unchecked_set), std::end(unchecked_set), new_set) == std::end(unchecked_set)) {
					unchecked_set.push_back(new_set);
				}
			}
		}
	}
}

void ParserGenerator::closure_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, Set& output_set) {
	Set unchecked_item_list;
	std::swap(output_set.item_list, unchecked_item_list.item_list);

	while(!unchecked_item_list.item_list.empty()) {
		Item current_item = unchecked_item_list.item_list.back();
		unchecked_item_list.item_list.pop_back();

		output_set.item_list.push_back(current_item);

		if(current_item.cursor < current_item.rule.word_list.size() && !current_item.rule.word_list[current_item.cursor].is_terminal) {
			assert(rule_list.find(&current_item.rule.word_list[current_item.cursor].content) != std::end(rule_list)); // TODO remove

			for(const Rule& rule : rule_list.find(&current_item.rule.word_list[current_item.cursor].content)->second) {

				// Look next token
				if(current_item.cursor+1 < current_item.rule.word_list.size()) {
					std::vector<const TokenSymbol*> first_list;
					get_first(ebnf_syntax, rule_list, current_item.rule.word_list[current_item.cursor+1],first_list);

					if(first_list.size() > 1) {
						std::sort(std::begin(first_list), std::end(first_list));
						first_list.erase(std::unique(std::begin(first_list), std::end(first_list)), std::end(first_list));
					}
					for(const TokenSymbol* symbol : first_list) {
						Item new_item(rule, 0, *symbol);
						if(std::find(std::begin(output_set.item_list), std::end(output_set.item_list), new_item) == std::end(output_set.item_list)
								&& std::find(std::begin(unchecked_item_list.item_list), std::end(unchecked_item_list.item_list), new_item) == std::end(unchecked_item_list.item_list))
							unchecked_item_list.item_list.push_back(new_item);
					}
				}
				else { // TODO check epsilon productive ?
					Item new_item(rule, 0, *(&current_item.next_token));
					if(std::find(std::begin(output_set.item_list), std::end(output_set.item_list), new_item) == std::end(output_set.item_list)
							&& std::find(std::begin(unchecked_item_list.item_list), std::end(unchecked_item_list.item_list), new_item) == std::end(unchecked_item_list.item_list))
						unchecked_item_list.item_list.push_back(new_item);
				}
			}
		}
	}
}

void ParserGenerator::goto_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, const Set& input_set, const TokenSymbol& symbol, Set& output_set) {
	for(const Item& item : input_set.item_list) {
		if(item.cursor < item.rule.word_list.size()) {
			if(item.rule.word_list[item.cursor].content == symbol) {
				Item new_item(item.rule, item.cursor+1, item.next_token);
				if(std::find(std::begin(output_set.item_list), std::end(output_set.item_list), new_item) == std::end(output_set.item_list))
					output_set.item_list.push_back(new_item);
			}
		}
	}
	closure_procedure(ebnf_syntax, generated_syntax, rule_list, output_set);
}

void ParserGenerator::get_first(const Syntax& ebnf_syntax, const RuleList& rule_list, const Word& word, std::vector<const TokenSymbol*>& output) {
	std::vector<const TokenSymbol*> history;
	_get_first(ebnf_syntax, rule_list, word, output, history);
}

void ParserGenerator::_get_first(const Syntax& ebnf_syntax, const RuleList& rule_list, const Word& word, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) {
	if(word.is_terminal) {
		output.push_back(&word.content);
	}
	else {
		for(const Rule& rule : rule_list.find(&word.content)->second) {
			if(rule.word_list.size() > 0 && std::find(std::begin(history), std::end(history), &rule.word_list[0].content) == std::end(history)) {
				history.push_back(&rule.word_list[0].content);
				_get_first(ebnf_syntax, rule_list, rule.word_list[0], output, history);
			}
		}
	}
}


bool ParserGenerator::is_epsilon_productive(const Syntax& ebnf_syntax, const RuleList& rule_list, const Word& word) {
	if(word.is_terminal) {
		return false;
	}
	else {
		for(const Rule& rule : rule_list.find(&word.content)->second) {
			if(rule.word_list.size() > 0) {
				if(is_epsilon_productive(ebnf_syntax, rule_list, rule.word_list[0]))
					return true;
			}
			else
				return true;
		}
	}
}

void ParserGenerator::generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, const RuleList& rule_list, const std::vector<Set>& set_list) {
	output_syntax.set_parser_table(set_list.size());


	for(unsigned int set_index = 0; set_index < set_list.size(); set_index++) {

		for(auto it_symbol = output_syntax.begin_terminal(); it_symbol != output_syntax.end_terminal(); ++it_symbol) {
			output_syntax.parser_goto_table()[(it_symbol-output_syntax.begin_terminal())*set_list.size()+set_index] = Syntax::Parser_unaccepting_state;
		}

		for(const Item& item : set_list[set_index].item_list) {
			int j;
			if(item.cursor == item.rule.word_list.size()) {
				output_syntax.parser_action_table()[output_syntax.index_of_symbol(item.next_token)*set_list.size()+set_index] = Syntax::Parser_action_reduce_mask | output_syntax.index_of_symbol(item.rule.left_member);
				std::cout << "Action[" << set_index << ", " << item.next_token.string() << "] : r " << output_syntax.index_of_symbol(item.rule.left_member) << std::endl;
			}
			else if(item.rule.word_list[item.cursor].is_terminal && (j = find_goto(ebnf_syntax, output_syntax, rule_list, set_list[set_index], item.rule.word_list[item.cursor].content, set_list)) != -1) {
				output_syntax.parser_action_table()[output_syntax.index_of_symbol(item.rule.word_list[item.cursor].content)*set_list.size()+set_index] = Syntax::Parser_action_shift_mask | j;
				std::cout << "Action[" << set_index << ", " << item.rule.word_list[item.cursor].content.string() << "] : s " << j << std::endl;
			}
			else {

			}
		}

		for(auto it_symbol = output_syntax.begin_non_terminal(); it_symbol != output_syntax.end_non_terminal(); ++it_symbol) {
			int j;
			if((j = find_goto(ebnf_syntax, output_syntax, rule_list, set_list[set_index], *it_symbol, set_list)) != -1) {
				output_syntax.parser_goto_table()[(it_symbol-output_syntax.begin_non_terminal())*set_list.size()+set_index] = j;
				std::cout << "Goto[" << set_index << ", " << it_symbol->string() << "] : " << j << std::endl;
			}
			else {
				output_syntax.parser_goto_table()[(it_symbol-output_syntax.begin_non_terminal())*set_list.size()+set_index] = Syntax::Parser_unaccepting_state;
			}
		}
	}

}

int ParserGenerator::find_goto(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, const Set& input_set, const TokenSymbol& symbol, const std::vector<Set>& set_list) {
	Set result;
	goto_procedure(ebnf_syntax, generated_syntax, rule_list, input_set, symbol, result);

	for(unsigned int i=0; i<set_list.size(); i++) {
		if(result.item_list.size() == set_list[i].item_list.size() && std::is_permutation(std::begin(result.item_list), std::end(result.item_list), std::begin(set_list[i].item_list)))
			return i;
	}
	return -1;
}
