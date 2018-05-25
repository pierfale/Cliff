#include "cliff/tool/syntax_generator/EBNFRepresentation.h"

using namespace cliff;


EBNFOperation::EBNFOperation(EBNFOperationType type) : _type(type), _content("") {

}

EBNFOperation::EBNFOperation(EBNFOperationType type, const char* content): _type(type), _content(content) {

}

void EBNFOperation::print(std::ostream& stream) const {
	switch (_type) {
	case EBNF_ROOT:
		for(size_t i=0; i<_children.size(); i++) {
			_children.at(i).print(stream);
		}
		break;
	case EBNF_ALTERNATIVE:
		for(size_t i=0; i<_children.size(); i++) {
			if(i > 0) {
				stream << " | ";
			}
			_children.at(i).print(stream);
		}

		break;
	case EBNF_SEQUENCE:
		for(size_t i=0; i<_children.size(); i++) {
			if(i > 0) {
				stream << " ";
			}
			_children.at(i).print(stream);
		}

		break;
	case EBNF_REPETITION:
		stream << "(";
		_children.at(0).print(stream);
		stream << ")*";
		break;
	case EBNF_OPTIONAL:
		stream << "(";
		_children.at(0).print(stream);
		stream << ")?";
		break;
	case EBNF_TERMINAL:
		stream << "\"";
		for(size_t i=0; _content[i] != '\0'; i++) {
			char c = _content[i];
			switch (c) {
			case '\n':
				stream << "\\n";
				break;
			case '\\':
				stream << "\\";
				break;
			case '\r':
				stream << "\\r";
				break;
			case '\t':
				stream << "\\t";
				break;
			case '\"':
				stream << "\\\"";
				break;
			default:
				stream << c;
				break;
			}
		}
		stream << "\"";
		break;
	case EBNF_NON_TERMINAL:

		if(_children.empty()) {
			stream << _content;
		}
		else {

			for(size_t i=0; i<_children.size(); i++) {
				if(i > 0) {
					stream << ".";
				}
				stream << _children.at(i)._content;
			}

		}

		break;
	default:
		break;
	}
}


EBNFScope::EBNFScope(EBNFScope* parent_scope, const std::string& name) : _parent_scope(parent_scope), _scope_name(name), _lexer_omit_token(EBNF_ROOT) {

}

void EBNFScope::print(std::ostream& stream) const {
	_print(stream, 0);
}


void EBNFScope::_print(std::ostream& stream, size_t ltab) const {
	std::string left_str(ltab, '\t');
	//stream << left_str << "SCOPE " << _scope_name << ":" << std::endl;
	stream << left_str << "OPTION lexer omit token: ";
	_lexer_omit_token.print(stream);
	stream << std::endl;

	for(const auto& rule : _rules) {
		stream << left_str << rule.first << ": ";
		rule.second.print(stream);
		stream << std::endl;
	}

	for(const auto& rule : _regular_expressions) {
		stream << left_str << rule.first << ": ";
		rule.second.print(stream);
		stream << std::endl;
	}

	for(const auto& scope : _children_scope) {
		stream << left_str << "SCOPE " << scope.first << " {" << std::endl;
		scope.second._print(stream, ltab+1);
		stream << left_str << "}" << std::endl;
	}
}
