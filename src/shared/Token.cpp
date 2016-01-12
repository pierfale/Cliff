#include "cliff/shared/Token.h"

using namespace cliff;

//
//	TokenSymbol
//

TokenSymbol::TokenSymbol(const char* owner_string) : _string(nullptr), _size(std::strlen(owner_string)) {
	_string = new char[_size+1];
	std::memcpy(_string, owner_string, _size+1);
}

TokenSymbol::TokenSymbol(const char* owner_string, unsigned int size) : _string(new char[size+1]), _size(size)  {
	std::memcpy(_string, owner_string, _size+1);
}

TokenSymbol::TokenSymbol(TokenSymbol&& that) : _string(that._string), _size(that._size) {
	that._string = nullptr;
	that._size = 0;
}

TokenSymbol::~TokenSymbol() {
	delete[] _string;
}


const char* TokenSymbol::string() const {
	return _string;
}

unsigned int TokenSymbol::size() const {
return _size;
}

bool TokenSymbol::operator==(const TokenSymbol& that) const {
	return this == &that;
}

bool TokenSymbol::operator!=(const TokenSymbol& that) const {
	return !operator==(that);
}

bool TokenSymbol::operator<(const TokenSymbol& that) const {
	return this < &that;
}
//
//	Token
//

Token::Token(const TokenSymbol& type) : _type(type), _content(nullptr) {

}

Token::Token(const TokenSymbol& type, const char* owner_content) : _type(type), _content(nullptr) {
	unsigned int size = std::strlen(owner_content);
	_content = new char[size+1];
	memcpy(_content, owner_content, size+1);
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

const TokenSymbol& Token::type() const {
	return _type;
}

const char* Token::content() const {
	return _content;
}
