#include "cliff/tool/syntax_generator/SyntaxReader.h"

using namespace cliff;

AbstractSyntaxTree& SyntaxReader::execute(const char* filename, Syntax& syntax, MemoryContainer<AbstractSyntaxTree>& output_container) {
	std::cout << "filename = " << filename << std::endl;

	Syntax input_syntax;
	input_syntax.load("syntax.bin");
	Reader reader(filename);
	Lexer lexer(input_syntax, reader);
	Parser parser(input_syntax, lexer);



	MemoryContainer<AbstractSyntaxTree> ast_memory;
	AbstractSyntaxTree& syntax_tree_root = parser.execute(ast_memory);
	TreeTransformer ebnf_tree_transformer;

	ebnf_tree_transformer.add_symbol("rule_name", false);
	ebnf_tree_transformer.add_symbol("rule_terminal", false);
	ebnf_tree_transformer.add_symbol("rule_non_terminal", false);


	/*Syntax transformed_syntax(syntax);
	syntax.add_symbol("rule_terminal");
	syntax.add_symbol("rule_non_terminal");*/


/*
	ebnf_tree_transformer.add_rule([](const AbstractSyntaxTree& current_node) {
		TreeComparator comparator;
		return comparator.match_with(current_node).of_type("string_literal");
	}, [](TreeTransformerContext& transformer,  AbstractSyntaxTree* current_node) -> AbstractSyntaxTree* {
		//transformer.recurse();

		std::string string_content;
		transformer.foreach_children(TreeTransformer::of_type("string_literal_letter"), [string_content&](TreeTransformerContext& transformer,  AbstractSyntaxTree* current_node) {
			string_content += transformer.map_children(TreeTransformer::all(), [](TreeTransformerContext& transformer,  AbstractSyntaxTree* current_node) {
				if(current_node->type() == syntax.get_symbol_from_name("string_literal_letter_authorized")) {
					return current_node->content();
				}
				else if(current_node->type() == syntax.get_symbol_from_name("string_literal_letter_protected")) {
					return current_node->content().substr(1);
				}
				else {
					return "";
				}

			});
		});


		parent_node.add_child(syntax.get_symbol_from_name("rule_terminal"), string_content);

	});

	ebnf_tree_transformer.add_rule(TreeTransformer::of_type("string_literal"), TreeTransformer::Action::ascend_child(1), true);

	ebnf_tree_transformer.add_rule(TreeTransformer::of_type("string_literal_letter"), TreeTransformer::Action::ascend_children, true);
	ebnf_tree_transformer.add_rule(TreeTransformer::of_type("string_literal_letter_authorized"), TreeTransformer::Action::ascend_children, true);
	ebnf_tree_transformer.add_rule(TreeTransformer::of_type("string_literal_letter_protected"), [](TreeTransformerContext& transformer,  AbstractSyntaxTree* current_node) {
		return 	current_node->transform()
	}, true);*/

	/*
	 * 3
	 */
	ebnf_tree_transformer.add_rule(TreePatternMatching::of_type("rule"), [](TreeTransformer::Context& context) {
		std::cout << "#" << context.node().token().type().string() << (context.node().token().content() != nullptr ? context.node().token().content() : "") << std::endl;

		context.child_at_position(0).replace_node("rule_name");
		/*
		transformer.remove_child(1);
		transformer.recurse_child(2);*/
	});

	return ebnf_tree_transformer.execute(input_syntax, syntax, syntax_tree_root, output_container);

/*
	ebnf_tree_transformer.add_rule(TreeTransformer::of_type("string_literal"), [](TreeTransformerContext& transformer,  AbstractSyntaxTree* current_node) {
		std::string content = transformer.fold_children<std::string>(TreeTransformer::of_type("string_literal_letter"), "")
				.add_rule(TreeTransformer::of_type("string_literal_letter_authorized"), [](TreeTransformerContext& transformer,  AbstractSyntaxTree* current_node){ return std::string(current_node->content());});
				.add_rule(TreeTransformer::of_type("string_literal_letter_authorized"), [](TreeTransformerContext& transformer,  AbstractSyntaxTree* current_node){ return std::string(current_node->content()).substr(1);});

		transformer.replace_node(syntax.get_symbol_from_name("rule_terminal"), content);
	});

*/

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
	/*AbstractSyntaxTree* root = ebnf_tree_transformer.execute(syntax_tree_root, output_container);

	if(root == nullptr)
		THROW(exception::Exception, "SyntaxReader : root node null");*/

	return syntax_tree_root;
	//return *root;
}
