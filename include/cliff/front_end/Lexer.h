#ifndef _CLIFF_LEXER_H
#define _CLIFF_LEXER_H

#include <istream>
#include <vector>
#include <queue>

#include <iostream>
#include <iomanip>

#include "cliff/shared/Exception.h"
#include "cliff/front_end/Syntax.h"
#include "cliff/shared/Token.h"
#include "cliff/front_end/FrontChain.h"
#include "cliff/front_end/Reader.h"

namespace cliff {


class Lexer {

	public:
		Lexer(const Syntax& syntax, Reader& input);
		bool process_next(Syntax::State parser_state, Token& output);

	private:
		const Syntax& _syntax;
		Reader& _input;

		Syntax::State _current_state;
		const TokenSymbol* _last_accepting_state;
		Syntax::Index _last_accepting_state_flags;

		std::queue<Syntax::Letter> _unaccepted_letter_buffer;
		std::queue<Syntax::Letter> _accepted_letter_buffer;
		std::queue<Syntax::Letter> _restart_letter_buffer;
	};
}

#endif
