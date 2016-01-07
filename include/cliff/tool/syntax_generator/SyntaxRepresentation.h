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

			RuleDefinition(RuleDefinition* parent, unsigned int parent_relative_position, Type type);
			RuleDefinition(RuleDefinition* parent, unsigned int parent_relative_position, Type type, const TokenSymbol* symbol);
			RuleDefinition(const RuleDefinition& that) = delete;
			RuleDefinition(RuleDefinition&& that);

			RuleDefinition& add_child(Type type);
			RuleDefinition& add_child(Type type, const TokenSymbol* symbol);

			std::pair<const RuleDefinition*, unsigned int> adjust(unsigned int cursor_position) const;

			bool is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int start_position = 0, bool expend_non_terminal = true) const;
			void next_definition_of_type_list(const SyntaxRepresentation& syntax_representation, unsigned int cursor, Type type, std::vector<const RuleDefinition*>& output) const;
			bool first_list(const SyntaxRepresentation& syntax_representation, unsigned int cursor, std::vector<const TokenSymbol*>& output) const;


			std::vector<RuleDefinition>& list();
			const std::vector<RuleDefinition>& list() const;

			const RuleDefinition* parent() const;
			unsigned int parent_relative_position() const;
			Type type() const;
			const TokenSymbol& content() const;

			void print(std::ostream& stream) const;

		private:
			bool _is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int start_position, bool expend_non_terminal, std::map<const TokenSymbol*, bool>& history) const;
			bool _first_list(const SyntaxRepresentation& syntax_representation, unsigned int cursor, std::vector<const TokenSymbol*>& output, std::map<const TokenSymbol*, bool>& history) const;

			RuleDefinition* _parent;
			unsigned int _parent_relative_position;
			Type _type;
			const TokenSymbol* _symbol;
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
			InlinedAlternative(const TokenSymbol& _rule_name, const std::vector<Symbol>& rule_definition, bool unbound_repetition = false);

			void first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output) const;
			bool is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position) const;

			const std::vector<Symbol>& sequence() const;
			const TokenSymbol& rule_name() const;
			bool has_unbound_repetition() const;

			void _first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const;
			bool _is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& history) const;

		private:

			const TokenSymbol& _rule_name;
			bool _unbound_repetition;
			std::vector<Symbol> _sequence;
		};

		class Rule {

			friend class InlinedAlternative;

		public:
			Rule(const TokenSymbol& rule_name);
			Rule(const TokenSymbol& rule_name, RuleDefinition::Type root_type);
			Rule(const TokenSymbol& rule_name, RuleDefinition::Type root_type, const TokenSymbol* root_symbol);

			void add_alternative(const std::vector<Symbol>& rule_definition);
			void first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output) const;
			bool is_epsilon_productive(const SyntaxRepresentation& syntax_representation) const;


			const TokenSymbol& rule_name() const;
			const std::vector<InlinedAlternative>& alternatives() const;

			RuleDefinition& root_rule_definition();
			const RuleDefinition& root_rule_definition() const;

			void print(std::ostream& stream) const;
		private:
			void _first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const;
			bool _is_epsilon_productive(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& history) const;

			const TokenSymbol& _rule_name;
			RuleDefinition _root_rule_definition; // TODO remove
			std::vector<InlinedAlternative> _alternative_list;

		};

		SyntaxRepresentation(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& input_syntax);

		Rule& get_rule_by_symbol(const TokenSymbol& symbol);
		const Rule& get_rule_by_symbol(const TokenSymbol& symbol) const;

		void print(std::ostream& stream) const;

	private:
		void construct(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node, RawRuleList& rule_list);
		void construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, RawRuleList& rule_list, const AbstractSyntaxTree& current_node, RuleDefinition& current_rule);
		void inline_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawRuleList& rule_list);
		void inline_rule_definition(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleDefinition& current_input_rule, std::vector<std::vector<Symbol>>& alternative_list);

		const TokenSymbol* _entry_rule;
		std::map<const TokenSymbol*, Rule> _rule_list;
		std::vector<TokenSymbol> temporary_rule_name;

	};
}
#endif
