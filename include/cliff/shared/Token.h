#ifndef _CLIFF_TOKEN_H
#define _CLIFF_TOKEN_H

#include <cstring>

#include <iostream>

#include "cliff/shared/HashString.h"

namespace cliff {

	class TokenSymbol : public StaticHashString {

	public:
		TokenSymbol(const char* str);
		TokenSymbol(const TokenSymbol& that) = delete;
		TokenSymbol(TokenSymbol&& that);
	};

	class Token {

	public:
		Token();
		Token(const TokenSymbol& type);
		Token(const TokenSymbol& type, const char* content);
		Token(const Token& that);
		~Token();

		void set(const TokenSymbol& type, const char* content = nullptr);

		const TokenSymbol& type() const;
		const char* content() const;

	private:
		const TokenSymbol* _type;
		char* _content;

	};
}

#endif
