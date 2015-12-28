#include "cliff/front_end/Syntax.h"

using namespace cliff;

Syntax::Syntax() : _lexer_state_number(0), _lexer_table(nullptr) {

}

void Syntax::load(const char* filename) {
	std::ifstream file(filename, std::ios::in | std::ios::binary);

	if(!file.is_open()) {
		THROW(exception::FileNotFound, filename);
	}

	std::cout << "Read syntax file" << std::endl;
	std::cout << "================" << std::endl;

	uint32_t symbol_number;
	file.read((char*)&symbol_number, sizeof(uint32_t));
	uint32_t terminal_number;
	file.read((char*)&terminal_number, sizeof(uint32_t));

	std::cout << symbol_number << " symbols loaded" << std::endl;

	std::vector<const char*> symbols_name;
	for(unsigned int i=0; i<symbol_number; i++) {
		uint16_t symbol_name_size;
		file.read((char*)&symbol_name_size, sizeof(uint16_t));
		char* symbol_name = new char[symbol_name_size+1];
		file.read(symbol_name, symbol_name_size);
		symbol_name[symbol_name_size] = '\0';
		symbols_name.push_back(symbol_name);
		std::cout << "#" << i << " : " << symbol_name << std::endl;
	}

	set_symbol_table(symbols_name, terminal_number);

	uint32_t lexer_state_number;
	file.read((char*)&lexer_state_number, sizeof(uint32_t));

	set_lexer_table(lexer_state_number);

	file.read((char*)_lexer_accepting_state, _lexer_state_number*sizeof(Index));
	file.read((char*)_lexer_table, Direct_letter_range*_lexer_state_number*sizeof(State));

	uint32_t parser_state_number;
	file.read((char*)&parser_state_number, sizeof(uint32_t));

	set_parser_table(parser_state_number);

	file.read((char*)_lexer_accepting_state, _parser_state_number*(_symbol_number-_symbol_non_terminal_start)*sizeof(Index));
	file.read((char*)_lexer_table, _parser_state_number*_symbol_non_terminal_start*sizeof(Index));
}

void Syntax::save(const char* filename) {
	std::ofstream file(filename, std::ios::out | std::ios::trunc | std::ios::binary);

	if(!file) {
		THROW(exception::FileNotFound, filename);
	}

	uint32_t symbol_size = _symbol_number;
	file.write((char*)&symbol_size, sizeof(uint32_t));
	uint32_t terminal_number = _symbol_non_terminal_start;
	file.write((char*)&terminal_number, sizeof(uint32_t));

	for(unsigned int i=0; i<_symbol_number; i++) {
		uint16_t symbol_name_size = _symbol_table[i].size();
		file.write((char*)&symbol_name_size, sizeof(uint16_t));
		file.write(_symbol_table[i].string(), symbol_name_size);
	}

	uint32_t lexer_state_number = _lexer_state_number;
	file.write((char*)&lexer_state_number, sizeof(uint32_t));

	file.write((char*)_lexer_accepting_state, _lexer_state_number*sizeof(Index));
	file.write((char*)_lexer_table, Direct_letter_range*_lexer_state_number*sizeof(State));

	uint32_t parser_state_number = _parser_state_number;
	file.write((char*)&parser_state_number, sizeof(uint32_t));

	file.write((char*)_action_table, _parser_state_number*(_symbol_number-_symbol_non_terminal_start)*sizeof(Index));
	file.write((char*)_goto_table, _parser_state_number*_symbol_non_terminal_start*sizeof(Index));
}

//
//	Symbol
//

void Syntax::set_symbol_table(std::vector<const char*> symbols, unsigned int terminal_range) {
	_symbol_table = (TokenSymbol*)std::malloc(sizeof(TokenSymbol)*symbols.size());
	_symbol_number = symbols.size();
	_symbol_non_terminal_start = terminal_range;

	for(unsigned int i = 0; i<symbols.size(); i++) {
		new(_symbol_table+i) TokenSymbol(symbols[i]);
		_symbols_index.insert(std::make_pair(_symbol_table[i].string(), i));
	}
}

const TokenSymbol& Syntax::get_symbol_from_name(const char* symbol_name) const {
	auto it = _symbols_index.find(symbol_name);
	if(it == std::end(_symbols_index)) {
		THROW(exception::ElementNotFound, symbol_name);
	}
	else
		return _symbol_table[it->second];
}

unsigned int Syntax::index_of_symbol(const TokenSymbol& symbol) const {
	auto it = _symbols_index.find(symbol.string());
	if(it == std::end(_symbols_index)) {
		THROW(exception::ElementNotFound, symbol.string());
	}
	else
		return it->second;
}

const TokenSymbol* Syntax::begin_terminal() {
	return _symbol_table;
}

const TokenSymbol* Syntax::end_terminal() {
	return _symbol_table+_symbol_non_terminal_start;
}

const TokenSymbol* Syntax::begin_non_terminal() {
	return _symbol_table+_symbol_non_terminal_start;
}

const TokenSymbol* Syntax::end_non_terminal() {
	return _symbol_table+_symbol_number;
}

//
//	Lexer
//

Syntax::State Syntax::next_lexer_state(State current_state, Letter current_letter) const {
	if(current_letter < Direct_letter_range)
		return _lexer_table[Direct_letter_range*current_state+current_letter];
	else
		return 0; //TODO
}

const TokenSymbol* Syntax::lexer_accepting_state(State current_state) const {
	return _lexer_accepting_state[current_state] == Lexer_unaccepting_state ? nullptr : &_symbol_table[_lexer_accepting_state[current_state]];
}

void Syntax::set_lexer_table(unsigned int state_number) {
	_lexer_table = new State[Direct_letter_range*state_number];
	_lexer_accepting_state = new Index[state_number];
	_lexer_state_number = state_number;
}

Syntax::State* Syntax::lexer_table() {
	return _lexer_table;
}

const Syntax::State* Syntax::lexer_table() const {
	return _lexer_table;
}

Syntax::Index* Syntax::lexer_accepting_state() {
	return _lexer_accepting_state;
}

const Syntax::Index* Syntax::lexer_accepting_state() const {
	return _lexer_accepting_state;
}

//
// Parser
//
void Syntax::set_parser_table(unsigned int state_number) {
	_action_table = new Index[state_number*(_symbol_number-_symbol_non_terminal_start)];
	_goto_table = new Index[state_number*_symbol_non_terminal_start];
	_parser_state_number = state_number;
}

Syntax::Index* Syntax::parser_action_table() {
	return _action_table;
}

const Syntax::Index* Syntax::parser_action_table() const {
	return _action_table;
}

Syntax::Index* Syntax::parser_goto_table() {
	return _goto_table;
}

const Syntax::Index* Syntax::parser_goto_table() const {
	return _goto_table;
}
