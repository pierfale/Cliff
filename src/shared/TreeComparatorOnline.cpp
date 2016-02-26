#include "cliff/shared/TreeComparatorOnline.h"

using namespace cliff;

TreeComparatorOnline::TreeComparatorOnline() {

}

TreeComparatorOnline::Handler& TreeComparatorOnline::match_with(const AbstractSyntaxTree& tree) {
	_comparator_list.emplace_back(tree);
	return _comparator_list.back();
}

/*
 *	Handler
 */

TreeComparatorOnline::Handler::Handler(const AbstractSyntaxTree& first_node) : _equals(true) {
	_node_stack.push(&first_node);
}

TreeComparatorOnline::Handler::operator bool() const {
	return _equals;
}

TreeComparatorOnline::Handler& TreeComparatorOnline::Handler::of_type(const char* type_str) {
	if(_equals && std::strcmp(type_str, _node_stack.top()->type().string()) != 0)
		_equals = false;

	return *this;
}

TreeComparatorOnline::Handler& TreeComparatorOnline::Handler::in_child(unsigned int position) {
	if(_node_stack.top()->children().size() <= position)
		_equals = false;
	else
		_node_stack.push(_node_stack.top()->children()[position]);

	return *this;
}

TreeComparatorOnline::Handler& TreeComparatorOnline::Handler::end_child() {
	if(_node_stack.size() <= 1)
		throw std::runtime_error("invalid tree comparator");

	_node_stack.pop();

	return *this;
}


TreeComparatorOnline::Handler& TreeComparatorOnline::Handler::has_child_of_type(const char* type_str) {
	if(_equals) {
		bool found = false;
		for(AbstractSyntaxTree* child : _node_stack.top()->children()) {
			if(std::strcmp(type_str, child->type().string()) != 0) {
				found = true;
				break;
			}
		}
		if(!found)
			_equals = false;
	}

	return *this;
}

TreeComparatorOnline::Handler& TreeComparatorOnline::Handler::has_child_of_type_at_position(const char* type_str, unsigned int position) {
	if(_equals && (_node_stack.top()->children().size() <= position || std::strcmp(type_str, _node_stack.top()->children()[position]->type().string()) != 0))
		_equals = false;

	return *this;
}

TreeComparatorOnline::Handler& TreeComparatorOnline::Handler::has_child_number(unsigned int number) {
	if(_equals && _node_stack.top()->children().size() != number)
		_equals = false;

	return *this;
}

