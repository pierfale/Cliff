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
		enum RuleFlag {
			ListHead = 0x1,
			ListTail = 0x2,
			ListReduce = 0x4
		};

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



			RuleDefinition(Type type, unsigned int flags = 0);
			RuleDefinition(Type type, const TokenSymbol* symbol, unsigned int flags = 0);
			RuleDefinition(const RuleDefinition& that);
			RuleDefinition(RuleDefinition&& that);

			RuleDefinition& add_child(Type type, unsigned int flags = 0);
			RuleDefinition& add_child(Type type, const TokenSymbol* symbol, unsigned int flags = 0);
			RuleDefinition& add_child(const RuleDefinition& that);



			Type type() const;
			unsigned int flags() const;
			const TokenSymbol& content() const;
			std::vector<RuleDefinition>& list();
			const std::vector<RuleDefinition>& list() const;

		private:
			Type _type;
			unsigned int _flags;
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

		bool is_temporary_symbol_value(const TokenSymbol& symbol) const;
		const std::vector<TokenSymbol>& temporary_rule_name() const;

		void print(std::ostream& stream) const;

	private:
		void construct(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node, RawRuleList& rule_list);
		void construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, RawRuleList& rule_list, const AbstractSyntaxTree& current_node, RuleDefinition& current_rule, const TokenSymbol& current_rule_name);
		void inline_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawRuleList& rule_list);
		void inline_rule_definition(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleDefinition& current_input_rule, std::vector<std::pair<std::vector<Symbol>, unsigned int>>& alternative_list);

		const TokenSymbol* _entry_rule;
		std::map<const TokenSymbol*, Rule> _rule_list;
		std::vector<TokenSymbol> _temporary_rule_name;

	};
}
#endif
