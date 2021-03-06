#ifndef _CLIFF_PARSER_SYNTAX_GENERATOR_GENERATOR_H
#define _CLIFF_PARSER_SYNTAX_GENERATOR_GENERATOR_H

#include <algorithm>
#include <stack>

#include "cliff/front_end/Syntax.h"
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"

namespace cliff {

	class ParserGenerator {

	public:
		static void generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, SyntaxRepresentation& syntax_representation, std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list);

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
				return rule < that.rule || (rule == that.rule
						&& (cursor < that.cursor || (cursor == that.cursor
						&& (next_token < that.next_token))));
			}

			void following_symbols(std::vector<std::pair<SyntaxRepresentation::Symbol, bool>>& output) const {
				if(cursor < rule->sequence().size())
					output.push_back(std::make_pair(rule->sequence()[cursor], false));
			}

			void consume(const TokenSymbol& symbol,  std::vector<Item>& output) const {
				if(cursor < rule->sequence().size()) {
					if(rule->sequence()[cursor].content() == symbol)
						output.emplace_back(*rule, cursor+1, *next_token);
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

				stream << ", " << next_token->string() << ", flags=" << std::hex << rule->flags() << std::dec << std::endl;
			}

			const SyntaxRepresentation::InlinedAlternative* rule;
			unsigned int cursor;
			const TokenSymbol* next_token;
		};

		struct Set {

			Set() : item_list(), transition_symbol(nullptr), predecessor_set(0), is_repetition(false) {

			}

			Set(const Set& that) : item_list(that.item_list), transition_symbol(that.transition_symbol), predecessor_set(that.predecessor_set), is_repetition(that.is_repetition) {

			}

			void erase_duplicate() {
				//std::cout << item_list.size() << std::endl;
				std::sort(std::begin(item_list), std::end(item_list));
				item_list.erase(std::unique(std::begin(item_list), std::end(item_list)), std::end(item_list));
			}

			bool operator==(const Set& that) const {
				return item_list.size() == that.item_list.size() && std::is_permutation(std::begin(item_list), std::end(item_list), std::begin(that.item_list));
			}

			std::vector<Item> item_list;
			const TokenSymbol* transition_symbol;
			unsigned int predecessor_set;
			bool is_repetition;

		};

		static void construct_closure_set(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, std::vector<Set>& output);
		static void closure_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, Set& output_set);
		static void goto_procedure(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, const Set& input_set, const TokenSymbol& symbol, Set& output_set);
		static void generate_parser(const Syntax& ebnf_syntax, Syntax& output_syntax, const SyntaxRepresentation& syntax_representation, const std::vector<Set>& set_list, std::vector<const DeterministeFiniteAutomataNode*>& lexer_state_list);
		static int find_goto(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const SyntaxRepresentation& syntax_representation, const Set& input_set, const TokenSymbol& symbol, const std::vector<Set>& set_list);
		static void compute_first_of(const SyntaxRepresentation& syntax_representation, const std::vector<SyntaxRepresentation::Symbol>& list, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history);
		static bool reachable_lexing_state(const DeterministeFiniteAutomataNode& start_node, std::vector<const TokenSymbol*> possible_accepting_token);

		static void set_action_table_or_else(Syntax& output_syntax, unsigned int index, unsigned int action, unsigned int reduce_number, exception::UserMessage&& exception);
		static void set_lexer_accepting_state_or_else(Syntax& output_syntax, unsigned int index, unsigned int value, exception::UserMessage&& exception);
		static void set_lexer_accepting_state_if_none(Syntax& output_syntax, unsigned int index, unsigned int value);
	};
}
#endif
