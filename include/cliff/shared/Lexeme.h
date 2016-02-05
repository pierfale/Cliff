#ifndef _CLIFF_LEXEME_H
#define _CLIFF_LEXEME_H

#include <vector>
#include "cliff/shared/Token.h"

namespace cliff {

	class Lexeme {

	public:
		Lexeme();

	private:
		std::vector<const TokenSymbol*> _symbols;
		char* _content;

	};
}
#endif
