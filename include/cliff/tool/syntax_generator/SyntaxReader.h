#ifndef _CLIFF_SYNTAX_GENERATOR_SYNTAX_READER_H
#define _CLIFF_SYNTAX_GENERATOR_SYNTAX_READER_H

#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/front_end/Lexer.h"
#include "cliff/front_end/Parser.h"
#include "cliff/shared/TreeTransformer.h"
#include "cliff/shared/TreeComparatorOnline.h"

namespace cliff {

	class SyntaxReader {

	public:

		static AbstractSyntaxTree& execute(const char* filename, Syntax& syntax, MemoryContainer<AbstractSyntaxTree>& output_container);

	};
}
#endif
