#include "cliff/shared/TreeTransformer.h"

using namespace cliff;


TreeTransformer::TreeTransformer() : _output_syntax(nullptr) {

}

void TreeTransformer::add_rule(TreePatternMatching::Handle filter, ActionFunction action) {
	_rules.emplace_back(filter, action);
}

void TreeTransformer::add_symbol(const char* symbol_str, bool is_terminal) {
	_symbol_transformation.push_back(std::make_tuple(symbol_str, is_terminal, SymbolAction::ADD));
}

AbstractSyntaxTree* TreeTransformer::execute(const Syntax& input_syntax, Syntax& output_syntax, const AbstractSyntaxTree& input_tree, MemoryContainer<AbstractSyntaxTree>& output_container) {
	//
	//	Convert Syntax
	//
	_output_syntax = &output_syntax;
	std::vector<std::pair<const char*, bool>> symbol_list;
	input_syntax.get_symbol_list(symbol_list);

	for(const std::tuple<const char*, bool, SymbolAction>& symbol_action : _symbol_transformation) {
		switch(std::get<2>(symbol_action)) {

		case SymbolAction::ADD:
			symbol_list.push_back(std::make_pair(std::get<0>(symbol_action), std::get<1>(symbol_action)));
			break;

		case SymbolAction::REMOVE:
			std::remove_if(std::begin(symbol_list), std::end(symbol_list), [&symbol_action](const std::pair<const char*, bool>& element) {
				return element.second == std::get<1>(symbol_action) && std::strcmp(element.first, std::get<0>(symbol_action)) == 0;
			});
			break;
		}
	}

	output_syntax.set_symbol_list(symbol_list);

	//AbstractSyntaxTree& output_tree_root = output_container.emplace(input_tree.token()); // TODO use output_syntax symbols
	return _execute(output_syntax, input_tree, nullptr, output_container, false);
}

AbstractSyntaxTree* TreeTransformer::_execute(const Syntax& syntax, const AbstractSyntaxTree& current_input_node, AbstractSyntaxTree* parent_node,  MemoryContainer<AbstractSyntaxTree>& output_container, bool deleted) {

	Context& context = _get_or_create_context(&current_input_node);

	AbstractSyntaxTree* new_node = nullptr;

	bool node_deleted = deleted ? true : context._to_be_deleted;

	if(!node_deleted) {
		new_node = parent_node == nullptr ?
							&output_container.emplace(output_container, context._create()) :
							&parent_node->add_child(context._create());
	}

	for(AbstractSyntaxTree* child : current_input_node.children()) {
		_execute(syntax, *child, new_node, output_container, node_deleted);
	}

	return new_node;
}

//
//	Implemented actions
//

AbstractSyntaxTree* TreeTransformer::Action::none(AbstractSyntaxTree* current_node) {
	return current_node;
}

AbstractSyntaxTree* TreeTransformer::Action::remove_recursive(AbstractSyntaxTree* current_node) {

}

AbstractSyntaxTree* TreeTransformer::Action::ascend_children(AbstractSyntaxTree* current_node) {

}

TreeTransformer::Context& TreeTransformer::_get_or_create_context(const AbstractSyntaxTree* node) {
	auto context_it = _delegate_context.find(node);

	if(context_it == std::end(_delegate_context)) {
		context_it = _delegate_context.emplace(std::piecewise_construct, std::forward_as_tuple(node), std::forward_as_tuple(*this, *node)).first;

		for(std::pair<TreePatternMatching::Handle, ActionFunction>& rule : _rules) {
			if(rule.first.match_with(*node)) {
				rule.second(context_it->second);
			}
		}
	}

	return context_it->second;
}

//
//	Context
//

TreeTransformer::Context::Context(TreeTransformer& transformer, const AbstractSyntaxTree& input_node) : _transformer(transformer), _input_node(input_node),  _to_be_deleted(false), _new_symbol(), _has_new_content(false), _new_content() {

}

const AbstractSyntaxTree& TreeTransformer::Context::node() const {
	return _input_node;
}


TreeTransformer::Context& TreeTransformer::Context::child_at_position(int position) {
	unsigned int computed_position = position >= 0 ? position : _input_node.children().size()+position;
	return _transformer._get_or_create_context(_input_node.children()[computed_position]);
}

void TreeTransformer::Context::children_filter(TreePatternMatching::Handle filter, std::vector<Context*>& output_list) {
	for(const AbstractSyntaxTree* child : _input_node.children()) {
		if(filter.match_with(*child)) {
			output_list.push_back(&_transformer._get_or_create_context(child));
		}
	}
}

//
//	Context actions
//

void TreeTransformer::Context::remove() {
	_to_be_deleted = true;
}

void TreeTransformer::Context::remove_and_ascend_children() {
	// ?
}

void TreeTransformer::Context::remove_all_children() {
	for(unsigned int i = 0; i<_input_node.children().size(); i++) {
		child_at_position(i).remove();
	}

/*	for(const auto& child: _input_node.children()) {
		_children_actions.push_back(std::make_pair(child, ContextAction::DELETE));
	}*/
	// or create child context with delete = true ? -> need not create the ref in the parent so
}

void TreeTransformer::Context::replace_node(const std::string& type) {
	_new_symbol = type;
}

void TreeTransformer::Context::replace_node(const std::string& type, const std::string& content) {
	_new_symbol = type;
	_new_content = content;
	_has_new_content = true;
}

Token TreeTransformer::Context::_create() {
	return Token(_transformer._output_syntax->get_symbol_from_name(!_new_symbol.empty() ? _new_symbol.c_str() : _input_node.token().type().string()),
				 _has_new_content ? _new_content.c_str() : _input_node.token().content());
}
