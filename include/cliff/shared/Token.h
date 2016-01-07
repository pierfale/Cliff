#ifndef _CLIFF_TOKEN_H
#define _CLIFF_TOKEN_H

#include <cstring>

#include <iostream>

namespace cliff {

	class TokenSymbol {

	public:
		TokenSymbol(const char* owner_string);
		TokenSymbol(const char* owner_string, unsigned int size);
		TokenSymbol(const TokenSymbol& that) = delete;
		TokenSymbol(TokenSymbol&& that);
		~TokenSymbol();

		const char* string() const;
		unsigned int size() const;

		bool operator==(const TokenSymbol& that) const;
		bool operator!=(const TokenSymbol& that) const;
	private:
		char* _string;
		unsigned int _size;
	};

	class Token {

	public:
		Token(const TokenSymbol& type);
		Token(const TokenSymbol& type, const char* owner_content);
		Token(const Token& that);
		~Token();

		const TokenSymbol& type() const;
		const char* content() const;

	private:
		const TokenSymbol& _type;
		char* _content;

	};
}

#endif
