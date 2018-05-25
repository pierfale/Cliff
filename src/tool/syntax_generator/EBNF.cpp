#include "cliff/tool/syntax_generator/EBNF.h"

using namespace cliff;

EBNF::EBNF() {
	load();
}

void EBNF::load() {

	std::vector<const char*> symbols_name = {	"token_global_statement", "token_keyword_global",

												"statement", "identifier", "scope_identifier", "scope_statement", "scope_content", "option",

												"unamed_terminal", "rule_list", "rule", "rule_name", "rule_alternative", "rule_sequence", "rule_optional", "rule_repetition",
												"rule_terminal", "rule_non_terminal",

												//Regular expression
												"regular_expression", "regular_expression_alternative", "regular_expression_repetition",
												"regular_expression_letter_list", "regular_expression_letter", "regular_expression_letter_all",
												"regular_expression_range", "regular_expression_range_block"};



	_syntax.set_symbol_table(symbols_name, 0);

	//const TokenSymbol& token_global_statement = syntax.get_symbol_from_name("token_global_statement");
	//const TokenSymbol& token_keyword_global = syntax.get_symbol_from_name("token_keyword_global");

	const TokenSymbol& token_symbol_statement = _syntax.get_symbol_from_name("statement");
	const TokenSymbol& token_symbol_identifier = _syntax.get_symbol_from_name("identifier");
	const TokenSymbol& token_symbol_scope_identifier = _syntax.get_symbol_from_name("scope_identifier");
	const TokenSymbol& token_symbol_scope_statement = _syntax.get_symbol_from_name("scope_statement");
	const TokenSymbol& token_symbol_scope_content = _syntax.get_symbol_from_name("scope_content");
	const TokenSymbol& token_symbol_option = _syntax.get_symbol_from_name("option");

	const TokenSymbol& token_symbol_unamed_terminal = _syntax.get_symbol_from_name("unamed_terminal");
	const TokenSymbol& token_symbol_rule_list = _syntax.get_symbol_from_name("rule_list");
	const TokenSymbol& token_symbol_rule = _syntax.get_symbol_from_name("rule");
	const TokenSymbol& token_symbol_rule_name = _syntax.get_symbol_from_name("rule_name");
	const TokenSymbol& token_symbol_rule_definition_alternative = _syntax.get_symbol_from_name("rule_alternative");
	const TokenSymbol& token_symbol_rule_optional = _syntax.get_symbol_from_name("rule_optional");
	const TokenSymbol& token_symbol_rule_repetition = _syntax.get_symbol_from_name("rule_repetition");
	const TokenSymbol& token_symbol_rule_definition = _syntax.get_symbol_from_name("rule_sequence");
	const TokenSymbol& token_symbol_rule_terminal = _syntax.get_symbol_from_name("rule_terminal");
	const TokenSymbol& token_symbol_rule_non_terminal = _syntax.get_symbol_from_name("rule_non_terminal");

	const TokenSymbol& token_symbol_regular_expression = _syntax.get_symbol_from_name("regular_expression");
	const TokenSymbol& token_symbol_regular_expression_letter_list = _syntax.get_symbol_from_name("regular_expression_letter_list");
	const TokenSymbol& token_symbol_regular_expression_range = _syntax.get_symbol_from_name("regular_expression_range");
	const TokenSymbol& token_symbol_regular_expression_range_block = _syntax.get_symbol_from_name("regular_expression_range_block");
	const TokenSymbol& token_symbol_regular_expression_letter = _syntax.get_symbol_from_name("regular_expression_letter");
	const TokenSymbol& token_symbol_regular_expression_letter_all = _syntax.get_symbol_from_name("regular_expression_letter_all");
	const TokenSymbol& token_symbol_regular_expression_alternative = _syntax.get_symbol_from_name("regular_expression_alternative");
	const TokenSymbol& token_symbol_regular_expression_repetition = _syntax.get_symbol_from_name("regular_expression_repetition");

	MemoryContainer<AbstractSyntaxTree> tree_memory;
	AbstractSyntaxTree ast_root(tree_memory, token_symbol_rule_list);

	// option lexer_omit_token = " " | "\n" | "\r";
	AbstractSyntaxTree& statement_1 = ast_root.add_child(token_symbol_statement);
	statement_1.add_child(Token(token_symbol_option, "option"));
	statement_1.add_child(Token(token_symbol_identifier, "lexer_omit_token"));
	statement_1.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_1_alternative = statement_1.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_1_alternative_1 = statement_1_alternative.add_child(token_symbol_rule_definition);
	statement_1_alternative_1.add_child(Token(token_symbol_rule_terminal, " "));

	AbstractSyntaxTree& statement_1_alternative_2 = statement_1_alternative.add_child(token_symbol_rule_definition);
	statement_1_alternative_2.add_child(Token(token_symbol_rule_terminal, "\n"));

	AbstractSyntaxTree& statement_1_alternative_3 = statement_1_alternative.add_child(token_symbol_rule_definition);
	statement_1_alternative_3.add_child(Token(token_symbol_rule_terminal, "\r"));

	AbstractSyntaxTree& statement_1_alternative_4 = statement_1_alternative.add_child(token_symbol_rule_definition);
	statement_1_alternative_4.add_child(Token(token_symbol_rule_terminal, "\t"));
	statement_1.add_child(Token(token_symbol_unamed_terminal, ";"));

	//	root := scope_content;
	AbstractSyntaxTree& statement_2 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_2_rule = statement_2.add_child(token_symbol_rule);

	statement_2_rule.add_child(Token(token_symbol_rule_name, "root"));
	statement_2_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_2_alternative = statement_2_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_2_alternative_1 = statement_2_alternative.add_child(token_symbol_rule_definition);
	statement_2_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "scope_content"));

	statement_2.add_child(Token(token_symbol_unamed_terminal, ";"));

	//	scope_statement := "scope" identifier "{" scope_content "}";
	AbstractSyntaxTree& statement_3 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_3_rule = statement_3.add_child(token_symbol_rule);

	statement_3_rule.add_child(Token(token_symbol_rule_name, "scope_statement"));
	statement_3_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_3_alternative = statement_3_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_3_alternative_1 = statement_3_alternative.add_child(token_symbol_rule_definition);
	statement_3_alternative_1.add_child(Token(token_symbol_rule_terminal, "scope"));
	statement_3_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	statement_3_alternative_1.add_child(Token(token_symbol_rule_terminal, "{"));
	statement_3_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "scope_content"));
	statement_3_alternative_1.add_child(Token(token_symbol_rule_terminal, "}"));
	statement_3.add_child(Token(token_symbol_unamed_terminal, ";"));

	//	scope_content :=  statement*;
	AbstractSyntaxTree& statement_4 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_4_rule = statement_4.add_child(token_symbol_rule);

	statement_4_rule.add_child(Token(token_symbol_rule_name, "scope_content"));
	statement_4_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_4_alternative = statement_4_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_4_alternative_1 = statement_4_alternative.add_child(token_symbol_rule_definition);
	AbstractSyntaxTree& statement_4_alternative_1_1 = statement_4_alternative_1.add_child(token_symbol_rule_repetition);
	statement_4_alternative_1_1.add_child(Token(token_symbol_rule_non_terminal, "statement"));
	statement_4.add_child(Token(token_symbol_unamed_terminal, ";"));

	//	statement := rule ";" | "option" identifier "=" rule_alternative ";" | scope_statement;
	AbstractSyntaxTree& statement_5 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_5_rule = statement_5.add_child(token_symbol_rule);

	statement_5_rule.add_child(Token(token_symbol_rule_name, "statement"));
	statement_5_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_5_alternative = statement_5_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_5_alternative_1 = statement_5_alternative.add_child(token_symbol_rule_definition);
	statement_5_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "rule"));
	statement_5_alternative_1.add_child(Token(token_symbol_rule_terminal, ";"));

	AbstractSyntaxTree& statement_5_alternative_2 = statement_5_alternative.add_child(token_symbol_rule_definition);
	statement_5_alternative_2.add_child(Token(token_symbol_rule_terminal, "option"));
	statement_5_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	statement_5_alternative_2.add_child(Token(token_symbol_rule_terminal, "="));
	statement_5_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "rule_alternative"));
	statement_5_alternative_2.add_child(Token(token_symbol_rule_terminal, ";"));

	AbstractSyntaxTree& statement_5_alternative_3 = statement_5_alternative.add_child(token_symbol_rule_definition);
	statement_5_alternative_3.add_child(Token(token_symbol_rule_non_terminal, "scope_statement"));

	statement_5.add_child(Token(token_symbol_unamed_terminal, ";"));

	// rule := identifier ":=" regular_expression.root | identifier ":=" rule_alternative;
	AbstractSyntaxTree& statement_6 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_6_rule = statement_6.add_child(token_symbol_rule);

	statement_6_rule.add_child(Token(token_symbol_rule_name, "rule"));
	statement_6_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_6_alternative = statement_6_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_6_alternative_1 = statement_6_alternative.add_child(token_symbol_rule_definition);
	statement_6_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	statement_6_alternative_1.add_child(Token(token_symbol_rule_terminal, ":="));
	AbstractSyntaxTree& statement_6_alternative_1_identifier_1 = statement_6_alternative_1.add_child(token_symbol_scope_identifier);
	statement_6_alternative_1_identifier_1.add_child(Token(token_symbol_identifier, "regular_expression"));
	statement_6_alternative_1_identifier_1.add_child(Token(token_symbol_rule_terminal, "."));
	statement_6_alternative_1_identifier_1.add_child(Token(token_symbol_identifier, "root"));

	AbstractSyntaxTree& statement_6_alternative_2 = statement_6_alternative.add_child(token_symbol_rule_definition);
	statement_6_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "identifier"));
	statement_6_alternative_2.add_child(Token(token_symbol_rule_terminal, ":="));
	statement_6_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "rule_alternative"));
	statement_6_alternative_2.add_child(Token(token_symbol_rule_terminal, ";"));

	statement_6.add_child(Token(token_symbol_unamed_terminal, ";"));


	//	rule_alternative := rule_sequence |  rule_alternative "|" rule_sequence;
	AbstractSyntaxTree& statement_7 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_7_rule = statement_7.add_child(token_symbol_rule);

	statement_7_rule.add_child(Token(token_symbol_rule_name, "rule_alternative"));
	statement_7_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_7_alternative = statement_7_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_7_alternative_1 = statement_7_alternative.add_child(token_symbol_rule_definition);
	statement_7_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "rule_sequence"));

	AbstractSyntaxTree& statement_7_alternative_2 = statement_7_alternative.add_child(token_symbol_rule_definition);
	statement_7_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "rule_alternative"));
	statement_7_alternative_2.add_child(Token(token_symbol_rule_terminal, "|"));
	statement_7_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "rule_sequence"));

	statement_7.add_child(Token(token_symbol_unamed_terminal, ";"));

	//	rule_sequence := rule_quantifier*;
	AbstractSyntaxTree& statement_8 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_8_rule = statement_8.add_child(token_symbol_rule);

	statement_8_rule.add_child(Token(token_symbol_rule_name, "rule_sequence"));
	statement_8_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_8_alternative = statement_8_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_8_alternative_1 = statement_8_alternative.add_child(token_symbol_rule_definition);
	AbstractSyntaxTree& statement_8_alternative_1_1 = statement_8_alternative_1.add_child(token_symbol_rule_repetition);
	statement_8_alternative_1_1.add_child(Token(token_symbol_rule_non_terminal, "rule_quantifier"));
	statement_8.add_child(Token(token_symbol_unamed_terminal, ";"));

	//	rule_quantifier := rule_unit |  rule_unit "*" |  rule_unit "+" |  rule_unit "?";
	AbstractSyntaxTree& statement_9 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_9_rule = statement_9.add_child(token_symbol_rule);

	statement_9_rule.add_child(Token(token_symbol_rule_name, "rule_quantifier"));
	statement_9_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_9_alternative = statement_9_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_9_alternative_1 = statement_9_alternative.add_child(token_symbol_rule_definition);
	statement_9_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));

	AbstractSyntaxTree& statement_9_alternative_2 = statement_9_alternative.add_child(token_symbol_rule_definition);
	statement_9_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));
	statement_9_alternative_2.add_child(Token(token_symbol_rule_terminal, "*"));

	AbstractSyntaxTree& statement_9_alternative_3 = statement_9_alternative.add_child(token_symbol_rule_definition);
	statement_9_alternative_3.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));
	statement_9_alternative_3.add_child(Token(token_symbol_rule_terminal, "+"));

	AbstractSyntaxTree& statement_9_alternative_4 = statement_9_alternative.add_child(token_symbol_rule_definition);
	statement_9_alternative_4.add_child(Token(token_symbol_rule_non_terminal, "rule_unit"));
	statement_9_alternative_4.add_child(Token(token_symbol_rule_terminal, "?"));

	statement_9.add_child(Token(token_symbol_unamed_terminal, ";"));

	//	rule_unit := literal_string.root |  identifier |  "(" rule_alternative ")";
	AbstractSyntaxTree& statement_10 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_10_rule = statement_10.add_child(token_symbol_rule);

	statement_10_rule.add_child(Token(token_symbol_rule_name, "rule_unit"));
	statement_10_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_10_alternative = statement_10_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_10_alternative_1 = statement_10_alternative.add_child(token_symbol_rule_definition);
	AbstractSyntaxTree& statement_10_alternative_1_identifier_1 = statement_10_alternative_1.add_child(token_symbol_scope_identifier);
	statement_10_alternative_1_identifier_1.add_child(Token(token_symbol_identifier, "literal_string"));
	statement_10_alternative_1_identifier_1.add_child(Token(token_symbol_rule_terminal, "."));
	statement_10_alternative_1_identifier_1.add_child(Token(token_symbol_identifier, "root"));

	AbstractSyntaxTree& statement_10_alternative_2 = statement_10_alternative.add_child(token_symbol_rule_definition);
	statement_10_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "identifier"));

	AbstractSyntaxTree& statement_10_alternative_3 = statement_10_alternative.add_child(token_symbol_rule_definition);
	statement_10_alternative_3.add_child(Token(token_symbol_rule_terminal, "("));
	statement_10_alternative_3.add_child(Token(token_symbol_rule_non_terminal, "rule_alternative"));
	statement_10_alternative_3.add_child(Token(token_symbol_rule_terminal, ")"));

	statement_10.add_child(Token(token_symbol_unamed_terminal, ";"));

	// identifier := #[a-zA-Z_][0-9a-zA-Z_]*#;
	AbstractSyntaxTree& statement_11 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_11_rule = statement_11.add_child(token_symbol_rule);

	statement_11_rule.add_child(Token(token_symbol_rule_name, "identifier"));
	statement_11_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_11_re = statement_11_rule.add_child(token_symbol_regular_expression);

	AbstractSyntaxTree& statement_11_re_1 = statement_11_re.add_child(token_symbol_regular_expression_range);
	statement_11_re_1.add_child(Token(token_symbol_unamed_terminal, "["));
	AbstractSyntaxTree& statement_11_re_1_1 = statement_11_re_1.add_child(token_symbol_regular_expression_range_block);
	statement_11_re_1_1.add_child(Token(token_symbol_regular_expression_letter, "a"));
	statement_11_re_1_1.add_child(Token(token_symbol_rule_terminal, "-"));
	statement_11_re_1_1.add_child(Token(token_symbol_regular_expression_letter, "z"));
	AbstractSyntaxTree& statement_11_re_1_2 = statement_11_re_1.add_child(token_symbol_regular_expression_range_block);
	statement_11_re_1_2.add_child(Token(token_symbol_regular_expression_letter, "A"));
	statement_11_re_1_2.add_child(Token(token_symbol_rule_terminal, "-"));
	statement_11_re_1_2.add_child(Token(token_symbol_regular_expression_letter, "Z"));
	AbstractSyntaxTree& statement_11_re_1_3 = statement_11_re_1.add_child(token_symbol_regular_expression_range_block);
	statement_11_re_1_3.add_child(Token(token_symbol_regular_expression_letter, "_"));
	statement_11_re_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	AbstractSyntaxTree& statement_11_re_2 = statement_11_re.add_child(token_symbol_regular_expression_repetition);
	AbstractSyntaxTree& statement_11_re_2_1 = statement_11_re_2.add_child(token_symbol_regular_expression_range);
	statement_11_re_2_1.add_child(Token(token_symbol_unamed_terminal, "["));
	AbstractSyntaxTree& statement_11_re_2_1_1 = statement_11_re_2_1.add_child(token_symbol_regular_expression_range_block);
	statement_11_re_2_1_1.add_child(Token(token_symbol_regular_expression_letter, "a"));
	statement_11_re_2_1_1.add_child(Token(token_symbol_rule_terminal, "-"));
	statement_11_re_2_1_1.add_child(Token(token_symbol_regular_expression_letter, "z"));
	AbstractSyntaxTree& statement_11_re_2_1_2 = statement_11_re_2_1.add_child(token_symbol_regular_expression_range_block);
	statement_11_re_2_1_2.add_child(Token(token_symbol_regular_expression_letter, "A"));
	statement_11_re_2_1_2.add_child(Token(token_symbol_rule_terminal, "-"));
	statement_11_re_2_1_2.add_child(Token(token_symbol_regular_expression_letter, "Z"));
	AbstractSyntaxTree& statement_11_re_2_1_3 = statement_11_re_2_1.add_child(token_symbol_regular_expression_range_block);
	statement_11_re_2_1_3.add_child(Token(token_symbol_regular_expression_letter, "0"));
	statement_11_re_2_1_3.add_child(Token(token_symbol_rule_terminal, "-"));
	statement_11_re_2_1_3.add_child(Token(token_symbol_regular_expression_letter, "9"));
	AbstractSyntaxTree& statement_11_re_2_1_4 = statement_11_re_2_1.add_child(token_symbol_regular_expression_range_block);
	statement_11_re_2_1_4.add_child(Token(token_symbol_regular_expression_letter, "_"));
	statement_11_re_2_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	//	scope_identifier := identifier | scope_identifier "." identifier;
	AbstractSyntaxTree& statement_12 = ast_root.add_child(token_symbol_statement);
	AbstractSyntaxTree& statement_12_rule = statement_10.add_child(token_symbol_rule);

	statement_12_rule.add_child(Token(token_symbol_rule_name, "scope_identifier"));
	statement_12_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& statement_12_alternative = statement_12_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& statement_12_alternative_1 = statement_12_alternative.add_child(token_symbol_rule_definition);
	statement_12_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "identifier"));


	AbstractSyntaxTree& statement_12_alternative_2 = statement_12_alternative.add_child(token_symbol_rule_definition);
	statement_12_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "scope_identifier"));
	statement_12_alternative_2.add_child(Token(token_symbol_rule_terminal, "."));
	statement_12_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "identifier"));

	statement_12.add_child(Token(token_symbol_unamed_terminal, ";"));

	AbstractSyntaxTree& literal_string_scope_statement = ast_root.add_child(token_symbol_scope_statement);
	literal_string_scope_statement.add_child(Token(token_symbol_unamed_terminal, "scope"));
	literal_string_scope_statement.add_child(Token(token_symbol_identifier, "literal_string"));
	literal_string_scope_statement.add_child(Token(token_symbol_unamed_terminal, "{"));

	AbstractSyntaxTree& literal_string_scope = literal_string_scope_statement.add_child(token_symbol_scope_content);

	// option lexer_omit_token =;
	AbstractSyntaxTree& literal_string_statement_1 = literal_string_scope.add_child(token_symbol_statement);
	literal_string_statement_1.add_child(Token(token_symbol_option, "option"));
	literal_string_statement_1.add_child(Token(token_symbol_identifier, "lexer_omit_token"));
	literal_string_statement_1.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& literal_string_statement_1_alternative = literal_string_statement_1.add_child(token_symbol_rule_definition_alternative);
	literal_string_statement_1_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_1.add_child(Token(token_symbol_unamed_terminal, ";"));

	// root := "\"" character* "\"";
	AbstractSyntaxTree& literal_string_statement_2 = literal_string_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& literal_string_statement_2_rule = literal_string_statement_2.add_child(token_symbol_rule);

	literal_string_statement_2_rule.add_child(Token(token_symbol_rule_name, "root"));
	literal_string_statement_2_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& literal_string_statement_2_alternative = literal_string_statement_2_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& literal_string_statement_2_alternative_1 = literal_string_statement_2_alternative.add_child(token_symbol_rule_definition);

	literal_string_statement_2_alternative_1.add_child(Token(token_symbol_rule_terminal, "\""));
	AbstractSyntaxTree& literal_string_statement_2_alternative_1_1 = literal_string_statement_2_alternative_1.add_child(token_symbol_rule_repetition);
	literal_string_statement_2_alternative_1_1.add_child(Token(token_symbol_rule_non_terminal, "character"));
	literal_string_statement_2_alternative_1.add_child(Token(token_symbol_rule_terminal, "\""));

	literal_string_statement_2.add_child(Token(token_symbol_unamed_terminal, ";"));

	// character := generic_character | special_character;
	AbstractSyntaxTree& literal_string_statement_3 = literal_string_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& literal_string_statement_3_rule = literal_string_statement_3.add_child(token_symbol_rule);

	literal_string_statement_3_rule.add_child(Token(token_symbol_rule_name, "character"));
	literal_string_statement_3_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& literal_string_statement_3_alternative = literal_string_statement_3_rule.add_child(token_symbol_rule_definition_alternative);

	AbstractSyntaxTree& literal_string_statement_3_alternative_1 = literal_string_statement_3_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_3_alternative_1.add_child(Token(token_symbol_rule_non_terminal, "generic_character"));

	AbstractSyntaxTree& literal_string_statement_3_alternative_2 = literal_string_statement_3_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_3_alternative_2.add_child(Token(token_symbol_rule_non_terminal, "special_character"));

	literal_string_statement_3.add_child(Token(token_symbol_unamed_terminal, ";"));

	// generic_character := #[^\\"]#;
	AbstractSyntaxTree& literal_string_statement_4 = literal_string_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& literal_string_statement_4_rule = literal_string_statement_4.add_child(token_symbol_rule);

	literal_string_statement_4_rule.add_child(Token(token_symbol_rule_name, "generic_character"));
	literal_string_statement_4_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& literal_string_statement_4_re = literal_string_statement_4_rule.add_child(token_symbol_regular_expression);
	AbstractSyntaxTree& literal_string_statement_4_re_1 = literal_string_statement_4_re.add_child(token_symbol_regular_expression_repetition);
	AbstractSyntaxTree& literal_string_statement_4_re_1_1 = literal_string_statement_4_re_1.add_child(token_symbol_regular_expression_range);
	literal_string_statement_4_re_1_1.add_child(Token(token_symbol_unamed_terminal, "[^"));
	AbstractSyntaxTree& literal_string_statement_4_re_1_1_1 = literal_string_statement_4_re_1_1.add_child(token_symbol_regular_expression_range_block);
	literal_string_statement_4_re_1_1_1.add_child(Token(token_symbol_rule_terminal, "\""));
	literal_string_statement_4_re_1_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	literal_string_statement_4.add_child(Token(token_symbol_unamed_terminal, ";"));


	// special_character := "\\\n" | "\\\t" | "\\\"";
	AbstractSyntaxTree& literal_string_statement_5 = literal_string_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& literal_string_statement_5_rule = literal_string_statement_3.add_child(token_symbol_rule);

	literal_string_statement_5_rule.add_child(Token(token_symbol_rule_name, "special_character"));
	literal_string_statement_5_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& literal_string_statement_5_alternative = literal_string_statement_5_rule.add_child(token_symbol_rule_definition_alternative);

	AbstractSyntaxTree& literal_string_statement_5_alternative_1 = literal_string_statement_5_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_5_alternative_1.add_child(Token(token_symbol_rule_terminal, "\\n"));

	AbstractSyntaxTree& literal_string_statement_5_alternative_2 = literal_string_statement_5_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_5_alternative_2.add_child(Token(token_symbol_rule_terminal, "\\r"));

	AbstractSyntaxTree& literal_string_statement_5_alternative_3 = literal_string_statement_5_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_5_alternative_3.add_child(Token(token_symbol_rule_terminal, "\\t"));

	AbstractSyntaxTree& literal_string_statement_5_alternative_4 = literal_string_statement_5_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_5_alternative_4.add_child(Token(token_symbol_rule_terminal, "\\\""));

	AbstractSyntaxTree& literal_string_statement_5_alternative_5 = literal_string_statement_5_alternative.add_child(token_symbol_rule_definition);
	literal_string_statement_5_alternative_5.add_child(Token(token_symbol_rule_terminal, "\\\\"));

	literal_string_statement_5.add_child(Token(token_symbol_unamed_terminal, ";"));


	literal_string_scope_statement.add_child(Token(token_symbol_unamed_terminal, "}"));


	AbstractSyntaxTree& regular_expression_scope_statement = ast_root.add_child(token_symbol_scope_statement);
	regular_expression_scope_statement.add_child(Token(token_symbol_unamed_terminal, "scope"));
	regular_expression_scope_statement.add_child(Token(token_symbol_identifier, "regular_expression"));
	regular_expression_scope_statement.add_child(Token(token_symbol_unamed_terminal, "{"));

	AbstractSyntaxTree& regular_expression_scope = regular_expression_scope_statement.add_child(token_symbol_scope_content);

	// option lexer_omit_token =;
	AbstractSyntaxTree& regular_expression_statement_1 = regular_expression_scope.add_child(token_symbol_statement);
	regular_expression_statement_1.add_child(Token(token_symbol_option, "option"));
	regular_expression_statement_1.add_child(Token(token_symbol_identifier, "lexer_omit_token"));
	regular_expression_statement_1.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_1_alternative = regular_expression_statement_1.add_child(token_symbol_rule_definition_alternative);
	regular_expression_statement_1_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_1.add_child(Token(token_symbol_unamed_terminal, ";"));

	// root 		:= "#" block* "#";
	AbstractSyntaxTree& regular_expression_statement_2 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_2_rule = regular_expression_statement_2.add_child(token_symbol_rule);

	regular_expression_statement_2_rule.add_child(Token(token_symbol_rule_name, "root"));
	regular_expression_statement_2_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_2_alternative = regular_expression_statement_2_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& regular_expression_statement_2_alternative_1 = regular_expression_statement_2_alternative.add_child(token_symbol_rule_definition);

	regular_expression_statement_2_alternative_1.add_child(Token(token_symbol_rule_terminal, "#"));
	AbstractSyntaxTree& regular_expression_statement_2_alternative_1_1 = regular_expression_statement_2_alternative_1.add_child(token_symbol_rule_repetition);
	regular_expression_statement_2_alternative_1_1.add_child(Token(token_symbol_rule_non_terminal, "block"));
	regular_expression_statement_2_alternative_1.add_child(Token(token_symbol_rule_terminal, "#"));

	regular_expression_statement_2.add_child(Token(token_symbol_unamed_terminal, ";"));



	// block	:= letter quantifier? |  range quantifier?;
	AbstractSyntaxTree& regular_expression_statement_3 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_3_rule = regular_expression_statement_3.add_child(token_symbol_rule);

	regular_expression_statement_3_rule.add_child(Token(token_symbol_rule_name, "block"));
	regular_expression_statement_3_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_3_definition_alternative = regular_expression_statement_3_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& regular_expression_statement_3_1 = regular_expression_statement_3_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_3_1.add_child(Token(token_symbol_rule_non_terminal, "letter"));
	AbstractSyntaxTree& regular_expression_statement_3_1_ = regular_expression_statement_3_1.add_child(token_symbol_rule_optional);
	regular_expression_statement_3_1_.add_child(Token(token_symbol_rule_non_terminal, "quantifier"));
	AbstractSyntaxTree& regular_expression_statement_3_2 = regular_expression_statement_3_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_3_2.add_child(Token(token_symbol_rule_non_terminal, "range"));
	AbstractSyntaxTree& regular_expression_statement_3_2_ = regular_expression_statement_3_2.add_child(token_symbol_rule_optional);
	regular_expression_statement_3_2_.add_child(Token(token_symbol_rule_non_terminal, "quantifier"));

	regular_expression_statement_3.add_child(Token(token_symbol_unamed_terminal, ";"));

	// regular_expression_range := "[" regular_expression_range_block* "]" | "[^" regular_expression_range_block* "]";
	AbstractSyntaxTree& regular_expression_statement_4 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_4_rule = regular_expression_statement_4.add_child(token_symbol_rule);

	regular_expression_statement_4_rule.add_child(Token(token_symbol_rule_name, "range"));
	regular_expression_statement_4_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_4_definition_alternative = regular_expression_statement_4_rule.add_child(token_symbol_rule_definition_alternative);

	AbstractSyntaxTree& regular_expression_statement_4_1 = regular_expression_statement_4_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_4_1.add_child(Token(token_symbol_rule_terminal, "["));
	AbstractSyntaxTree& regular_expression_statement_4_1_1 = regular_expression_statement_4_1.add_child(Token(token_symbol_rule_repetition));
	regular_expression_statement_4_1_1.add_child(Token(token_symbol_rule_non_terminal, "range_block"));
	regular_expression_statement_4_1.add_child(Token(token_symbol_rule_terminal, "]"));

	AbstractSyntaxTree& regular_expression_statement_4_2 = regular_expression_statement_4_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_4_2.add_child(Token(token_symbol_rule_terminal, "[^"));
	AbstractSyntaxTree& regular_expression_statement_4_2_1 = regular_expression_statement_4_2.add_child(Token(token_symbol_rule_repetition));
	regular_expression_statement_4_2_1.add_child(Token(token_symbol_rule_non_terminal, "range_block"));
	regular_expression_statement_4_2.add_child(Token(token_symbol_rule_terminal, "]"));

	regular_expression_statement_4.add_child(Token(token_symbol_unamed_terminal, ";"));

	//     range_block 	:= letter | letter "-" letter;
	AbstractSyntaxTree& regular_expression_statement_5 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_5_rule = regular_expression_statement_5.add_child(token_symbol_rule);

	regular_expression_statement_5_rule.add_child(Token(token_symbol_rule_name, "range_block"));
	regular_expression_statement_5_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_5_definition_alternative = regular_expression_statement_5_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& regular_expression_statement_5_1 = regular_expression_statement_5_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_5_1.add_child(Token(token_symbol_rule_non_terminal, "letter"));
	AbstractSyntaxTree& regular_expression_statement_5_2 = regular_expression_statement_5_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_5_2.add_child(Token(token_symbol_rule_non_terminal, "letter"));
	regular_expression_statement_5_2.add_child(Token(token_symbol_rule_terminal, "-"));
	regular_expression_statement_5_2.add_child(Token(token_symbol_rule_non_terminal, "letter"));

	regular_expression_statement_5.add_child(Token(token_symbol_unamed_terminal, ";"));

	// quantifier := "+" | "*" | "?" | "{" digit "," digit "}"
	AbstractSyntaxTree& regular_expression_statement_6 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_6_rule = regular_expression_statement_6.add_child(token_symbol_rule);


	regular_expression_statement_6_rule.add_child(Token(token_symbol_rule_name, "quantifier"));
	regular_expression_statement_6_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_6_definition_alternative = regular_expression_statement_6_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& regular_expression_statement_6_1 = regular_expression_statement_6_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_6_1.add_child(Token(token_symbol_rule_terminal, "+"));
	AbstractSyntaxTree& regular_expression_statement_6_2 = regular_expression_statement_6_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_6_2.add_child(Token(token_symbol_rule_terminal, "*"));
	AbstractSyntaxTree& regular_expression_statement_6_3 = regular_expression_statement_6_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_6_3.add_child(Token(token_symbol_rule_terminal, "?"));

	regular_expression_statement_6.add_child(Token(token_symbol_unamed_terminal, ";"));

	// letter := letter_authorized | letter_protected

	AbstractSyntaxTree& regular_expression_statement_7 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_7_rule = regular_expression_statement_7.add_child(token_symbol_rule);

	regular_expression_statement_7_rule.add_child(Token(token_symbol_rule_name, "letter"));
	regular_expression_statement_7_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_7_definition_alternative = regular_expression_statement_7_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& regular_expression_statement_7_1 = regular_expression_statement_7_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_7_1.add_child(Token(token_symbol_rule_non_terminal, "letter_authorized"));
	AbstractSyntaxTree& regular_expression_statement_7_2 = regular_expression_statement_7_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_7_2.add_child(Token(token_symbol_rule_non_terminal, "letter_protected"));

	regular_expression_statement_7.add_child(Token(token_symbol_unamed_terminal, ";"));

	// letter_authorized := #[^\[\]\#\+\*\?\-]#
	AbstractSyntaxTree& regular_expression_statement_8 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_8_rule = regular_expression_statement_8.add_child(token_symbol_rule);

	regular_expression_statement_8_rule.add_child(Token(token_symbol_rule_name, "letter_authorized"));
	regular_expression_statement_8_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_8_re = regular_expression_statement_8_rule.add_child(token_symbol_regular_expression);

	AbstractSyntaxTree& regular_expression_statement_8_re_1 = regular_expression_statement_8_re.add_child(token_symbol_regular_expression_range);
	regular_expression_statement_8_re_1.add_child(Token(token_symbol_unamed_terminal, "[^"));
	AbstractSyntaxTree& regular_expression_statement_8_re_1_1 = regular_expression_statement_8_re_1.add_child(token_symbol_regular_expression_range_block);
	regular_expression_statement_8_re_1_1.add_child(Token(token_symbol_rule_terminal, "["));
	AbstractSyntaxTree& regular_expression_statement_8_re_1_2 = regular_expression_statement_8_re_1.add_child(token_symbol_regular_expression_range_block);
	regular_expression_statement_8_re_1_2.add_child(Token(token_symbol_rule_terminal, "]"));
	AbstractSyntaxTree& regular_expression_statement_8_re_1_3 = regular_expression_statement_8_re_1.add_child(token_symbol_regular_expression_range_block);
	regular_expression_statement_8_re_1_3.add_child(Token(token_symbol_rule_terminal, "#"));
	AbstractSyntaxTree& regular_expression_statement_8_re_1_4 = regular_expression_statement_8_re_1.add_child(token_symbol_regular_expression_range_block);
	regular_expression_statement_8_re_1_4.add_child(Token(token_symbol_rule_terminal, "+"));
	AbstractSyntaxTree& regular_expression_statement_8_re_1_5 = regular_expression_statement_8_re_1.add_child(token_symbol_regular_expression_range_block);
	regular_expression_statement_8_re_1_5.add_child(Token(token_symbol_rule_terminal, "*"));
	AbstractSyntaxTree& regular_expression_statement_8_re_1_6 = regular_expression_statement_8_re_1.add_child(token_symbol_regular_expression_range_block);
	regular_expression_statement_8_re_1_6.add_child(Token(token_symbol_rule_terminal, "?"));
	AbstractSyntaxTree& regular_expression_statement_8_re_1_7 = regular_expression_statement_8_re_1.add_child(token_symbol_regular_expression_range_block);
	regular_expression_statement_8_re_1_7.add_child(Token(token_symbol_rule_terminal, "-"));
	regular_expression_statement_8_re_1.add_child(Token(token_symbol_unamed_terminal, "]"));

	regular_expression_statement_8.add_child(Token(token_symbol_unamed_terminal, ";"));

	//regular_expression_letter_protected := "\[" | "\]" | "\#" | "\+" | "\*" | "\?" | "\-"
	AbstractSyntaxTree& regular_expression_statement_9 = regular_expression_scope.add_child(token_symbol_statement);
	AbstractSyntaxTree& regular_expression_statement_9_rule = regular_expression_statement_9.add_child(token_symbol_rule);

	regular_expression_statement_9_rule.add_child(Token(token_symbol_rule_name, "letter_protected"));
	regular_expression_statement_9_rule.add_child(token_symbol_unamed_terminal);
	AbstractSyntaxTree& regular_expression_statement_9_definition_alternative = regular_expression_statement_9_rule.add_child(token_symbol_rule_definition_alternative);
	AbstractSyntaxTree& regular_expression_statement_9_1 = regular_expression_statement_9_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_9_1.add_child(Token(token_symbol_rule_terminal, "\\["));
	AbstractSyntaxTree& regular_expression_statement_9_2 = regular_expression_statement_9_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_9_2.add_child(Token(token_symbol_rule_terminal, "\\]"));
	AbstractSyntaxTree& regular_expression_statement_9_3 = regular_expression_statement_9_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_9_3.add_child(Token(token_symbol_rule_terminal, "\\#"));
	AbstractSyntaxTree& regular_expression_statement_9_4 = regular_expression_statement_9_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_9_4.add_child(Token(token_symbol_rule_terminal, "\\+"));
	AbstractSyntaxTree& regular_expression_statement_9_5 = regular_expression_statement_9_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_9_5.add_child(Token(token_symbol_rule_terminal, "\\*"));
	AbstractSyntaxTree& regular_expression_statement_9_6 = regular_expression_statement_9_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_9_6.add_child(Token(token_symbol_rule_terminal, "\\?"));
	AbstractSyntaxTree& regular_expression_statement_9_7 = regular_expression_statement_9_definition_alternative.add_child(token_symbol_rule_definition);
	regular_expression_statement_9_7.add_child(Token(token_symbol_rule_terminal, "\\-"));

	regular_expression_statement_9.add_child(Token(token_symbol_unamed_terminal, ";"));


	regular_expression_scope_statement.add_child(Token(token_symbol_unamed_terminal, "}"));


	std::cout << "Abstract Syntax Tree: " << std::endl;
	ast_root.print(std::cout);
	std::cout << std::endl;

	EBNFScope root_scope(nullptr, "");
	EBNFConstruct::process(_syntax, ast_root, root_scope);



	std::cout << "EBNF Representation: " << std::endl;
	root_scope.print(std::cout);
	std::cout << std::endl;

	//generate_syntax(root_scope);
}
/*
void EBNF::generate_syntax(const AbstractSyntaxTree& syntax_tree) {
	Syntax generated_syntax;

	QualifiedIdentifierNode<EBNFSymbolType> root_identifier_node("", nullptr);
	extract_symbols(syntax_tree, QualifiedIdentifier<EBNFSymbolType>(root_identifier_node));

	std::vector<std::pair<std::string, bool>> symbols_name;
	generate_symbol_name(QualifiedIdentifier<EBNFSymbolType>(root_identifier_node), symbols_name);

	symbols_name.push_back(std::make_pair(Syntax::EOF_symbol, true));
	symbols_name.push_back(std::make_pair(Syntax::Root_symbol, false));

	std::sort(std::begin(symbols_name), std::end(symbols_name), [](const std::pair<std::string, bool>& symbol_1, const std::pair<std::string, bool>& symbol_2) {
		return symbol_1.second != symbol_2.second ? symbol_1.second : symbol_1.first < symbol_2.first;
	});

	symbols_name.erase(std::unique(std::begin(symbols_name), std::end(symbols_name), [](const std::pair<std::string, bool>& symbol_1, const std::pair<std::string, bool>& symbol_2) {
		return symbol_1.second == symbol_2.second && symbol_1.first < symbol_2.first;
	}), std::end(symbols_name));

	std::vector<std::string> ordered_symbols_name;
	ordered_symbols_name.resize(symbols_name.size());
	std::transform(std::begin(symbols_name), std::end(symbols_name), std::begin(ordered_symbols_name), [](const std::pair<std::string, bool>& symbol) {return symbol.first;});


	unsigned int terminal_number = std::count_if(std::begin(symbols_name), std::end(symbols_name),  [](const std::pair<std::string, bool>& symbol) -> bool {return symbol.second;});
	generated_syntax.set_symbol_table(ordered_symbols_name, terminal_number);

	SyntaxRepresentation syntax_representation(_syntax, generated_syntax, syntax_tree);
	syntax_representation.print(std::cout);
}
*/

/*
void EBNF::extract_symbols(const AbstractSyntaxTree& syntax_tree, QualifiedIdentifier<EBNFSymbolType> current_prefix) {
	if(syntax_tree.type() == _syntax.get_symbol_from_name("rule_terminal")) {
		current_prefix.create_child(syntax_tree.content(), EBNF_TERMINAL);
	}
	else if(syntax_tree.type() == _syntax.get_symbol_from_name("rule")) {

		if(current_prefix.has_child(syntax_tree.children()[0]->content())) {
			throw exception::UserMessage(exception::UserMessage::Error, "EBNF: "+current_prefix.get_child(syntax_tree.children()[0]->content()).to_string()+" rule already defined");
		}

		if(syntax_tree.children()[2]->type() == _syntax.get_symbol_from_name("regular_expression")) {
			current_prefix.create_child(syntax_tree.children()[0]->content(), EBNF_TERMINAL);
		}
		else {
			current_prefix.create_child(syntax_tree.children()[0]->content(), EBNF_NON_TERMINAL);
			extract_symbols(*syntax_tree.children()[2], current_prefix);
		}
	}
	else if(syntax_tree.type() == _syntax.get_symbol_from_name("scope_statement")) {
		extract_symbols(*syntax_tree.children()[3], current_prefix.create_child(syntax_tree.children()[1]->content()));
	}
	else {
		for(const AbstractSyntaxTree* child : syntax_tree.children())
			extract_symbols(*child, current_prefix);
	}
}

void EBNF::generate_symbol_name(QualifiedIdentifier<EBNFSymbolType> node, std::vector<std::pair<std::string, bool>>& output) {
	if(node.children_number() == 0) { // leaf
		output.emplace_back(node.to_string(), node.info() == EBNF_TERMINAL);
	}
	else {
		for(auto& child : node.children()) {
			generate_symbol_name(QualifiedIdentifier<EBNFSymbolType>(child.second), output);
		}
	}

}
*/
