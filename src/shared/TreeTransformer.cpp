#include "cliff/shared/TreeTransformer.h"

using namespace cliff;


TreeTransformer::TreeTransformer() {

}

void TreeTransformer::add_rule(FilterFunction filter, Action action) {
	_rules.emplace_back(filter, action);
}

AbstractSyntaxTree* TreeTransformer::execute(const AbstractSyntaxTree& tree_root, MemoryContainer<AbstractSyntaxTree>& output_container) {
	TreeComparatorMapper<unsigned int> comparator;
	return _execute(tree_root, output_container, nullptr);
}

AbstractSyntaxTree* TreeTransformer::_execute(const AbstractSyntaxTree& tree_root, MemoryContainer<AbstractSyntaxTree>& output_container, AbstractSyntaxTree* parent_node) {
	Action action = None;
/*
	for(std::pair<FilterFunction, Action>& rule : _rules) {
		if(rule.first(tree_root)) {
			action = rule.second;
			break;
		}
	}*/

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
}

