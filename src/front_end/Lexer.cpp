#include "cliff/front_end/Lexer.h"

using namespace cliff;

Lexer::Lexer(const Syntax& syntax, Reader& input) : _syntax(syntax), _input(input), _current_state(Syntax::Lexer_init_state), _last_accepting_state(nullptr) {

}

/*
template<typename Previous>
void Lexer<Previous>::execute(std::istream& input, std::vector<Token>& output) {
	if(!input.good()) {
		THROW(exception::Exception, "Bad stream state");
	}

	Syntax::Letter current_letter;
	Syntax::State current_state = Syntax::Lexer_init_state;
	const TokenSymbol* last_accepting_state = nullptr;

	std::queue<Syntax::Letter> unaccepted_letter_buffer;
	std::queue<Syntax::Letter> accepted_letter_buffer;
	std::queue<Syntax::Letter> restart_letter_buffer;

	while(!restart_letter_buffer.empty() || (current_letter = input.get()) != EOF) {

		if(!restart_letter_buffer.empty()) {
			current_letter = restart_letter_buffer.front();
			restart_letter_buffer.pop();
		}

		unaccepted_letter_buffer.push(current_letter);
		current_state = _syntax.next_lexer_state(current_state, current_letter);

		if(current_state == Syntax::Lexer_state_error && last_accepting_state == nullptr) {
			THROW(exception::Exception, "Lexer : Unrecognized word"); // TODO better lexer eception
		}
		else if(current_state == Syntax::Lexer_state_error) {
			std::string token_content;
			token_content.reserve(accepted_letter_buffer.size());
			while(!accepted_letter_buffer.empty()) {
				token_content.append(1, accepted_letter_buffer.front());
				accepted_letter_buffer.pop();
			}
			output.push_back(Token(*last_accepting_state, token_content.c_str()));
			last_accepting_state = nullptr;
			current_state = Syntax::Lexer_init_state;

			while (!unaccepted_letter_buffer.empty()) {
				restart_letter_buffer.push(unaccepted_letter_buffer.front());
				unaccepted_letter_buffer.pop();
			}
		}

		const TokenSymbol* current_symbol = _syntax.lexer_accepting_state(current_state);
		if(current_symbol != nullptr) {
			while(!unaccepted_letter_buffer.empty()) {
				accepted_letter_buffer.push(unaccepted_letter_buffer.front());
				unaccepted_letter_buffer.pop();
			}
			last_accepting_state = current_symbol;
		}
	}

	if(!unaccepted_letter_buffer.empty()) {
		THROW(exception::Exception, "Lexer : Unrecognized word (end)"); // TODO better lexer eception
	}

	if(last_accepting_state != nullptr) {
		std::string token_content;
		token_content.insert(0, accepted_letter_buffer.size(), accepted_letter_buffer.front());
		output.push_back(Token(*last_accepting_state, token_content.c_str()));
	}

	// push eof
	output.push_back(Token(_syntax.get_symbol_from_name(Syntax::EOF_symbol)));
}*/

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

		std::cout << current_letter << std::endl;

		if(current_letter == EOF) { // EOF
			if(!_unaccepted_letter_buffer.empty()) {
				THROW(exception::Exception, "Lexer : Unrecognized word (end)"); // TODO better lexer eception
			}

			if(_last_accepting_state != nullptr) {
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
			output.set(*_last_accepting_state, token_content.c_str());

			_last_accepting_state = nullptr;
			_current_state = Syntax::Lexer_init_state;

			while (!_unaccepted_letter_buffer.empty()) {
				_restart_letter_buffer.push(_unaccepted_letter_buffer.front());
				_unaccepted_letter_buffer.pop();
			}


			return true;
		}

		const TokenSymbol* current_symbol = _syntax.lexer_accepting_state(_current_state, parser_state);
		if(current_symbol != nullptr) {
			while(!_unaccepted_letter_buffer.empty()) {
				_accepted_letter_buffer.push(_unaccepted_letter_buffer.front());
				_unaccepted_letter_buffer.pop();
			}
			_last_accepting_state = current_symbol;
		}
	}
}
