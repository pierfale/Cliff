#ifndef _CLIFF_SYNTAX_GENERATOR_EBNF_REPRESENTATION_H
#define _CLIFF_SYNTAX_GENERATOR_EBNF_REPRESENTATION_H

#include <algorithm>
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/QualifiedIdentifier.h"
#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"

namespace cliff {

	enum EBNFOperationType {

		EBNF_ROOT,
		EBNF_ALTERNATIVE,
		EBNF_SEQUENCE,
		EBNF_REPETITION,
		EBNF_OPTIONAL,
		EBNF_NON_TERMINAL,
		EBNF_SCOPE_NAME,
		EBNF_TERMINAL,

	};

	class EBNFOperation {

		friend class EBNFConstruct;

	public:
		EBNFOperation(EBNFOperationType type);
		EBNFOperation(EBNFOperationType type, const char* content);

		void print(std::ostream& stream) const;

	private:
		EBNFOperationType _type;
		const char* _content;
		std::vector<EBNFOperation> _children;
	};

	class EBNFScope {

		friend class EBNFConstruct;

	public:
		EBNFScope(EBNFScope* parent_scope, const std::string& name);

		void print(std::ostream& stream) const;

	private:

		void _print(std::ostream& stream, size_t ltab) const;

		std::string _scope_name;

		EBNFScope* _parent_scope;
		std::map<std::string, EBNFScope> _children_scope;

		EBNFOperation _lexer_omit_token;
		std::map<std::string, EBNFOperation> _rules;
		std::map<std::string, RegularExpressionRepresentation> _regular_expressions;

	};


}

#endif
