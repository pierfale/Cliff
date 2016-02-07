#ifndef _CLIFF_SYNTAX_REPRESENTATION_H
#define _CLIFF_SYNTAX_REPRESENTATION_H

#include <algorithm>
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/TreeTransformer.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/Exception.h"
#include "cliff/tool/syntax_generator/RegularExpressionRepresentation.h"

namespace cliff {

	class SyntaxRepresentation {

	public:
		enum RuleFlag {
			ListHead = 0x1,
			ListTail = 0x2,
			ListReduce = 0x4
		};

		class RawSubRule {

		public:
			enum Type {
				Sequence,
				OptionalSequence,
				RepetitionSequence,
				RepetitionNotEmptySequence,
				Alternative,
				Terminal,
				NonTerminal
			};



			RawSubRule(Type type, unsigned int flags = 0);
			RawSubRule(Type type, const TokenSymbol* symbol, unsigned int flags = 0);
			RawSubRule(const RawSubRule& that);
			RawSubRule(RawSubRule&& that);

			RawSubRule& add_child(Type type, unsigned int flags = 0);
			RawSubRule& add_child(Type type, const TokenSymbol* symbol, unsigned int flags = 0);
			RawSubRule& add_child(const RawSubRule& that);



			Type type() const;
			unsigned int flags() const;
			const TokenSymbol& content() const;
			std::vector<RawSubRule>& list();
			const std::vector<RawSubRule>& list() const;

		private:
			Type _type;
			unsigned int _flags;
			const TokenSymbol* _symbol;
			std::vector<RawSubRule> _rule_list;
		};

		class RawRule {

		public:
			RawRule(RawSubRule::Type root_type, unsigned int flags = 0);
			RawRule(RawSubRule::Type root_type, const TokenSymbol* symbol, unsigned int flags = 0);

			RawSubRule& root();
			const RawSubRule& root() const;

		private:
			RawSubRule _root;
			RawSubRule _ignore_sequences;

		};

		typedef std::map<const TokenSymbol*, RawRule> RawRuleList;

		class Symbol {

		public:

			Symbol(const TokenSymbol& content, bool is_terminal);

			bool is_terminal() const;
			const TokenSymbol& content() const;

		private:
			bool _is_terminal;
			const TokenSymbol& _content;
		};

		class InlinedAlternative {

			friend class Rule;

		public:
			InlinedAlternative(const TokenSymbol& _rule_name, const std::vector<Symbol>& rule_definition, unsigned int flags);

			void first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output) const;
			bool is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position) const;

			unsigned int flags() const;
			const std::vector<Symbol>& sequence() const;
			const TokenSymbol& rule_name() const;

			void _first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const;
			bool _is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& history) const;

		private:

			unsigned int _flags;
			const TokenSymbol& _rule_name;
			std::vector<Symbol> _sequence;
		};

		class Rule {

			friend class InlinedAlternative;

		public:
			Rule(const TokenSymbol& rule_name);

			void add_alternative(const std::vector<Symbol>& rule_definition, unsigned int flags);
			void first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output) const;
			bool is_epsilon_productive(const SyntaxRepresentation& syntax_representation) const;

			const TokenSymbol& rule_name() const;
			const std::vector<InlinedAlternative>& alternatives() const;

			void print(std::ostream& stream) const;
		private:
			void _first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const;
			bool _is_epsilon_productive(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& history) const;

			const TokenSymbol& _rule_name;
			std::vector<InlinedAlternative> _alternative_list;

		};

		SyntaxRepresentation(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& input_syntax);

		Rule& get_rule_by_symbol(const TokenSymbol& symbol);
		const Rule& get_rule_by_symbol(const TokenSymbol& symbol) const;

		bool is_dummy_symbol_value(const TokenSymbol& symbol) const;
		const std::vector<TokenSymbol>& dummy_rule_name() const;

		std::map<const TokenSymbol*, RegularExpressionRepresentation>& regular_expression_list();
		const std::map<const TokenSymbol*, RegularExpressionRepresentation>& regular_expression_list() const;

		void print(std::ostream& stream) const;

	private:
		void construct_dummy_symbol(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node);
		void fill_regular_expression(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node);
		void construct(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node, RawRuleList& rule_list);
		void construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, RawRuleList& rule_list, const AbstractSyntaxTree& current_node, RawSubRule& current_rule, const TokenSymbol& current_rule_name);
		void inline_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawRuleList& rule_list);
		void inline_rule_definition(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawSubRule& current_input_rule, std::vector<std::pair<std::vector<Symbol>, unsigned int>>& alternative_list);

		const TokenSymbol* _entry_rule;
		std::map<const TokenSymbol*, Rule> _rule_list;
		std::map<const TokenSymbol*, RegularExpressionRepresentation> _regular_expression_list;
		std::vector<TokenSymbol> _dummy_rule_name;
		unsigned int _dummy_rule_cursor;

	};
}
#endif
