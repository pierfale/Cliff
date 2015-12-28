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
	/*syntax.load("syntax.bin");

	Lexer lexer(syntax);
	Parser parser(syntax);

	std::ifstream file(filename, std::ios::in);

	if(!file.is_open()) {
		THROW(exception::FileNotFound, filename);
	}

	std::stringstream char_stream;
	char_stream << file.rdbuf();

	std::vector<Token> token_stream;
	lexer.execute(char_stream, token_stream);

	MemoryContainer<AbstractSyntaxTree> ast_memory;
	AbstractSyntaxTree& root = parser.execute(token_stream, ast_memory);
*/
	//
	//	TMP
	//

	std::vector<const char*> symbols_name = {	"unamed_terminal", "rule_list", "rule", "rule_name", "rule_definition_alternative", "rule_definition",
												"rule_terminal", "rule_non_terminal",

												//Regular expression
												"regular_expression", "regular_expression_alternative", "regular_expression_repetition",
												"regular_expression_letter_list", "regular_expression_letter"};

	syntax.set_symbol_table(symbols_name, 0);

	const TokenSymbol& token_symbol_unamed_terminal = syntax.get_symbol_from_name("unamed_terminal");
	const TokenSymbol& token_symbol_rule_list = syntax.get_symbol_from_name("rule_list");
	const TokenSymbol& token_symbol_rule = syntax.get_symbol_from_name("rule");
	const TokenSymbol& token_symbol_rule_name = syntax.get_symbol_from_name("rule_name");
	const TokenSymbol& token_symbol_rule_definition_alternative = syntax.get_symbol_from_name("rule_definition_alternative");
	const TokenSymbol& token_symbol_rule_definition = syntax.get_symbol_from_name("rule_definition");
	const TokenSymbol& token_symbol_rule_terminal = syntax.get_symbol_from_name("rule_terminal");
	const TokenSymbol& token_symbol_rule_non_terminal = syntax.get_symbol_from_name("rule_non_terminal");

	const TokenSymbol& token_symbol_regular_expression = syntax.get_symbol_from_name("regular_expression");
	const TokenSymbol& token_symbol_regular_expression_letter_list = syntax.get_symbol_from_name("regular_expression_letter_list");
	const TokenSymbol& token_symbol_regular_expression_letter = syntax.get_symbol_from_name("regular_expression_letter");
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

	AbstractSyntaxTree rule_list_node(tree_memory,token_symbol_rule_list);

	AbstractSyntaxTree& rule_2_node = rule_list_node.add_child(token_symbol_rule);
	rule_2_node.add_child(Token(token_symbol_rule_name, "list"));
	rule_2_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_2_definition_alternative = rule_2_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_2_1 = token_symbol_rule_2_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_2_1.add_child(Token(token_symbol_rule_non_terminal, "list"));
	token_symbol_rule_2_1.add_child(Token(token_symbol_rule_non_terminal, "pair"));
	AbstractSyntaxTree& token_symbol_rule_2_2 = token_symbol_rule_2_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_2_2.add_child(Token(token_symbol_rule_non_terminal, "pair"));

	AbstractSyntaxTree& rule_3_node = rule_list_node.add_child(token_symbol_rule);
	rule_3_node.add_child(Token(token_symbol_rule_name, "pair"));
	rule_3_node.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& token_symbol_rule_3_definition_alternative = rule_3_node.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& token_symbol_rule_3_1 = token_symbol_rule_3_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_terminal, "("));
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_non_terminal, "pair"));
	token_symbol_rule_3_1.add_child(Token(token_symbol_rule_terminal, ")"));
	AbstractSyntaxTree& token_symbol_rule_3_2 = token_symbol_rule_3_definition_alternative.add_child(token_symbol_rule_definition);
	token_symbol_rule_3_2.add_child(Token(token_symbol_rule_terminal, "("));
	token_symbol_rule_3_2.add_child(Token(token_symbol_rule_terminal, ")"));
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

	std::sort(std::begin(symbols_name), std::end(symbols_name), [](std::pair<const char*, bool> symbol_1, std::pair<const char*, bool> symbol_2) {
		return symbol_1.second != symbol_2.second ? symbol_1.second: std::strcmp(symbol_1.first, symbol_2.first) <= 0; });
	symbols_name.erase(std::unique(std::begin(symbols_name), std::end(symbols_name), [](std::pair<const char*, bool> symbol_1, std::pair<const char*, bool> symbol_2) {
		return symbol_1.second == symbol_2.second && std::strcmp(symbol_1.first, symbol_2.first) == 0; }));

	std::vector<const char*> ordered_symbols_name;
	ordered_symbols_name.resize(symbols_name.size());
	std::transform(std::begin(symbols_name), std::end(symbols_name), std::begin(ordered_symbols_name), [](std::pair<const char*, bool> symbol) {return symbol.first;});

	unsigned int terminal_number = std::count_if(std::begin(symbols_name), std::end(symbols_name),  [](std::pair<const char*, bool> symbol) -> bool {return symbol.second;});
	generated_syntax.set_symbol_table(ordered_symbols_name, terminal_number);

	std::cout << "===========" << std::endl;
	std::cout << "   Lexer   " << std::endl;
	std::cout << "===========" << std::endl;
	LexerGenerator::generate_lexer(ebnf_syntax, ast, generated_syntax);

	std::cout << "===========" << std::endl;
	std::cout << "   Parser  " << std::endl;
	std::cout << "===========" << std::endl;
	ParserGenerator::generate_parser(ebnf_syntax, ast, generated_syntax);

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
	if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule_name")) {
		symbols_name.push_back(std::make_pair(syntax_tree.content(), false));
	}
	else if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule_terminal")) {
		symbols_name.push_back(std::make_pair(syntax_tree.content(), true));
	}
	else if(syntax_tree.type() == ebnf_syntax.get_symbol_from_name("rule")) {
		get_symbols_name(ebnf_syntax, *syntax_tree.children()[0], symbols_name);
		get_symbols_name(ebnf_syntax, *syntax_tree.children()[2], symbols_name);
	}
	else {
		for(const AbstractSyntaxTree* child : syntax_tree.children())
			get_symbols_name(ebnf_syntax, *child, symbols_name);
	}
}
