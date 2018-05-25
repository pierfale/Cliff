#ifndef _CLIFF_SYNTAX_GENERATOR_SYNTAX_GENERATOR_H
#define _CLIFF_SYNTAX_GENERATOR_SYNTAX_GENERATOR_H

#include <iostream>
#include <fstream>
#include <stack>
#include <map>

#include "cliff/shared/EntryModule.h"
#include "cliff/ProgramOption.h"
#include "cliff/EntryBoundRegister.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/front_end/Lexer.h"
#include "cliff/front_end/Parser.h"
#include "cliff/front_end/Reader.h"
#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/TreeComparatorOnline.h"
#include "cliff/shared/TreeTransformer.h"
#include "cliff/tool/syntax_generator/LexerGenerator.h"
#include "cliff/tool/syntax_generator/ParserGenerator.h"
#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"
#include "cliff/tool/syntax_generator/SyntaxReader.h"
#include "cliff/tool/syntax_generator/EBNF.h"
#include "cliff/shared/QualifiedIdentifier.h"

namespace cliff {

	class SyntaxGenerator : public EntryModule {

	public:
		virtual void initialize();
		virtual void execute(ProgramOption::Iterator option_caller);

	private:
		//void create_syntax(const Syntax& ebnf_syntax, const AbstractSyntaxTree& ast);
		//void get_symbols_name(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, std::vector<std::pair<const char*, bool>>& symbols_name);
	};

	static RegisterModule<SyntaxGenerator, Module::Order::PreCompiler> _syntax_generator_module;
}

#endif
