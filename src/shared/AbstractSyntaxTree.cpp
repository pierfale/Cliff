#include "cliff/shared/AbstractSyntaxTree.h"

using namespace cliff;

AbstractSyntaxTree::AbstractSyntaxTree(MemoryContainer<AbstractSyntaxTree>& memory_container, const TokenSymbol& symbol) : _memory_container(memory_container), _token(symbol) {

}

AbstractSyntaxTree::AbstractSyntaxTree(MemoryContainer<AbstractSyntaxTree>& memory_container, const Token& token) : _memory_container(memory_container), _token(token) {

}

AbstractSyntaxTree::~AbstractSyntaxTree() {

}

AbstractSyntaxTree& AbstractSyntaxTree::add_child(const TokenSymbol& symbol) {
	AbstractSyntaxTree& child = _memory_container.emplace<MemoryContainer<AbstractSyntaxTree>&, const TokenSymbol&>(_memory_container, symbol);
	_children.push_back(&child);
	return child;
}

AbstractSyntaxTree& AbstractSyntaxTree::add_child(const Token& token) {
	AbstractSyntaxTree& child =  _memory_container.emplace<MemoryContainer<AbstractSyntaxTree>&, const Token&>(_memory_container, token);
	_children.push_back(&child);
	return child;
}

bool AbstractSyntaxTree::is_leaf() const {
	return _children.size() == 0;
}

const TokenSymbol& AbstractSyntaxTree::type() const {
	return _token.type();
}

const char* AbstractSyntaxTree::content() const {
	return _token.content();
}

const std::vector<AbstractSyntaxTree*>& AbstractSyntaxTree::children() const {
	return _children;
}

void AbstractSyntaxTree::print(std::ostream& stream) {
	_print(stream, 0);
}

void AbstractSyntaxTree::_print(std::ostream& stream, unsigned int depth) {
	std::string s(depth, ' ');

	if(is_leaf()) {
		stream << s << _token.type().string();
		if(_token.content() != nullptr)
			stream << " : " << _token.content() << std::endl;
	}
	else {
		stream << s << _token.type().string() << std::endl;
		for(AbstractSyntaxTree* node : _children)
			node->_print(stream, depth+1);
	}
}
