#include "cliff/tool/syntax_generator/EBNFConstruct.h"

using namespace cliff;

void EBNFConstruct::process(const Syntax& syntax, const AbstractSyntaxTree& input, EBNFScope& output) {
	process_scope(syntax, input, output);

	std::vector<EBNFScope*> prefix;
	bind_reference(output, prefix);
}

void EBNFConstruct::bind_reference(EBNFScope& scope, std::vector<EBNFScope*>& prefix) {

	prefix.push_back(&scope);
	for(auto& operation : scope._rules) {
		bind_reference(operation.second, prefix);
	}


	for(auto& child_scope : scope._children_scope) {
		bind_reference(child_scope.second, prefix);
	}
	prefix.pop_back();



}

void EBNFConstruct::bind_reference(EBNFOperation& operation, const std::vector<EBNFScope*>& prefix) {
	switch (operation._type) {
	case EBNF_NON_TERMINAL:
		{
			std::vector<const char*> name;
			for(const auto& child : operation._children) {
				name.push_back(child._content);
			}

			int scope_index =  prefix.size()-1/*-operation._children.size()*/;

			while(scope_index >= 0) {
				EBNFScope* current_scope = prefix.at(scope_index);

				if(resolve_name(*current_scope, name, 0)) {
					while(current_scope != nullptr && current_scope->_scope_name != "") {
						name.insert(std::begin(name), current_scope->_scope_name.c_str());
						current_scope = current_scope->_parent_scope;
					}
					break;
				}

				scope_index--;
			}

			if(scope_index < 0) {
				throw exception::UserMessage(exception::UserMessage::Error, "EBNF: Can't resolve "+std::accumulate(std::begin(name), std::end(name), std::string(), [](const std::string& acc, const std::string& curr) {
						return acc+"."+curr;
					}
				));
			}

			operation._children.clear();
			for(const char* str : name) {
				operation._children.emplace_back(EBNF_SCOPE_NAME, str);
			}

		}
		break;
	default:
		for(auto& child : operation._children) {
			bind_reference(child, prefix);
		}
		break;
	}

}

bool EBNFConstruct::resolve_name(EBNFScope& scope, const std::vector<const char*>& name, size_t name_index) {
	if(name.size() > name_index+1) {
		auto scope_it = scope._children_scope.find(name.at(name_index));

		if(scope_it == std::end(scope._children_scope)) {
			return false;
		}
		else {
			return resolve_name(scope_it->second, name, name_index+1);
		}
	}
	else if(name_index == name.size()-1){
		auto rule_it = scope._rules.find(name.at(name_index));

		if(rule_it != std::end(scope._rules)) {
			return true;
		}
		else {
			auto re_it = scope._regular_expressions.find(name.at(name_index));

			return re_it != std::end(scope._regular_expressions);
		}
	}
	else {
		return false;
	}
}

void EBNFConstruct::process_scope(const Syntax& syntax, const AbstractSyntaxTree& input, EBNFScope& output) {
	if(input.type() == syntax.get_symbol_from_name("scope_statement")) { // Sub scope
		const char* scope_name = input.children()[1]->content();
		EBNFScope& child_scope = output._children_scope.emplace(std::piecewise_construct, std::forward_as_tuple(scope_name), std::forward_as_tuple(&output, scope_name)).first->second;
		process_scope(syntax, *input.children()[3], child_scope);
	}
	else if(input.type() == syntax.get_symbol_from_name("rule")) { // Rule
		const char* rule_name = input.children()[0]->content();
		if(input.children()[2]->type() == syntax.get_symbol_from_name("regular_expression")) {
			output._regular_expressions.emplace(std::piecewise_construct, std::forward_as_tuple(rule_name), std::forward_as_tuple(syntax, *input.children()[2], input.children()[0]->type()));

		}
		else {
			EBNFOperation& root_operation = output._rules.emplace(std::piecewise_construct, std::forward_as_tuple(rule_name), std::forward_as_tuple(EBNF_ROOT)).first->second;
			process_rule(syntax, *input.children()[2], root_operation);
		}
	}
	else if(input.type() == syntax.get_symbol_from_name("statement") && input.children()[0]->type() == syntax.get_symbol_from_name("option")) { // Option
		const char* option_name = input.children()[1]->content();

		if(strcmp(option_name, "lexer_omit_token") == 0) {
			process_rule(syntax, *input.children()[3], output._lexer_omit_token);
		}
		else {
			throw exception::UserMessage(exception::UserMessage::Error, "EBNF: Unkown option "+std::string(option_name));
		}
	}
	else {
		for(const AbstractSyntaxTree* child : input.children())
			process_scope(syntax, *child, output);
	}
}

void EBNFConstruct::process_rule(const Syntax& syntax, const AbstractSyntaxTree& input, EBNFOperation& output) {
	/*
	if(input.type() == syntax.get_symbol_from_name("rule_alternative")) {
		if(input.children().size() == 3) {
			output._children.emplace_back(EBNF_ALTERNATIVE);
			EBNFOperation& child = output._children.back();
			process_rule(syntax, *input.children()[0], child);
			process_rule(syntax, *input.children()[2], child);
		}
		else {
			process_rule(syntax, *input.children()[0], output);
		}
	}
	*/
	if(input.type() == syntax.get_symbol_from_name("rule_alternative")) {
		if(input.children().size() == 0) {
			// nothing
		}
		else if(input.children().size() == 1) {
			process_rule(syntax, *input.children()[0], output);
		}
		else {
			output._children.emplace_back(EBNF_ALTERNATIVE);
			EBNFOperation& child_op = output._children.back();
			for(const AbstractSyntaxTree* child : input.children()) {
				process_rule(syntax, *child, child_op);
			}
		}
	}
	else if(input.type() == syntax.get_symbol_from_name("rule_sequence")) {
		if(input.children().size() == 0) {
			// nothing
		}
		else if(input.children().size() == 1) {
			process_rule(syntax, *input.children()[0], output);
		}
		else {
			output._children.emplace_back(EBNF_SEQUENCE);
			EBNFOperation& child_op = output._children.back();
			for(const AbstractSyntaxTree* child : input.children()) {
				process_rule(syntax, *child, child_op);
			}
		}
	}
	else if(input.type() == syntax.get_symbol_from_name("rule_repetition")) {
		output._children.emplace_back(EBNF_REPETITION);
		process_rule(syntax, *input.children()[0], output._children.back());
	}
	else if(input.type() == syntax.get_symbol_from_name("rule_optional")) {
		output._children.emplace_back(EBNF_OPTIONAL);
		process_rule(syntax, *input.children()[0], output._children.back());
	}
	else if(input.type() == syntax.get_symbol_from_name("rule_terminal")) {
		output._children.emplace_back(EBNF_TERMINAL, input.content());
	}
	else if(input.type() == syntax.get_symbol_from_name("rule_non_terminal")) {
		output._children.emplace_back(EBNF_NON_TERMINAL);
		EBNFOperation& child_op = output._children.back();
		child_op._children.emplace_back(EBNF_SCOPE_NAME, input.content());

	}
	else if(input.type() == syntax.get_symbol_from_name("scope_identifier")) {
		output._children.emplace_back(EBNF_NON_TERMINAL);
		EBNFOperation& child_op = output._children.back();
		for(size_t i=0; i<input.children().size(); i += 2) {
			child_op._children.emplace_back(EBNF_SCOPE_NAME, input.children()[i]->content());
		}
	}
	else {
		throw exception::UserMessage(exception::UserMessage::Error, "EBNF: Unkown rule node type "+std::string(input.type().string()));
	}
}
