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

	ebnf_tree_transformer.add_rule(TreePatternMatching::of_type("rule"), [](TreeTransformer::Context& context) {
		context.child_at_position(0).replace_node("rule_name");
		context.child_at_position(1).remove();
		//transformer.recurse_child(2);
	});

	ebnf_tree_transformer.add_rule(TreePatternMatching::of_type("string_literal_letter_authorized"), [](TreeTransformer::Context& context) {
		context.set("content", context.node().token().content());
	});

	ebnf_tree_transformer.add_rule(TreePatternMatching::of_type("string_literal_letter_protected"), [](TreeTransformer::Context& context) {
		context.set("content", context.child_at_position(0).node().token().content()+1);
	});

	ebnf_tree_transformer.add_rule(TreePatternMatching::of_type("string_literal_letter"), [](TreeTransformer::Context& context) {
		context.set("content", context.child_at_position(0).get<const char>("content"));
	});

	ebnf_tree_transformer.add_rule(TreePatternMatching::of_type("string_literal"), [](TreeTransformer::Context& context) {
		std::vector<TreeTransformer::Context*> context_list;
		context.children_filter(TreePatternMatching::of_type("string_literal_letter"), context_list);
		context.replace_node("rule_terminal", std::accumulate(std::begin(context_list), std::end(context_list), std::string(), [](std::string content, TreeTransformer::Context* c) {
			const char* c2 = c->get<const char>("content");
			return c2 != nullptr ? content+c2 : content;
		}));
		context.remove_all_children();
	});

	return *ebnf_tree_transformer.execute(input_syntax, syntax, syntax_tree_root, output_container); // todo case of nullptr

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
