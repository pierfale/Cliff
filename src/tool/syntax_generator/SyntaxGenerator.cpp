#include "cliff/tool/syntax_generator/SyntaxGenerator.h"

using namespace cliff;

void SyntaxGenerator::initialize() {
	EntryBoundRegister::bind_module_to_option(this, "gs", "generate-syntax", Module::Order::PreCompiler);
}

void SyntaxGenerator::execute(ProgramOption::Iterator option_caller) {

	std::string error_message_cmd("Option -gs (--generate-syntax) require syntax filename : -gs [filename]");
	const char* filename = option_caller.next_argument_or_else(error_message_cmd).require_text_argument_or_else(error_message_cmd);

	Syntax syntax;
	MemoryContainer<AbstractSyntaxTree> abstract_syntax_tree_container;
	AbstractSyntaxTree& abstract_syntax_tree_root = SyntaxReader::execute(filename, syntax, abstract_syntax_tree_container);

	std::cout << "Abstract Syntax Tree : " << std::endl;
	abstract_syntax_tree_root.print(std::cout);
	std::cout << std::endl;

	//create_syntax(syntax, abstract_syntax_tree_root);

	//
	//	TMP
	//

	syntax._symbols_index.clear();

	std::vector<const char*> symbols_name = {	"token_global_statement", "token_keyword_global",

												"unamed_terminal", "rule_list", "rule", "rule_name", "rule_alternative", "rule_sequence", "rule_optional", "rule_repetition",
												"rule_terminal", "rule_non_terminal",

												//Regular expression
												"regular_expression", "regular_expression_alternative", "regular_expression_repetition",
												"regular_expression_letter_list", "regular_expression_letter", "regular_expression_letter_all",
												"regular_expression_range", "regular_expression_range_block"};



	syntax.set_symbol_table(symbols_name, 0);

	const TokenSymbol& token_global_statement = syntax.get_symbol_from_name("token_global_statement");
	const TokenSymbol& token_keyword_global = syntax.get_symbol_from_name("token_keyword_global");

	const TokenSymbol& token_symbol_unamed_terminal = syntax.get_symbol_from_name("unamed_terminal");
	const TokenSymbol& token_symbol_rule_list = syntax.get_symbol_from_name("rule_list");
	const TokenSymbol& token_symbol_rule = syntax.get_symbol_from_name("rule");
	const TokenSymbol& token_symbol_rule_name = syntax.get_symbol_from_name("rule_name");
	const TokenSymbol& token_symbol_rule_definition_alternative = syntax.get_symbol_from_name("rule_alternative");
	const TokenSymbol& token_symbol_rule_optional = syntax.get_symbol_from_name("rule_optional");
	const TokenSymbol& token_symbol_rule_repetition = syntax.get_symbol_from_name("rule_repetition");
	const TokenSymbol& token_symbol_rule_definition = syntax.get_symbol_from_name("rule_sequence");
	const TokenSymbol& token_symbol_rule_terminal = syntax.get_symbol_from_name("rule_terminal");
	const TokenSymbol& token_symbol_rule_non_terminal = syntax.get_symbol_from_name("rule_non_terminal");

	const TokenSymbol& token_symbol_regular_expression = syntax.get_symbol_from_name("regular_expression");
	const TokenSymbol& token_symbol_regular_expression_letter_list = syntax.get_symbol_from_name("regular_expression_letter_list");
	const TokenSymbol& token_symbol_regular_expression_range = syntax.get_symbol_from_name("regular_expression_range");
	const TokenSymbol& token_symbol_regular_expression_range_block = syntax.get_symbol_from_name("regular_expression_range_block");
	const TokenSymbol& token_symbol_regular_expression_letter = syntax.get_symbol_from_name("regular_expression_letter");
	const TokenSymbol& token_symbol_regular_expression_letter_all = syntax.get_symbol_from_name("regular_expression_letter_all");
	const TokenSymbol& token_symbol_regular_expression_alternative = syntax.get_symbol_from_name("regular_expression_alternative");
	const TokenSymbol& token_symbol_regular_expression_repetition = syntax.get_symbol_from_name("regular_expression_repetition");
/*
	class A {

	public:
		A() {

		}

		~A() {

		}

		int _[32];
	};

	MemoryContainer<A> tree_memory;

	for(int i=0; i<80; i++) {
		std::cout << "->" << &tree_memory.emplace() << std::endl;
	}
*/

	MemoryContainer<AbstractSyntaxTree> tree_memory;
/*
	AbstractSyntaxTree rule_list_node(tree_memory,token_symbol_rule_list);

	// statement_list := statement | statement statement_list
	AbstractSyntaxTree& rule_1_node = rule_list_node.add_child(token_symbol_rule);
	rule_1_node.add_child(Token(token_symbol_rule_name, "statement_list"));
	rule_1_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_1_definition_alternative = rule_1_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_1_1 = token_symbol_rule_1_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_1_1.add_child(Token(token_symbol_rule_non_terminal, "statement"));
	AbstractSyntaxTree& token_symbol_rule_1_2 = token_symbol_rule_1_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_1_2.add_child(Token(token_symbol_rule_non_terminal, "statement"));
	token_symbol_rule_1_2.add_child(Token(token_symbol_rule_non_terminal, "statement_list"));

	// statement := syntax_rule ";"
	AbstractSyntaxTree& rule_2_node = rule_list_node.add_child(token_symbol_rule);
	rule_2_node.add_child(Token(token_symbol_rule_name, "statement"));
	rule_2_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_2_definition_alternative = rule_2_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_2_1 = token_symbol_rule_2_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_2_1.add_child(Token(token_symbol_rule_non_terminal, "syntax_rule"));
	token_symbol_rule_2_1.add_child(Token(token_symbol_rule_terminal, ";"));

	// syntax_rule := identifier ":=" syntax_rule_alternative_list
	AbstractSyntaxTree& rule_3_node = rule_list_node.add_child(token_symbol_rule);
	rule_3_node.add_child(Token(token_symbol_rule_name, "syntax_rule"));
	rule_3_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_3_definition_alternative = rule_3_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_3_1 = token_symbol_rule_3_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_terminal, ":="));
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_non_terminal, "syntax_rule_alternative_list"));

	// syntax_rule_alternative_list := syntax_rule_alternative | syntax_rule_alternative "|" syntax_rule_alternative_list
	AbstractSyntaxTree& rule_4_node = rule_list_node.add_child(token_symbol_rule);
	rule_4_node.add_child(Token(token_symbol_rule_name, "syntax_rule_alternative_list"));
	rule_4_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_4_definition_alternative = rule_4_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_4_1 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_1.add_child(Token(token_symbol_rule_non_terminal, "syntax_rule_alternative"));
	AbstractSyntaxTree& token_symbol_rule_4_2 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_2.add_child(Token(token_symbol_rule_non_terminal, "syntax_rule_alternative"));
	token_symbol_rule_4_2.add_child(Token(token_symbol_rule_terminal, "|"));
	token_symbol_rule_4_2.add_child(Token(token_symbol_rule_non_terminal, "syntax_rule_alternative_list"));

	// syntax_rule_alternative := identifier | identifier syntax_rule_alternative
	AbstractSyntaxTree& rule_5_node = rule_list_node.add_child(token_symbol_rule);
	rule_5_node.add_child(Token(token_symbol_rule_name, "syntax_rule_alternative"));
	rule_5_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_5_definition_alternative = rule_5_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_5_1 = token_symbol_rule_5_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_5_1.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	AbstractSyntaxTree& token_symbol_rule_5_2 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_5_2.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	token_symbol_rule_5_2.add_child(Token(token_symbol_rule_non_terminal, "syntax_rule_alternative"));

	// identifier := #[a-zA-Z_][a-zA-Z0-9_]*#
	AbstractSyntaxTree& rule_6_node = rule_list_node.add_child(token_symbol_rule);
	rule_6_node.add_child(Token(token_symbol_rule_name, "identifier"));
	rule_6_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_node = rule_6_node.add_child(token_symbol_regular_expression);
	AbstractSyntaxTree& rule_6_block_1 = regular_expression_node.add_child(token_symbol_regular_expression_block);
	AbstractSyntaxTree& rule_6_block_1 = regular_expression_node.add_child(token_symbol_regular_expression_block);

	letter_list_1.add_child(letter_1);
	AbstractSyntaxTree& repetition_node = regular_expression_node.add_child(token_symbol_regular_expression_repetition);
	AbstractSyntaxTree& alternative_node = repetition_node.add_child(token_symbol_regular_expression_alternative);
	AbstractSyntaxTree& letter_list_2 = alternative_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_2.add_child(letter_2);
	AbstractSyntaxTree& letter_list_3 = alternative_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_3.add_child(letter_3);
*/
	//
	// Regular Expression
	//

	AbstractSyntaxTree rule_list_node(tree_memory,token_symbol_rule_list);

	// grammar := (rule ";")*
	AbstractSyntaxTree& rule_bnf_node_1 = rule_list_node.add_child(token_symbol_rule);
	rule_bnf_node_1.add_child(Token(token_symbol_rule_name, "grammar"));
	rule_bnf_node_1.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_bnf_node_1_1 = rule_bnf_node_1.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& rule_bnf_node_1_1_1 = rule_bnf_node_1_1.add_child(token_symbol_rule_definition);
	AbstractSyntaxTree& rule_bnf_node_1_1_1_1 = rule_bnf_node_1_1_1.add_child(token_symbol_rule_repetition);
	rule_bnf_node_1_1_1_1.add_child(Token(token_symbol_rule_non_terminal, "rule"));
	rule_bnf_node_1_1_1_1.add_child(Token(token_symbol_rule_terminal, ";"));

	// rule := identifier ":=" regular_expression | identifier ":=" rule_alternative
	AbstractSyntaxTree& rule_bnf_node_2 = rule_list_node.add_child(token_symbol_rule);
	rule_bnf_node_2.add_child(Token(token_symbol_rule_name, "rule"));
	rule_bnf_node_2.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_bnf_node_2_1 = rule_bnf_node_2.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& rule_bnf_node_2_1_1 = rule_bnf_node_2_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_2_1_1.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	rule_bnf_node_2_1_1.add_child(Token(token_symbol_rule_terminal, ":="));
	rule_bnf_node_2_1_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression"));


	AbstractSyntaxTree& rule_bnf_node_2_1_2 = rule_bnf_node_2_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_2_1_2.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	rule_bnf_node_2_1_2.add_child(Token(token_symbol_rule_terminal, ":="));
	rule_bnf_node_2_1_2.add_child(Token(token_symbol_rule_non_terminal, "rule_alternative"));
	//rule_bnf_node_2_1_1.add_child(Token(token_symbol_rule_terminal, ";"));

	// rule_alternative := rule_sequence | rule_alternative "|" rule_sequence
	AbstractSyntaxTree& rule_bnf_node_3 = rule_list_node.add_child(token_symbol_rule);
	rule_bnf_node_3.add_child(Token(token_symbol_rule_name, "rule_alternative"));
	rule_bnf_node_3.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_bnf_node_3_1 = rule_bnf_node_3.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& rule_bnf_node_3_1_1 = rule_bnf_node_3_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_3_1_1.add_child(Token(token_symbol_rule_non_terminal, "rule_sequence"));

	AbstractSyntaxTree& rule_bnf_node_3_1_2 = rule_bnf_node_3_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_3_1_2.add_child(Token(token_symbol_rule_non_terminal, "rule_alternative"));
	rule_bnf_node_3_1_2.add_child(Token(token_symbol_rule_terminal, "|"));
	rule_bnf_node_3_1_2.add_child(Token(token_symbol_rule_non_terminal, "rule_sequence"));

	// rule_sequence := rule_quantifier*
	AbstractSyntaxTree& rule_bnf_node_4 = rule_list_node.add_child(token_symbol_rule);
	rule_bnf_node_4.add_child(Token(token_symbol_rule_name, "rule_sequence"));
	rule_bnf_node_4.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_bnf_node_4_1 = rule_bnf_node_4.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& rule_bnf_node_4_1_1 = rule_bnf_node_4_1.add_child(token_symbol_rule_definition);
	AbstractSyntaxTree& rule_bnf_node_4_1_1_1 = rule_bnf_node_4_1_1.add_child(token_symbol_rule_repetition);
	rule_bnf_node_4_1_1_1.add_child(Token(token_symbol_rule_non_terminal, "rule_quantifier"));

	// rule_quantifier := rule_unit | rule_unit "*" | rule_unit "+" | rule_unit "?"
	AbstractSyntaxTree& rule_bnf_node_6 = rule_list_node.add_child(token_symbol_rule);
	rule_bnf_node_6.add_child(Token(token_symbol_rule_name, "rule_quantifier"));
	rule_bnf_node_6.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_bnf_node_6_1 = rule_bnf_node_6.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& rule_bnf_node_6_1_1 = rule_bnf_node_6_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_6_1_1.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));

	AbstractSyntaxTree& rule_bnf_node_6_1_2 = rule_bnf_node_6_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_6_1_2.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));
	rule_bnf_node_6_1_2.add_child(Token(token_symbol_rule_terminal, "*"));

	AbstractSyntaxTree& rule_bnf_node_6_1_3 = rule_bnf_node_6_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_6_1_3.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));
	rule_bnf_node_6_1_3.add_child(Token(token_symbol_rule_terminal, "+"));

	AbstractSyntaxTree& rule_bnf_node_6_1_4 = rule_bnf_node_6_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_6_1_4.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));
	rule_bnf_node_6_1_4.add_child(Token(token_symbol_rule_terminal, "?"));

	// rule_unit := string_literal | identifier | "(" rule_alternative ")"
	AbstractSyntaxTree& rule_bnf_node_5 = rule_list_node.add_child(token_symbol_rule);
	rule_bnf_node_5.add_child(Token(token_symbol_rule_name, "rule_unit"));
	rule_bnf_node_5.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_bnf_node_5_1 = rule_bnf_node_5.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& rule_bnf_node_5_1_1 = rule_bnf_node_5_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_5_1_1.add_child(Token(token_symbol_rule_non_terminal, "string_literal"));

	AbstractSyntaxTree& rule_bnf_node_5_1_2 = rule_bnf_node_5_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_5_1_2.add_child(Token(token_symbol_rule_non_terminal, "identifier"));

	AbstractSyntaxTree& rule_bnf_node_5_1_3 = rule_bnf_node_5_1.add_child(token_symbol_rule_definition);
	rule_bnf_node_5_1_3.add_child(Token(token_symbol_rule_terminal, "("));
	rule_bnf_node_5_1_3.add_child(Token(token_symbol_rule_non_terminal, "rule_alternative"));
	rule_bnf_node_5_1_3.add_child(Token(token_symbol_rule_terminal, ")"));

	// regular_expression := "#" regular_expression_alternative "#"
	AbstractSyntaxTree& rule_1_node = rule_list_node.add_child(token_symbol_rule);
	rule_1_node.add_child(Token(token_symbol_rule_name, "regular_expression"));
	rule_1_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_1_definition_alternative = rule_1_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_1_1 = token_symbol_rule_1_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_1_1.add_child(Token(token_symbol_rule_terminal, "#"));
	token_symbol_rule_1_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_alternative"));
	token_symbol_rule_1_1.add_child(Token(token_symbol_rule_terminal, "#"));


	// regular_expression_alternative :=  regular_expression_block_list | regular_expression_alternative "|" regular_expression_block_list
	AbstractSyntaxTree& rule_100_node = rule_list_node.add_child(token_symbol_rule);
	rule_100_node.add_child(Token(token_symbol_rule_name, "regular_expression_alternative"));
	rule_100_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_100_definition_alternative = rule_100_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_100_1 = token_symbol_rule_100_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_100_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_block_list"));

	AbstractSyntaxTree& token_symbol_rule_100_2 = token_symbol_rule_100_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_100_2.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_alternative"));
	token_symbol_rule_100_2.add_child(Token(token_symbol_rule_terminal, "|"));
	token_symbol_rule_100_2.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_block_list"));


	// regular_expression_block_list := regular_expression_block+
	AbstractSyntaxTree& rule_101_node = rule_list_node.add_child(token_symbol_rule);
	rule_101_node.add_child(Token(token_symbol_rule_name, "regular_expression_block_list"));
	rule_101_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_101_definition_alternative = rule_101_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_101_1 = token_symbol_rule_101_definition_alternative.add_child(token_symbol_rule_definition);
	AbstractSyntaxTree& token_symbol_rule_101_1_1 = token_symbol_rule_101_1.add_child(Token(token_symbol_rule_repetition));
	token_symbol_rule_101_1_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_block"));


	// regular_expression_block := regular_expression_letter regular_expression_quantifier? | regular_expression_range regular_expression_quantifier? | "(" regular_expression_alternative ")"
	AbstractSyntaxTree& rule_3_node = rule_list_node.add_child(token_symbol_rule);
	rule_3_node.add_child(Token(token_symbol_rule_name, "regular_expression_block"));
	rule_3_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_3_definition_alternative = rule_3_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_3_1 = token_symbol_rule_3_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_letter"));
	AbstractSyntaxTree& token_symbol_rule_3_1_ = token_symbol_rule_3_1.add_child(token_symbol_rule_optional);
	token_symbol_rule_3_1_.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_quantifier"));

	AbstractSyntaxTree& token_symbol_rule_3_2 = token_symbol_rule_3_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_3_2.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_range"));
	AbstractSyntaxTree& token_symbol_rule_3_2_ = token_symbol_rule_3_2.add_child(token_symbol_rule_optional);
	token_symbol_rule_3_2_.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_quantifier"));

	AbstractSyntaxTree& token_symbol_rule_3_3 = token_symbol_rule_3_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_3_3.add_child(Token(token_symbol_rule_terminal, "("));
	token_symbol_rule_3_3.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_alternative"));
	token_symbol_rule_3_3.add_child(Token(token_symbol_rule_terminal, ")"));

	// regular_expression_range := "[" regular_expression_range_block* "]" | "[^" regular_expression_range_block* "]";
	AbstractSyntaxTree& rule_5_node = rule_list_node.add_child(token_symbol_rule);
	rule_5_node.add_child(Token(token_symbol_rule_name, "regular_expression_range"));
	rule_5_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_5_definition_alternative = rule_5_node.add_child(token_symbol_rule_definition_alternative);

	AbstractSyntaxTree& token_symbol_rule_5_1 = token_symbol_rule_5_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_5_1.add_child(Token(token_symbol_rule_terminal, "["));
	AbstractSyntaxTree& token_symbol_rule_5_1_1 = token_symbol_rule_5_1.add_child(Token(token_symbol_rule_repetition));
	token_symbol_rule_5_1_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_range_block"));
	token_symbol_rule_5_1.add_child(Token(token_symbol_rule_terminal, "]"));

	AbstractSyntaxTree& token_symbol_rule_5_2 = token_symbol_rule_5_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_5_2.add_child(Token(token_symbol_rule_terminal, "[^"));
	AbstractSyntaxTree& token_symbol_rule_5_2_1 = token_symbol_rule_5_2.add_child(Token(token_symbol_rule_repetition));
	token_symbol_rule_5_2_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_range_block"));
	token_symbol_rule_5_2.add_child(Token(token_symbol_rule_terminal, "]"));

	// regular_expression_range_block := regular_expression_letter | regular_expression_letter "-" regular_expression_letter
	AbstractSyntaxTree& rule_7_node = rule_list_node.add_child(token_symbol_rule);
	rule_7_node.add_child(Token(token_symbol_rule_name, "regular_expression_range_block"));
	rule_7_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_7_definition_alternative = rule_7_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_7_1 = token_symbol_rule_7_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_7_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_letter"));
	AbstractSyntaxTree& token_symbol_rule_7_2 = token_symbol_rule_7_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_7_2.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_letter"));
	token_symbol_rule_7_2.add_child(Token(token_symbol_rule_terminal, "-"));
	token_symbol_rule_7_2.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_letter"));

	// regular_expression_quantifier := "+" | "*" | "?" | "{" digit "," digit "}"
	AbstractSyntaxTree& rule_8_node = rule_list_node.add_child(token_symbol_rule);
	rule_8_node.add_child(Token(token_symbol_rule_name, "regular_expression_quantifier"));
	rule_8_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_8_definition_alternative = rule_8_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_8_1 = token_symbol_rule_8_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_8_1.add_child(Token(token_symbol_rule_terminal, "+"));
	AbstractSyntaxTree& token_symbol_rule_8_2 = token_symbol_rule_8_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_8_2.add_child(Token(token_symbol_rule_terminal, "*"));
	AbstractSyntaxTree& token_symbol_rule_8_3 = token_symbol_rule_8_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_8_3.add_child(Token(token_symbol_rule_terminal, "?"));

	// regular_expression_letter := regular_expression_letter_authorized | regular_expression_letter_protected
	AbstractSyntaxTree& rule_9_node = rule_list_node.add_child(token_symbol_rule);
	rule_9_node.add_child(Token(token_symbol_rule_name, "regular_expression_letter"));
	rule_9_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_9_definition_alternative = rule_9_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_9_1 = token_symbol_rule_9_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_9_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_letter_authorized"));
	AbstractSyntaxTree& token_symbol_rule_9_2 = token_symbol_rule_9_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_9_2.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_letter_protected"));

	// regular_expression_letter_authorized := #[^\[\]\#\+\*\?\-\|]#
	AbstractSyntaxTree& rule_91_node = rule_list_node.add_child(token_symbol_rule);
	rule_91_node.add_child(Token(token_symbol_rule_name, "regular_expression_letter_authorized"));
	rule_91_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_91_re = rule_91_node.add_child(token_symbol_regular_expression);

	AbstractSyntaxTree& rule_91_re_1 = rule_91_re.add_child(token_symbol_regular_expression_range);
	rule_91_re_1.add_child(Token(token_symbol_unamed_terminal, "[^"));
	AbstractSyntaxTree& rule_91_re_1_1 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_1.add_child(Token(token_symbol_rule_terminal, "["));
	AbstractSyntaxTree& rule_91_re_1_2 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_2.add_child(Token(token_symbol_rule_terminal, "]"));
	AbstractSyntaxTree& rule_91_re_1_3 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_3.add_child(Token(token_symbol_rule_terminal, "#"));
	AbstractSyntaxTree& rule_91_re_1_4 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_4.add_child(Token(token_symbol_rule_terminal, "+"));
	AbstractSyntaxTree& rule_91_re_1_5 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_5.add_child(Token(token_symbol_rule_terminal, "*"));
	AbstractSyntaxTree& rule_91_re_1_6 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_6.add_child(Token(token_symbol_rule_terminal, "?"));
	AbstractSyntaxTree& rule_91_re_1_7 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_7.add_child(Token(token_symbol_rule_terminal, "-"));
	AbstractSyntaxTree& rule_91_re_1_8 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_8.add_child(Token(token_symbol_rule_terminal, "("));
	AbstractSyntaxTree& rule_91_re_1_9 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_9.add_child(Token(token_symbol_rule_terminal, ")"));
	AbstractSyntaxTree& rule_91_re_1_10 = rule_91_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_91_re_1_10.add_child(Token(token_symbol_rule_terminal, "|"));
	rule_91_re_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	//regular_expression_letter_protected := "\[" | "\]" | "\#" | "\+" | "\*" | "\?" | "\-" | "\|"
	AbstractSyntaxTree& rule_92_node = rule_list_node.add_child(token_symbol_rule);
	rule_92_node.add_child(Token(token_symbol_rule_name, "regular_expression_letter_protected"));
	rule_92_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_92_definition_alternative = rule_92_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_92_1 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_1.add_child(Token(token_symbol_rule_terminal, "\\["));
	AbstractSyntaxTree& token_symbol_rule_92_2 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_2.add_child(Token(token_symbol_rule_terminal, "\\]"));
	AbstractSyntaxTree& token_symbol_rule_92_3 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_3.add_child(Token(token_symbol_rule_terminal, "\\#"));
	AbstractSyntaxTree& token_symbol_rule_92_4 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_4.add_child(Token(token_symbol_rule_terminal, "\\+"));
	AbstractSyntaxTree& token_symbol_rule_92_5 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_5.add_child(Token(token_symbol_rule_terminal, "\\*"));
	AbstractSyntaxTree& token_symbol_rule_92_6 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_6.add_child(Token(token_symbol_rule_terminal, "\\?"));
	AbstractSyntaxTree& token_symbol_rule_92_7 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_7.add_child(Token(token_symbol_rule_terminal, "\\-"));
	AbstractSyntaxTree& token_symbol_rule_92_8 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_8.add_child(Token(token_symbol_rule_terminal, "\\("));
	AbstractSyntaxTree& token_symbol_rule_92_9 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_9.add_child(Token(token_symbol_rule_terminal, "\\)"));
	AbstractSyntaxTree& token_symbol_rule_92_10 = token_symbol_rule_92_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_92_10.add_child(Token(token_symbol_rule_terminal, "\\|"));

	// whitespace := # |\t|\n#
	AbstractSyntaxTree& rule_10_node = rule_list_node.add_child(token_symbol_rule);
	rule_10_node.add_child(Token(token_symbol_rule_name, "whitespace"));
	rule_10_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_10_re = rule_10_node.add_child(token_symbol_regular_expression);
	AbstractSyntaxTree& rule_10_re_1 = rule_10_re.add_child(token_symbol_regular_expression_alternative);

	rule_10_re_1.add_child(Token(token_symbol_regular_expression_letter, " "));
	rule_10_re_1.add_child(Token(token_symbol_regular_expression_letter, "\n"));
	rule_10_re_1.add_child(Token(token_symbol_regular_expression_letter, "\t"));

	// all_char_expect_quote := #[^"]*#
	/*AbstractSyntaxTree& rule_11_node = rule_list_node.add_child(token_symbol_rule);
	rule_11_node.add_child(Token(token_symbol_rule_name, "all_char_expect_quote"));
	rule_11_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_11_re = rule_11_node.add_child(token_symbol_regular_expression);
	AbstractSyntaxTree& rule_11_re_1 = rule_11_re.add_child(token_symbol_regular_expression_repetition);
	AbstractSyntaxTree& rule_11_re_1_1 = rule_11_re_1.add_child(token_symbol_regular_expression_range);
	rule_11_re_1_1.add_child(Token(token_symbol_unamed_terminal, "[^"));
	AbstractSyntaxTree& rule_11_re_1_1_1 = rule_11_re_1_1.add_child(token_symbol_regular_expression_range_block);
	rule_11_re_1_1_1.add_child(Token(token_symbol_rule_terminal, "\""));
	rule_11_re_1_1.add_child(Token(token_symbol_unamed_terminal, "]"));*/

	// string_literal := "\"" string_literal_letter* "\""
	AbstractSyntaxTree& rule_12_node = rule_list_node.add_child(token_symbol_rule);
	rule_12_node.add_child(Token(token_symbol_rule_name, "string_literal"));
	rule_12_node.add_child(token_symbol_unamed_terminal);

	AbstractSyntaxTree& token_symbol_rule_12_definition_alternative = rule_12_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_12_1 = token_symbol_rule_12_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_12_1.add_child(Token(token_symbol_rule_terminal, "\""));
	AbstractSyntaxTree& token_symbol_rule_12_1_1 = token_symbol_rule_12_1.add_child(Token(token_symbol_rule_repetition));
	token_symbol_rule_12_1_1.add_child(Token(token_symbol_rule_non_terminal, "string_literal_letter"));
	token_symbol_rule_12_1.add_child(Token(token_symbol_rule_terminal, "\""));

	// string_literal_letter := string_literal_letter_authorized | string_literal_letter_protected
	AbstractSyntaxTree& rule_121_node = rule_list_node.add_child(token_symbol_rule);
	rule_121_node.add_child(Token(token_symbol_rule_name, "string_literal_letter"));
	rule_121_node.add_child(token_symbol_unamed_terminal);

	AbstractSyntaxTree& token_symbol_rule_121_definition_alternative = rule_121_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_121_1 = token_symbol_rule_121_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_121_1.add_child(Token(token_symbol_rule_non_terminal, "string_literal_letter_authorized"));
	AbstractSyntaxTree& token_symbol_rule_121_2 = token_symbol_rule_121_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_121_2.add_child(Token(token_symbol_rule_non_terminal, "string_literal_letter_protected"));

	//string_literal_letter_authorized := #[^"\]#
	AbstractSyntaxTree& rule_122_node = rule_list_node.add_child(token_symbol_rule);
	rule_122_node.add_child(Token(token_symbol_rule_name, "string_literal_letter_authorized"));
	rule_122_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_122_re = rule_122_node.add_child(token_symbol_regular_expression);
	AbstractSyntaxTree& rule_122_re_1 = rule_122_re.add_child(token_symbol_regular_expression_range);
	rule_122_re_1.add_child(Token(token_symbol_unamed_terminal, "[^"));
	AbstractSyntaxTree& rule_122_re_1_1 = rule_122_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_122_re_1_1.add_child(Token(token_symbol_rule_terminal, "\""));
	/*AbstractSyntaxTree& rule_122_re_1_2 = rule_122_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_122_re_1_2.add_child(Token(token_symbol_rule_terminal, "\\"));*/
	rule_122_re_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	//string_literal_letter_protected := "\\\\" | "\\\""
	AbstractSyntaxTree& rule_123_node = rule_list_node.add_child(token_symbol_rule);
	rule_123_node.add_child(Token(token_symbol_rule_name, "string_literal_letter_protected"));
	rule_123_node.add_child(token_symbol_unamed_terminal);

	AbstractSyntaxTree& token_symbol_rule_123_definition_alternative = rule_123_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_123_1 = token_symbol_rule_123_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_123_1.add_child(Token(token_symbol_rule_terminal, "\\\\"));
	AbstractSyntaxTree& token_symbol_rule_123_2 = token_symbol_rule_123_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_123_2.add_child(Token(token_symbol_rule_terminal, "\\\""));

	// identifier := #[a-zA-Z_][0-9a-zA-Z_]*#
	AbstractSyntaxTree& rule_13_node = rule_list_node.add_child(token_symbol_rule);
	rule_13_node.add_child(Token(token_symbol_rule_name, "identifier"));
	rule_13_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_13_re = rule_13_node.add_child(token_symbol_regular_expression);

	AbstractSyntaxTree& rule_13_re_1 = rule_13_re.add_child(token_symbol_regular_expression_range);
	rule_13_re_1.add_child(Token(token_symbol_unamed_terminal, "["));
	AbstractSyntaxTree& rule_13_re_1_1 = rule_13_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_13_re_1_1.add_child(Token(token_symbol_regular_expression_letter, "a"));
	rule_13_re_1_1.add_child(Token(token_symbol_rule_terminal, "-"));
	rule_13_re_1_1.add_child(Token(token_symbol_regular_expression_letter, "z"));
	AbstractSyntaxTree& rule_13_re_1_2 = rule_13_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_13_re_1_2.add_child(Token(token_symbol_regular_expression_letter, "A"));
	rule_13_re_1_2.add_child(Token(token_symbol_rule_terminal, "-"));
	rule_13_re_1_2.add_child(Token(token_symbol_regular_expression_letter, "Z"));
	AbstractSyntaxTree& rule_13_re_1_3 = rule_13_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_13_re_1_3.add_child(Token(token_symbol_regular_expression_letter, "_"));
	rule_13_re_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	AbstractSyntaxTree& rule_13_re_2 = rule_13_re.add_child(token_symbol_regular_expression_repetition);
	AbstractSyntaxTree& rule_13_re_2_1 = rule_13_re_2.add_child(token_symbol_regular_expression_range);
	rule_13_re_2_1.add_child(Token(token_symbol_unamed_terminal, "["));
	AbstractSyntaxTree& rule_13_re_2_1_1 = rule_13_re_2_1.add_child(token_symbol_regular_expression_range_block);
	rule_13_re_2_1_1.add_child(Token(token_symbol_regular_expression_letter, "a"));
	rule_13_re_2_1_1.add_child(Token(token_symbol_rule_terminal, "-"));
	rule_13_re_2_1_1.add_child(Token(token_symbol_regular_expression_letter, "z"));
	AbstractSyntaxTree& rule_13_re_2_1_2 = rule_13_re_2_1.add_child(token_symbol_regular_expression_range_block);
	rule_13_re_2_1_2.add_child(Token(token_symbol_regular_expression_letter, "A"));
	rule_13_re_2_1_2.add_child(Token(token_symbol_rule_terminal, "-"));
	rule_13_re_2_1_2.add_child(Token(token_symbol_regular_expression_letter, "Z"));
	AbstractSyntaxTree& rule_13_re_2_1_3 = rule_13_re_2_1.add_child(token_symbol_regular_expression_range_block);
	rule_13_re_2_1_3.add_child(Token(token_symbol_regular_expression_letter, "0"));
	rule_13_re_2_1_3.add_child(Token(token_symbol_rule_terminal, "-"));
	rule_13_re_2_1_3.add_child(Token(token_symbol_regular_expression_letter, "9"));
	AbstractSyntaxTree& rule_13_re_2_1_4 = rule_13_re_2_1.add_child(token_symbol_regular_expression_range_block);
	rule_13_re_2_1_4.add_child(Token(token_symbol_regular_expression_letter, "_"));
	rule_13_re_2_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	//
	//	Parenthesis
	//
/*
	AbstractSyntaxTree rule_list_node(tree_memory,token_symbol_rule_list);

	AbstractSyntaxTree& rule_2_node = rule_list_node.add_child(token_symbol_rule);
	rule_2_node.add_child(Token(token_symbol_rule_name, "list"));
	rule_2_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_2_definition_alternative = rule_2_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_2_1 = token_symbol_rule_2_definition_alternative.add_child(token_symbol_rule_definition);

	token_symbol_rule_2_1.add_child(Token(token_symbol_rule_terminal, "#"));
	AbstractSyntaxTree& token_symbol_rule_2_1_r = token_symbol_rule_2_1.add_child(token_symbol_rule_repetition);
	token_symbol_rule_2_1_r.add_child(Token(token_symbol_rule_non_terminal, "pair"));
	token_symbol_rule_2_1.add_child(Token(token_symbol_rule_terminal, "#"));

	AbstractSyntaxTree& rule_3_node = rule_list_node.add_child(token_symbol_rule);
	rule_3_node.add_child(Token(token_symbol_rule_name, "pair"));
	rule_3_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_3_definition_alternative = rule_3_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_3_1 = token_symbol_rule_3_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_terminal, "("));
	AbstractSyntaxTree& token_symbol_rule_3_1_1 = token_symbol_rule_3_1.add_child(token_symbol_rule_optional);
	token_symbol_rule_3_1_1.add_child(Token(token_symbol_rule_non_terminal, "list"));
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_terminal, ")"));
*/
/*
	AbstractSyntaxTree& rule_1_node = rule_list_node.add_child(token_symbol_rule);
	rule_1_node.add_child(Token(token_symbol_rule_name, "identifier"));
	rule_1_node.add_child(token_symbol_unamed_terminal);


	AbstractSyntaxTree& regular_expression_node = rule_1_node.add_child(token_symbol_regular_expression);
	AbstractSyntaxTree& letter_list_1 = regular_expression_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_1.add_child(letter_1);
	AbstractSyntaxTree& repetition_node = regular_expression_node.add_child(token_symbol_regular_expression_repetition);
	AbstractSyntaxTree& alternative_node = repetition_node.add_child(token_symbol_regular_expression_alternative);
	AbstractSyntaxTree& letter_list_2 = alternative_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_2.add_child(letter_2);
	AbstractSyntaxTree& letter_list_3 = alternative_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_3.add_child(letter_3);
*/

/*

	Token letter_f(token_symbol_regular_expression_letter, "f");
	Token letter_e(token_symbol_regular_expression_letter, "e");
	Token letter_i(token_symbol_regular_expression_letter, "i");

	AbstractSyntaxTree root_node(tree_memory, token_symbol_regular_expression);
	AbstractSyntaxTree& letter_list_1 = root_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_1.add_child(letter_f);
	AbstractSyntaxTree& alternative_node = root_node.add_child(token_symbol_regular_expression_alternative);
	AbstractSyntaxTree& letter_list_2 = alternative_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_2.add_child(letter_e);
	letter_list_2.add_child(letter_e);
	AbstractSyntaxTree& letter_list_3 = alternative_node.add_child(token_symbol_regular_expression_letter_list);
	letter_list_3.add_child(letter_i);
	letter_list_3.add_child(letter_e);
*/
	std::cout << "Abstract Syntax Tree : " << std::endl;
	rule_list_node.print(std::cout);
	std::cout << std::endl;

	create_syntax(syntax, rule_list_node);
}

void SyntaxGenerator::create_syntax(const Syntax& ebnf_syntax, const AbstractSyntaxTree& ast) {

	//
	//	Syntax File Structure
	//
	//	MAGIC IDENTIFIER (4 bytes) : Always Value 0x2342
	//	VERSION (2 bytes) : Version of syntax file
	//	FLAGS (2 bytes) : ENDIANESS | 0x00
	//	TOKEN SYMBOL TABLE
	//		NUMBER OF SYMBOL (4 bytes)
	//			SYNBOL LENGTH (1 bytes) : n
	//			SYNBOL CHAR SEQUENCE (n bytes)

	Syntax generated_syntax;

	std::vector<std::pair<const char*, bool>> symbols_name;
	get_symbols_name(ebnf_syntax, ast, symbols_name);

	symbols_name.push_back(std::make_pair(Syntax::EOF_symbol, true));
	symbols_name.push_back(std::make_pair(Syntax::Root_symbol, false));
/*
	std::sort(std::begin(symbols_name), std::end(symbols_name), [](std::pair<const char*, bool> symbol_1, std::pair<const char*, bool> symbol_2) {
		return symbol_1.second != symbol_2.second ? symbol_1.second: std::strcmp(symbol_1.first, symbol_2.first) <= 0; });

	symbols_name.erase(std::unique(std::begin(symbols_name), std::end(symbols_name), [](std::pair<const char*, bool> symbol_1, std::pair<const char*, bool> symbol_2) {
		return symbol_1.second == symbol_2.second && std::strcmp(symbol_1.first, symbol_2.first) == 0; }), std::end(symbols_name));

	std::vector<const char*> ordered_symbols_name;
	ordered_symbols_name.resize(symbols_name.size());
	std::transform(std::begin(symbols_name), std::end(symbols_name), std::begin(ordered_symbols_name), [](std::pair<const char*, bool> symbol) {return symbol.first;});


	unsigned int terminal_number = std::count_if(std::begin(symbols_name), std::end(symbols_name),  [](std::pair<const char*, bool> symbol) -> bool {return symbol.second;});
	generated_syntax.set_symbol_table(ordered_symbols_name, terminal_number);
*/
	generated_syntax.set_symbol_list(symbols_name);

	SyntaxRepresentation syntax_representation(ebnf_syntax, generated_syntax, ast);
	syntax_representation.print(std::cout);



	std::cout << "===========" << std::endl;
	std::cout << "   Lexer   " << std::endl;
	std::cout << "===========" << std::endl;
	std::vector<const DeterministeFiniteAutomataNode*> lexer_state_list;
	MemoryContainer<DeterministeFiniteAutomataNode> dfa_memory;
	DeterministeFiniteAutomataNode dfa_start_node(dfa_memory);
	LexerGenerator::generate_lexer(ebnf_syntax, ast, generated_syntax, syntax_representation, lexer_state_list, dfa_start_node);

	std::cout << "===========" << std::endl;
	std::cout << "   Parser  " << std::endl;
	std::cout << "===========" << std::endl;
	ParserGenerator::generate_parser(ebnf_syntax, generated_syntax, syntax_representation, lexer_state_list);

	generated_syntax.save("syntax.bin");

}

void SyntaxGenerator::get_symbols_name(const Syntax& ebnf_syntax, const AbstractSyntaxTree& syntax_tree, std::vector<std::pair<const char*, bool>>& symbols_name) {
	if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule_terminal")) {
		symbols_name.push_back(std::make_pair(syntax_tree.content(), true));
	}
	else if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule")) {
		if(syntax_tree.children()[2]->type() == ebnf_syntax.get_symbol_from_name("regular_expression")) {
			symbols_name.push_back(std::make_pair(syntax_tree.children()[0]->content(), true));
		}
		else {
			symbols_name.push_back(std::make_pair(syntax_tree.children()[0]->content(), false));
			get_symbols_name(ebnf_syntax, *syntax_tree.children()[2], symbols_name);
		}
	}
	else {
		for(const AbstractSyntaxTree* child : syntax_tree.children())
			get_symbols_name(ebnf_syntax, *child, symbols_name);
	}
}
