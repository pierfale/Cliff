#ifndef _CLIFF_SYNTAX_H
#define _CLIFF_SYNTAX_H

#include <map>
#include <vector>
#include <fstream>

#include "cliff/shared/Token.h"
#include "cliff/shared/Exception.h"

namespace cliff {

	class Syntax {

	public:
		typedef uint32_t State;
		typedef uint32_t Index;
		typedef int Letter;

		static const char* EOF_symbol;
		static const char* Root_symbol;

		static const State Lexer_init_state = 0x0;
		static const State Lexer_state_error = 0xFFFFFFFF;
		static const Index Lexer_unaccepting_state = 0xFFFFFFFF;
		static const int Direct_letter_range = 128;

		static const State Parser_init_state;
		static const Index Parser_unaccepting_state = 0xFFFFFFFF;
		static const Index Parser_action_shift_mask = 0x40000000;
		static const Index Parser_action_reduce_mask = 0x20000000;
		static const Index Parser_action_accept_mask = 0x10000000;
		static const Index Parser_action_content_mask = 0xFFFFFF;

		Syntax();
		~Syntax();

		void load(const char* filename);
		void save(const char* filename);

		//
		// Symbol
		//
		void set_symbol_table(std::vector<const char*> symbols, unsigned int terminal_range);
		const TokenSymbol& get_symbol_from_name(const char* symbol_name) const;
		const TokenSymbol& get_symbol_from_name_or_else(const char* symbol_name, const exception::UserMessage& message) const;

		unsigned int index_of_symbol(const TokenSymbol& symbol) const;

		const TokenSymbol* begin_terminal();
		const TokenSymbol* end_terminal();
		const TokenSymbol* begin_non_terminal();
		const TokenSymbol* end_non_terminal();

		//
		// Lexer
		//
		State next_lexer_state(State current_state, Letter current_letter) const;
		const TokenSymbol* lexer_accepting_state(State current_state) const;

		void set_lexer_table(unsigned int state_number);
		State* lexer_table();
		const State* lexer_table() const;
		Index* lexer_accepting_state();
		const Index* lexer_accepting_state() const;

		//
		// Parser
		//
		Index next_parser_action(State current_state, const TokenSymbol& current_symbol) const;
		State next_parser_goto(State current_state, const TokenSymbol& current_symbol) const;
		const TokenSymbol& parser_reduce_symbol(State current_state, const TokenSymbol& current_symbol) const;
		unsigned int parser_reduce_number(State current_state, const TokenSymbol& current_symbol) const;

		void set_parser_table(unsigned int state_number);
		Index* parser_action_table();
		const Index* parser_action_table() const;
		Index* parser_goto_table();
		const Index* parser_goto_table() const;

		Index* parser_reduce_number();
		const Index* parser_reduce_number() const;

	private:
		//
		//	Symbol
		//
		struct StrCompare : public std::binary_function<const char*, const char*, bool> {
		public:
			bool operator() (const char* str1, const char* str2) const
			{ return std::strcmp(str1, str2) < 0; }
		};
		std::map<const char*, unsigned int, StrCompare> _symbols_index;

		unsigned int _symbol_number;
		unsigned int _symbol_non_terminal_start;
		TokenSymbol* _symbol_table;

		//
		//	Lexer
		//
		unsigned int _lexer_state_number;
		State* _lexer_table;
		Index* _lexer_accepting_state;

		//
		//	Parser
		//
		unsigned int _parser_state_number;
		Index* _action_table;
		Index* _reduce_number;

		Index* _goto_table;
		std::tuple<const TokenSymbol*, unsigned int> _reduce_table;


	};
}
#endif
