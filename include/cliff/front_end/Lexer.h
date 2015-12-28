#ifndef _CLIFF_LEXER_H
#define _CLIFF_LEXER_H

#include <istream>
#include <vector>
#include <queue>

#include <iostream>

#include "cliff/shared/Exception.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/Token.h"

namespace cliff {

	class Lexer {

	public:
		Lexer(const Syntax& syntax);

		void execute(std::istream& input, std::vector<Token>& output);

	private:
		const Syntax& _syntax;

	};
}

#endif
