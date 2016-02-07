#include "cliff/tool/syntax_generator/SyntaxGenerator.h"

using namespace cliff;

void SyntaxGenerator::initialize() {
	EntryBoundRegister::bind_module_to_option(this, "gs", "generate-syntax", Module::Order::PreCompiler);
}

void SyntaxGenerator::execute(ProgramOption::Iterator option_caller) {

	std::string error_message_cmd("Option -gs (--generate-syntax) require syntax filename : -gs [filename]");
	const char* filename = option_caller.next_argument_or_else(error_message_cmd).require_text_argument_or_else(error_message_cmd);

	std::cout << "filename = " << filename << std::endl;

	Syntax syntax;
	syntax.load("syntax.bin");
	Reader reader(filename);
	Lexer lexer(syntax, reader);
	Parser parser(syntax, lexer);


	MemoryContainer<AbstractSyntaxTree> ast_memory;
	AbstractSyntaxTree& root = parser.execute(ast_memory);

	std::cout << "Abstract Syntax Tree : " << std::endl;
	root.print(std::cout);
	std::cout << std::endl;

	//
	//	TMP
	//

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

	Token letter_1(token_symbol_regular_expression_letter, "a");
	Token letter_2(token_symbol_regular_expression_letter, "b");
	Token letter_3(token_symbol_regular_expression_letter, "c");
	Token letter_4(token_symbol_regular_expression_letter, "d");
	Token letter_5(token_symbol_regular_expression_letter, "e");
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

	// global ignore = " " "\n"
	/*AbstractSyntaxTree& global_node = rule_list_node.add_child(token_global_statement);
	global_node.add_child(Token(token_keyword_global));
	global_node.add_child(Token(token_symbol_rule_terminal, " "));
	global_node.add_child(Token(token_symbol_rule_terminal, "\n"));*/

	// local ignore = none
	// regular_expression := "#" regular_expression_block* "#"
	AbstractSyntaxTree& rule_1_node = rule_list_node.add_child(token_symbol_rule);
	rule_1_node.add_child(Token(token_symbol_rule_name, "regular_expression"));
	rule_1_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_1_definition_alternative = rule_1_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_1_1 = token_symbol_rule_1_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_1_1.add_child(Token(token_symbol_rule_terminal, "#"));
	AbstractSyntaxTree& token_symbol_rule_1_1_1 = token_symbol_rule_1_1.add_child(Token(token_symbol_rule_repetition));
	token_symbol_rule_1_1_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_block"));
	token_symbol_rule_1_1.add_child(Token(token_symbol_rule_terminal, "#"));


	// regular_expression_block := regular_expression_letter [regular_expression_quantifier] | regular_expression_range [regular_expression_quantifier]
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

/*
	// regular_expression_letter := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" | "a" | "b" | "c" | "d" | "e" | "." ...
	AbstractSyntaxTree& rule_4_node = rule_list_node.add_child(token_symbol_rule);
	rule_4_node.add_child(Token(token_symbol_rule_name, "regular_expression_letter"));
	rule_4_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_4_definition_alternative = rule_4_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_4_1 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_1.add_child(Token(token_symbol_rule_terminal, "a"));
	AbstractSyntaxTree& token_symbol_rule_4_2 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_2.add_child(Token(token_symbol_rule_terminal, "b"));
	AbstractSyntaxTree& token_symbol_rule_4_3 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_3.add_child(Token(token_symbol_rule_terminal, "c"));
	AbstractSyntaxTree& token_symbol_rule_4_4 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_4.add_child(Token(token_symbol_rule_terminal, "d"));
	AbstractSyntaxTree& token_symbol_rule_4_5 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_5.add_child(Token(token_symbol_rule_terminal, "e"));
	AbstractSyntaxTree& token_symbol_rule_4_6 = token_symbol_rule_4_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_4_6.add_child(Token(token_symbol_rule_terminal, "."));
*/
	// regular_expression_range := "[" regular_expression_range_block* "]"
	AbstractSyntaxTree& rule_5_node = rule_list_node.add_child(token_symbol_rule);
	rule_5_node.add_child(Token(token_symbol_rule_name, "regular_expression_range"));
	rule_5_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_5_definition_alternative = rule_5_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_5_1 = token_symbol_rule_5_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_5_1.add_child(Token(token_symbol_rule_terminal, "["));
	AbstractSyntaxTree& token_symbol_rule_5_1_1 = token_symbol_rule_5_1.add_child(Token(token_symbol_rule_repetition));
	token_symbol_rule_5_1_1.add_child(Token(token_symbol_rule_non_terminal, "regular_expression_range_block"));
	token_symbol_rule_5_1.add_child(Token(token_symbol_rule_terminal, "]"));

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

	// regular_expression_letter := #[^\[\]]#
	AbstractSyntaxTree& rule_9_node = rule_list_node.add_child(token_symbol_rule);
	rule_9_node.add_child(Token(token_symbol_rule_name, "regular_expression_letter"));
	rule_9_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& rule_9_re = rule_9_node.add_child(token_symbol_regular_expression);

	AbstractSyntaxTree& rule_9_re_1 = rule_9_re.add_child(token_symbol_regular_expression_range);
	rule_9_re_1.add_child(Token(token_symbol_unamed_terminal, "[^"));
	AbstractSyntaxTree& rule_9_re_1_1 = rule_9_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_9_re_1_1.add_child(Token(token_symbol_rule_terminal, "["));
	AbstractSyntaxTree& rule_9_re_1_2 = rule_9_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_9_re_1_2.add_child(Token(token_symbol_rule_terminal, "]"));
	AbstractSyntaxTree& rule_9_re_1_3 = rule_9_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_9_re_1_3.add_child(Token(token_symbol_rule_terminal, "#"));
	AbstractSyntaxTree& rule_9_re_1_4 = rule_9_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_9_re_1_4.add_child(Token(token_symbol_rule_terminal, "+"));
	AbstractSyntaxTree& rule_9_re_1_5 = rule_9_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_9_re_1_5.add_child(Token(token_symbol_rule_terminal, "*"));
	AbstractSyntaxTree& rule_9_re_1_6 = rule_9_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_9_re_1_6.add_child(Token(token_symbol_rule_terminal, "?"));
	AbstractSyntaxTree& rule_9_re_1_7 = rule_9_re_1.add_child(token_symbol_regular_expression_range_block);
	rule_9_re_1_7.add_child(Token(token_symbol_rule_terminal, "-")); // wut ?
	rule_9_re_1.add_child(Token(token_symbol_unamed_terminal, "]"));


/*
	rule_9_re.add_child(Token(token_symbol_regular_expression_letter, "a"));
	AbstractSyntaxTree& rule_9_re_1 = rule_9_re.add_child(token_symbol_regular_expression_repetition);
	AbstractSyntaxTree& rule_9_re_1_1 = rule_9_re_1.add_child(token_symbol_regular_expression_alternative);
	rule_9_re_1_1.add_child(Token(token_symbol_regular_expression_letter, "b"));
	rule_9_re_1_1.add_child(Token(token_symbol_regular_expression_letter, "c"));*/
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

	std::sort(std::begin(symbols_name), std::end(symbols_name), [](std::pair<const char*, bool> symbol_1, std::pair<const char*, bool> symbol_2) {
		return symbol_1.second != symbol_2.second ? symbol_1.second: std::strcmp(symbol_1.first, symbol_2.first) <= 0; });

	symbols_name.erase(std::unique(std::begin(symbols_name), std::end(symbols_name), [](std::pair<const char*, bool> symbol_1, std::pair<const char*, bool> symbol_2) {
		return symbol_1.second == symbol_2.second && std::strcmp(symbol_1.first, symbol_2.first) == 0; }), std::end(symbols_name));

	std::vector<const char*> ordered_symbols_name;
	ordered_symbols_name.resize(symbols_name.size());
	std::transform(std::begin(symbols_name), std::end(symbols_name), std::begin(ordered_symbols_name), [](std::pair<const char*, bool> symbol) {return symbol.first;});


	unsigned int terminal_number = std::count_if(std::begin(symbols_name), std::end(symbols_name),  [](std::pair<const char*, bool> symbol) -> bool {return symbol.second;});
	generated_syntax.set_symbol_table(ordered_symbols_name, terminal_number);

	SyntaxRepresentation syntax_representation(ebnf_syntax, generated_syntax, ast);
	syntax_representation.print(std::cout);



	std::cout << "===========" << std::endl;
	std::cout << "   Lexer   " << std::endl;
	std::cout << "===========" << std::endl;
	std::map<const TokenSymbol*, std::vector<unsigned int>> accepting_states;
	LexerGenerator::generate_lexer(ebnf_syntax, ast, generated_syntax, syntax_representation, accepting_states);

	std::cout << "===========" << std::endl;
	std::cout << "   Parser  " << std::endl;
	std::cout << "===========" << std::endl;
	ParserGenerator::generate_parser(ebnf_syntax, generated_syntax, syntax_representation, accepting_states);

	generated_syntax.save("syntax.bin");

/*
	Graphe root_node;

	std::vector<TokenSymbol>& token_symbol_table = ebnf_syntax.token_symbol_list();
	for(auto it_token_symbol = std::begin(token_symbol_table); it != std::begin(token_symbol_table); ++it) {
		if(root_node.has_edge_label((*it_token_symbol)[0])) {

		}
		else {
			root_node.add_mode_with_edge_label((*it_token_symbol)[0]);
		}
	}*/

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
