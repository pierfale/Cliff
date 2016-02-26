#ifndef _CLIFF_TREE_COMPARATOR_ONLINE_H
#define _CLIFF_TREE_COMPARATOR_ONLINE_H

#include <stack>
#include <stdexcept>

#include "cliff/shared/AbstractSyntaxTree.h"

namespace cliff {

	class TreeComparatorOnline {

	public:
		class Handler {

		public:
			Handler(const AbstractSyntaxTree& first_node);

			operator bool() const;

			Handler& of_type(const char* type_str);
			Handler& in_child(unsigned int position);
			Handler& end_child();
			Handler& has_child_of_type(const char* type_str);
			Handler& has_child_of_type_at_position(const char* type_str, unsigned int position);
			Handler& has_child_number(unsigned int number);

		private:
			bool _equals;
			std::stack<const AbstractSyntaxTree*> _node_stack;
		};

		TreeComparatorOnline();

		Handler& match_with(const AbstractSyntaxTree& tree);

	private:
		std::vector<Handler> _comparator_list;

	};
}

#endif
