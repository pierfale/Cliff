#include "cliff/shared/TreeTransformer.h"

using namespace cliff;


TreeTransformer::TreeTransformer() {

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

	AbstractSyntaxTree& output_tree_root = output_container.emplace(input_tree.token()); // TODO use output_syntax symbols
	_execute(output_syntax, input_tree, output_tree_root, output_container);
	return &output_tree_root;
}

AbstractSyntaxTree* TreeTransformer::_execute(const Syntax& syntax, const AbstractSyntaxTree& current_input_node, AbstractSyntaxTree& current_output_node,  MemoryContainer<AbstractSyntaxTree>& output_container) {

	for(std::pair<TreePatternMatching::Handle, ActionFunction>& rule : _rules) {
		if(rule.first.match_with(current_input_node)) {
			TreeTransformer::Context context(syntax, current_input_node);
			rule.second(context);
			break;
		}
	}

	for(AbstractSyntaxTree* child : current_input_node.children()) {
		_execute(syntax, *child, output_container);
	}

	return nullptr;


	/*
	for(std::pair<FilterFunction, Action>& rule : _rules) {
		if(rule.first(tree_root)) {
			action = rule.second;
			break;
		}
	}*/
/*
	switch(action) {

	case Remove_recursive:
		break;
	case Ascend:
		if(parent_node == nullptr && tree_root.children().size() > 1)
			THROW(exception::Exception, "TreeTransformer : parent node null");
		break;

		for(AbstractSyntaxTree* child : tree_root.children())
			_execute(*child, output_container, parent_node);

		return parent_node;
	case None:
		AbstractSyntaxTree& new_tree = parent_node == nullptr ? output_container.emplace(output_container, tree_root.token()) : parent_node->add_child(tree_root.token());

		for(AbstractSyntaxTree* child : tree_root.children())
			_execute(*child, output_container, &new_tree);

		return &new_tree;
	}

	return nullptr;
	*/
	return nullptr;
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

//
//	Context
//

TreeTransformer::Context::Context(const Syntax& syntax, AbstractSyntaxTree& current_node) : _current_node(current_node), _syntax(syntax) {

}

AbstractSyntaxTree& TreeTransformer::Context::node() {
	return _current_node;
}

const AbstractSyntaxTree& TreeTransformer::Context::node() const {
	return _current_node;
}

TreeTransformer::Context TreeTransformer::Context::child_at_position(unsigned int position) {
	return Context(_syntax, *_current_node.children()[position]);
}

const TreeTransformer::Context TreeTransformer::Context::child_at_position(unsigned int position) const {
	return Context(_syntax, *_current_node.children()[position]);
}

const Syntax& TreeTransformer::Context::syntax() const {
	return _syntax;
}

//
//	Context actions
//

void TreeTransformer::Context::remove_recursive() {

}

void TreeTransformer::Context::remove_and_ascend_children() {

}

void TreeTransformer::Context::replace_node(const TokenSymbol& type, const char* content) {
	_current_node.set(type, content);
}

void TreeTransformer::Context::replace_node_and_children(const TokenSymbol& type, const char* content) {
	_current_node.set(type, content);
	_current_node.children().clear();
}
