#ifndef _CLIFF_SYNTAX_REPRESENTATION_H
#define _CLIFF_SYNTAX_REPRESENTATION_H

#include <algorithm>
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/TreeTransformer.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/Exception.h"

namespace cliff {

	class SyntaxRepresentation {

	public:
		class RuleDefinition {

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

			RuleDefinition(Type type, const TokenSymbol* parent_rule_name, bool unbound_repetition);
			RuleDefinition(Type type, const TokenSymbol* symbol, const TokenSymbol* parent_rule_name, bool unbound_repetition);
			RuleDefinition(const RuleDefinition& that);
			RuleDefinition(RuleDefinition&& that);

			RuleDefinition& add_child(Type type);
			RuleDefinition& add_child(Type type, const TokenSymbol* symbol);
			RuleDefinition& add_child(const RuleDefinition& that);

			std::vector<RuleDefinition>& list();
			const std::vector<RuleDefinition>& list() const;


			bool unbound_repetition() const;
			Type type() const;
			const TokenSymbol& content() const;
			const TokenSymbol* parent_rule_name() const;

		private:
			bool _unbound_repetition;
			Type _type;
			const TokenSymbol* _symbol;
			const TokenSymbol* _parent_rule_name;
			std::vector<RuleDefinition> _rule_list;
		};

		typedef std::map<const TokenSymbol*, RuleDefinition> RawRuleList;

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
			InlinedAlternative(const TokenSymbol& _rule_name, const std::vector<Symbol>& rule_definition, const TokenSymbol* parent_rule_name = nullptr, bool unbound_repetition = false);

			void first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output) const;
			bool is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position) const;

			bool unbound_repetition() const;
			const std::vector<Symbol>& sequence() const;
			const TokenSymbol& rule_name() const;
			const TokenSymbol* parent_rule_name() const;
			bool has_unbound_repetition() const;

			void _first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const;
			bool _is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& history) const;

		private:

			const TokenSymbol& _rule_name;
			const TokenSymbol* _parent_rule_name;
			bool _unbound_repetition;
			std::vector<Symbol> _sequence;
		};

		class Rule {

			friend class InlinedAlternative;

		public:
			Rule(const TokenSymbol& rule_name, const TokenSymbol* parent_rule_name, bool unbound_repetition);
			Rule(const TokenSymbol& rule_name, RuleDefinition::Type root_type, const TokenSymbol* parent_rule_name, bool unbound_repetition);
			Rule(const TokenSymbol& rule_name, RuleDefinition::Type root_type, const TokenSymbol* root_symbol, const TokenSymbol* parent_rule_name, bool unbound_repetition);

			void add_alternative(const std::vector<Symbol>& rule_definition);
			void first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output) const;
			bool is_epsilon_productive(const SyntaxRepresentation& syntax_representation) const;


			bool unbound_repetition() const;
			const TokenSymbol& rule_name() const;
			const TokenSymbol* parent_rule_name() const;
			const std::vector<InlinedAlternative>& alternatives() const;

			RuleDefinition& root_rule_definition();
			const RuleDefinition& root_rule_definition() const;

			void print(std::ostream& stream) const;
		private:
			void _first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const;
			bool _is_epsilon_productive(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& history) const;

			bool _unbound_repetition;
			const TokenSymbol& _rule_name;
			const TokenSymbol* _parent_rule_name;
			RuleDefinition _root_rule_definition; // TODO remove
			std::vector<InlinedAlternative> _alternative_list;

		};

		SyntaxRepresentation(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& input_syntax);

		Rule& get_rule_by_symbol(const TokenSymbol& symbol);
		const Rule& get_rule_by_symbol(const TokenSymbol& symbol) const;

		bool is_temporary_symbol_value(const TokenSymbol& symbol) const;
		const std::vector<TokenSymbol>& temporary_rule_name() const;

		void print(std::ostream& stream) const;

	private:
		void construct(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node, RawRuleList& rule_list);
		void construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, RawRuleList& rule_list, const AbstractSyntaxTree& current_node, RuleDefinition& current_rule, const TokenSymbol& current_rule_name);
		void inline_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawRuleList& rule_list);
		void inline_rule_definition(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleDefinition& current_input_rule, std::vector<std::vector<Symbol>>& alternative_list);

		const TokenSymbol* _entry_rule;
		std::map<const TokenSymbol*, Rule> _rule_list;
		std::vector<TokenSymbol> _temporary_rule_name;

	};
}
#endif
