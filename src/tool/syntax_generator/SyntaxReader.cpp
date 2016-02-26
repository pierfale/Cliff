#include "cliff/tool/syntax_generator/SyntaxReader.h"

using namespace cliff;

AbstractSyntaxTree& SyntaxReader::execute(const char* filename, Syntax& syntax, MemoryContainer<AbstractSyntaxTree>& output_container) {
	std::cout << "filename = " << filename << std::endl;

	syntax.load("syntax.bin");
	Reader reader(filename);
	Lexer lexer(syntax, reader);
	Parser parser(syntax, lexer);


	MemoryContainer<AbstractSyntaxTree> ast_memory;
	AbstractSyntaxTree& syntax_tree_root = parser.execute(ast_memory);

	TreeTransformer ebnf_tree_transformer;
/*
	ebnf_tree_transformer.add_rule([](const AbstractSyntaxTree& current_node) {
		TreeComparator comparator;
		return comparator.match_with(current_node).of_type("rule")
				.has_child_of_type_at_position("rule_name", 0)
				.in_child(2)
					.of_type("regular_expression")
				.end_child() == true;
	}, TreeTransformer::Ascend);
*/
	AbstractSyntaxTree* root = ebnf_tree_transformer.execute(syntax_tree_root, output_container);

	if(root == nullptr)
		THROW(exception::Exception, "SyntaxReader : root node null");

	return *root;
}
