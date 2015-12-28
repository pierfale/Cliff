#ifndef _CLIFF_PARSER_GENERATOR_H
#define _CLIFF_PARSER_GENERATOR_H

#include <algorithm>
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/AbstractSyntaxTree.h"

namespace cliff {

	class ParserGenerator {

	public:
		static void generate_parser(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, Syntax& output_syntax);

	private:
		struct Word {
			Word(bool is_terminal_, const TokenSymbol& content_) : is_terminal(is_terminal_), content(content_) {

			}

			bool operator==(const Word& that) const {
				return &content == &that.content;
			}

			bool is_terminal;
			const TokenSymbol& content;
		};

		struct Rule {

			Rule(const TokenSymbol& left_member_) : left_member(left_member_) {

			}

			bool operator==(const Rule& that) const {
				return word_list.size() == that.word_list.size() && std::equal(std::begin(word_list), std::end(word_list), std::begin(that.word_list));
			}

			const TokenSymbol& left_member;
			std::vector<Word> word_list;
		};

		struct Item {
			Item(const Rule& rule_, unsigned int cursor_, const TokenSymbol& next_token_) : rule(rule_), cursor(cursor_), next_token(next_token_) {

			}

			Item(const Item& that) : rule(that.rule), cursor(that.cursor), next_token(that.next_token) {

			}

			bool operator==(const Item& that) const {
				return rule == that.rule && cursor == that.cursor && next_token == that.next_token;
			}

			const Rule& rule;
			unsigned int cursor;
			const TokenSymbol& next_token;
		};

		struct Set {

			Set() : item_list(), transition_symbol(nullptr), predecessor_set(0) {

			}

			Set(const Set& that) : item_list(that.item_list), transition_symbol(that.transition_symbol), predecessor_set(that.predecessor_set) {

			}

			bool operator==(const Set& that) const {
				return item_list.size() == that.item_list.size() && std::is_permutation(std::begin(item_list), std::end(item_list), std::begin(that.item_list));
			}

			std::vector<Item> item_list;
			const TokenSymbol* transition_symbol;
			unsigned int predecessor_set;

		};

		typedef std::map<const TokenSymbol*, std::vector<Rule>> RuleList;

		static void construct_rule_list(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& syntax_tree, RuleList& rule_list, const char** main_rule);
		static void construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& syntax_tree, Rule& rule);
		static void construct_closure_set(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, std::vector<Set>& output);
		static void closure_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, Set& output_set);
		static void goto_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, const Set& input_set, const TokenSymbol& symbol, Set& output_set);
		static void get_first(const Syntax& ebnf_syntax, const RuleList& rule_list, const Word& word, std::vector<const TokenSymbol*>& output);
		static void _get_first(const Syntax& ebnf_syntax, const RuleList& rule_list, const Word& word, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history);
		static bool is_epsilon_productive(const Syntax& ebnf_syntax, const RuleList& rule_list, const Word& word);
		static void generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, const RuleList& rule_list, const std::vector<Set>& set_list);
		static int find_goto(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleList& rule_list, const Set& input_set, const TokenSymbol& symbol, const std::vector<Set>& set_list);

	};
}
#endif
