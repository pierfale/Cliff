#ifndef _CLIFF_ABSTRACT_SYNTAX_TREE_H
#define _CLIFF_ABSTRACT_SYNTAX_TREE_H

#include <vector>
#include <ostream>
#include <cassert>
#include "cliff/shared/Token.h"
#include "cliff/shared/MemoryContainer.h"

#include <iostream>

namespace cliff {
class AbstractSyntaxTree {

public:
	AbstractSyntaxTree(MemoryContainer<AbstractSyntaxTree>& memory_container);
	AbstractSyntaxTree(MemoryContainer<AbstractSyntaxTree>& memory_container, const TokenSymbol& symbol);
	AbstractSyntaxTree(MemoryContainer<AbstractSyntaxTree>& memory_container, const Token& token);
	~AbstractSyntaxTree();

	AbstractSyntaxTree& add_child(const TokenSymbol& symbol);
	AbstractSyntaxTree& add_child(const Token& token);
	AbstractSyntaxTree& add_child(AbstractSyntaxTree* child);

	void set(const TokenSymbol& type, const char* content = nullptr);

	bool is_leaf() const;
	const TokenSymbol& type() const;
	const char* content() const;
	Token& token();
	const Token& token() const;

	const std::vector<AbstractSyntaxTree*>& children() const;
	std::vector<AbstractSyntaxTree*>& children();

	void print(std::ostream& stream) const;

private:
	void _print(std::ostream& stream, unsigned int depth) const;

	MemoryContainer<AbstractSyntaxTree>& _memory_container;
	Token _token;
	std::vector<AbstractSyntaxTree*> _children;
};
}

#endif
