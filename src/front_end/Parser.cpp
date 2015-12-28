#include "cliff/front_end/Parser.h"

using namespace cliff;

Parser::Parser(const Syntax& syntax) : _syntax(syntax) {

}

AbstractSyntaxTree& Parser::execute(const std::vector<Token>& input, MemoryContainer<AbstractSyntaxTree>& tree_memory) {

}
