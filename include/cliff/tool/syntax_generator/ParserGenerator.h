#ifndef _CLIFF_PARSER_GENERATOR_H
#define _CLIFF_PARSER_GENERATOR_H

#include <algorithm>
#include <stack>

#include "cliff/front_end/Syntax.h"
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"

namespace cliff {

	class ParserGenerator {

	public:
		static void generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, SyntaxRepresentation& syntax_representation);

	private:

		struct Item {
			Item(const SyntaxRepresentation::InlinedAlternative& rule_, unsigned int cursor_, const TokenSymbol& next_token_) : rule(&rule_), cursor(cursor_), next_token(&next_token_) {

			}

			Item(const Item& that) : rule(that.rule), cursor(that.cursor), next_token(that.next_token) {

			}

			Item& operator=(const Item&& that) {
				rule = that.rule;
				cursor = that.cursor;
				next_token = that.next_token;
			}

			bool operator==(const Item& that) const {
				return rule == that.rule && cursor == that.cursor && next_token == that.next_token;
			}

			bool operator<(const Item& that) const {
				return rule < that.rule ||
						(rule == that.rule && cursor < that.cursor ||
						(cursor == that.cursor && next_token < that.next_token));
			}

			void following_symbols(std::vector<SyntaxRepresentation::Symbol>& output) const {
				if(cursor < rule->sequence().size())
					output.push_back(rule->sequence()[cursor]);
				else if(rule->has_unbound_repetition())
					output.push_back(rule->sequence()[0]);
			}

			void consume(const TokenSymbol& symbol,  std::vector<Item>& output) const {
				if(cursor < rule->sequence().size()) {
					if(rule->sequence()[cursor].content() == symbol)
						output.emplace_back(*rule, cursor+1, *next_token);
				}
				else if(rule->has_unbound_repetition()) {
					if(rule->sequence()[0].content() == symbol)
						output.emplace_back(*rule, 1, *next_token);
				}
			}

			void print(std::ostream& stream) const {
				stream << rule->rule_name().string() << " :=";

				for(unsigned int i=0; i<rule->sequence().size(); i++) {
					if(cursor == i)
						stream << " @";

					stream << " " << rule->sequence()[i].content().string();
				}
				if(cursor == rule->sequence().size())
					stream << " @";

				stream << ", " << next_token->string() << std::endl;
			}

			const SyntaxRepresentation::InlinedAlternative* rule;
			unsigned int cursor;
			const TokenSymbol* next_token;
		};

		struct Set {

			Set() : item_list(), transition_symbol(nullptr), predecessor_set(0) {

			}

			Set(const Set& that) : item_list(that.item_list), transition_symbol(that.transition_symbol), predecessor_set(that.predecessor_set) {

			}

			void erase_duplicate() {
				std::sort(std::begin(item_list), std::end(item_list));
				item_list.erase(std::unique(std::begin(item_list), std::end(item_list)), std::end(item_list));
			}

			bool operator==(const Set& that) const {
				return item_list.size() == that.item_list.size() && std::is_permutation(std::begin(item_list), std::end(item_list), std::begin(that.item_list));
			}

			std::vector<Item> item_list;
			const TokenSymbol* transition_symbol;
			unsigned int predecessor_set;

		};

		static void construct_closure_set(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, std::vector<Set>& output);
		static void closure_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, Set& output_set);
		static void goto_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, const Set& input_set, const TokenSymbol& symbol, Set& output_set);
		static void generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, const SyntaxRepresentation& syntax_representation, const std::vector<Set>& set_list);
		static int find_goto(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, const Set& input_set, const TokenSymbol& symbol, const std::vector<Set>& set_list);

	};
}
#endif
