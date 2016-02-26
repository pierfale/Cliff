#include "cliff/front_end/Lexer.h"

using namespace cliff;

Lexer::Lexer(const Syntax& syntax, Reader& input) : _syntax(syntax), _input(input), _current_state(Syntax::Lexer_init_state), _last_accepting_state(nullptr) {

}

bool Lexer::process_next(Syntax::State parser_state, Token& output) {
	Syntax::Letter current_letter;

	while(true) { // while no accepting state found

		if(!_restart_letter_buffer.empty()) {
			current_letter = _restart_letter_buffer.front();
			_restart_letter_buffer.pop();
		}
		else if(!_input.process_next(current_letter)) {
			current_letter = EOF;
		}

		if(current_letter == EOF) { // EOF

			if(_last_accepting_state_flags & Syntax::Lexer_accepting_state_ignore) {
				_last_accepting_state = nullptr;
			}
			else if(!_unaccepted_letter_buffer.empty()) {
				THROW(exception::Exception, "Lexer : Unrecognized word (end)"); // TODO better lexer eception
			}
			else if(_last_accepting_state != nullptr) {
				std::string token_content;
				token_content.insert(0, _accepted_letter_buffer.size(), _accepted_letter_buffer.front());
				output.set(*_last_accepting_state, token_content.c_str());
				_last_accepting_state = nullptr;
				return true;
			}
			output.set(_syntax.get_symbol_from_name(Syntax::EOF_symbol));
			return false;
		}

		_unaccepted_letter_buffer.push(current_letter);
		_current_state = _syntax.next_lexer_state(_current_state, current_letter);

		if(_syntax.is_disabled_path(_current_state, parser_state)) {
			_current_state = Syntax::Lexer_state_error;
		}


		if(_current_state == Syntax::Lexer_state_error && _last_accepting_state == nullptr) {
			THROW(exception::Exception, "Lexer : Unrecognized word"); // TODO better lexer eception
		}
		else if(_current_state == Syntax::Lexer_state_error) {
			std::string token_content;
			token_content.reserve(_accepted_letter_buffer.size());
			while(!_accepted_letter_buffer.empty()) {
				token_content.append(1, _accepted_letter_buffer.front());
				_accepted_letter_buffer.pop();
			}

			while (!_unaccepted_letter_buffer.empty()) {
				_restart_letter_buffer.push(_unaccepted_letter_buffer.front());
				_unaccepted_letter_buffer.pop();
			}

			if(!(_last_accepting_state_flags & Syntax::Lexer_accepting_state_ignore)) {
				output.set(*_last_accepting_state, token_content.c_str());

				_last_accepting_state = nullptr;
				_current_state = Syntax::Lexer_init_state;

				return true;
			}
			else {
				_last_accepting_state = nullptr;
				_current_state = Syntax::Lexer_init_state;
			}
		}

		const TokenSymbol* current_symbol = _syntax.lexer_accepting_state(_current_state, parser_state, _last_accepting_state_flags);
		if(current_symbol != nullptr) {
			while(!_unaccepted_letter_buffer.empty()) {
				_accepted_letter_buffer.push(_unaccepted_letter_buffer.front());
				_unaccepted_letter_buffer.pop();
			}
			_last_accepting_state = current_symbol;
		}
	}
}
