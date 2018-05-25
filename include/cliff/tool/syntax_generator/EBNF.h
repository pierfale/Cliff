#ifndef _CLIFF_SYNTAX_GENERATOR_EBNF_H
#define _CLIFF_SYNTAX_GENERATOR_EBNF_H

#include <algorithm>
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/QualifiedIdentifier.h"
#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"
#include "cliff/tool/syntax_generator/EBNFConstruct.h"
#include "cliff/tool/syntax_generator/EBNFRepresentation.h"
namespace cliff {

	enum EBNFSymbolType {
		//EBNF_TERMINAL,
		//EBNF_NON_TERMINAL
	};

	class EBNF {

	public:
		EBNF();

		void load();

		//void generate_syntax(const AbstractSyntaxTree& syntax_tree);
		//void extract_symbols(const AbstractSyntaxTree& syntax_tree, QualifiedIdentifier<EBNFSymbolType> current_prefix);
		//void generate_symbol_name(QualifiedIdentifier<EBNFSymbolType> node, std::vector<std::pair<std::string, bool>>& output);

	private:
		Syntax _syntax;

	};
}

#endif
