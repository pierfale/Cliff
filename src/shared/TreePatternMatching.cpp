#include "cliff/shared/TreePatternMatching.h"

using namespace cliff;

TreePatternMatching::Handle TreePatternMatching::node() {
	return TreePatternMatching::Handle();
}

TreePatternMatching::Handle TreePatternMatching::of_type(const std::string& p_type) {
	return TreePatternMatching::Handle().type(p_type);
}

TreePatternMatching::Handle::Handle() : _has_type_constraint(false), _type_constraint() {

}

TreePatternMatching::Handle& TreePatternMatching::Handle::type(const std::string& p_type) {
	_has_type_constraint = true;
	_type_constraint = p_type;
	return *this;
}

bool TreePatternMatching::Handle::match_with(const AbstractSyntaxTree& node) const {
	if(_has_type_constraint && node.type().string() != _type_constraint)
		return false;


	return true;
}
