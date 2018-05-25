#ifndef _CLIFF_SYNTAX_GENERATOR_EBNF_CONSTRUCT_H
#define _CLIFF_SYNTAX_GENERATOR_EBNF_CONSTRUCT_H

#include <algorithm>
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/QualifiedIdentifier.h"
#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"
#include "cliff/tool/syntax_generator/EBNFRepresentation.h"

namespace cliff {


	class EBNFConstruct {

	public:
		EBNFConstruct() = delete;

		static void process(const Syntax& syntax, const AbstractSyntaxTree& input, EBNFScope& output);

	private:
		static void process_scope(const Syntax& syntax, const AbstractSyntaxTree& input, EBNFScope& output);
		static void process_rule(const Syntax& syntax, const AbstractSyntaxTree& input, EBNFOperation& output);
		static void process_regular_expression();
		static void process_literal_string();


		static void bind_reference(EBNFScope& scope, std::vector<EBNFScope*>& prefix);
		static void bind_reference(EBNFOperation& scope, const std::vector<EBNFScope*>& prefix);

		static bool resolve_name(EBNFScope& scope, const std::vector<const char*>& name, size_t name_index);

		Syntax _syntax;

	};
}

#endif
