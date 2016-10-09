#include "cliff/shared/Token.h"

using namespace cliff;

//
//	TokenSymbol
//

TokenSymbol::TokenSymbol(const char* str) : CopyHashString(str) {

}

TokenSymbol::TokenSymbol(TokenSymbol&& that) : CopyHashString(std::move(that)) {

}

//
//	Token
//

Token::Token() : _type(nullptr), _content(nullptr) {

}

Token::Token(const TokenSymbol& type) : Token(type, nullptr) {

}

Token::Token(const TokenSymbol& type, const char* owner_content) : _type(&type), _content(nullptr) {
	if(owner_content != nullptr) {
		unsigned int size = std::strlen(owner_content);
		_content = new char[size+1];
		memcpy(_content, owner_content, size+1);
	}
}

Token::Token(const Token& that) : _type(that._type), _content(nullptr) {
	if(that._content != nullptr) {
		unsigned int size = std::strlen(that._content);
		_content = new char[size+1];
		memcpy(_content, that._content, size+1);
	}
}

Token::~Token() {
	delete[] _content;
}

void Token::set(const TokenSymbol& type, const char* content) {
	_type = &type;
	delete[] _content;
	if(content != nullptr) {
		unsigned int size = std::strlen(content);
		_content = new char[size+1];
		memcpy(_content, content, size+1);
	}
	else
		_content = nullptr;
}

const TokenSymbol& Token::type() const {
	return *_type;
}

const char* Token::content() const {
	return _content;
}
