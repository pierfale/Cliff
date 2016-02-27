#ifndef _CLIFF_TREE_COMPARATOR_MAPPER_H
#define _CLIFF_TREE_COMPARATOR_MAPPER_H

#include <stack>
#include <map>
#include <stdexcept>

#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/HashString.h"

namespace cliff {

	template<typename T>
	class TreeComparatorMapper {

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

		TreeComparatorMapper() {

		}

		Handler& add(T& value) {

		}

		T* find(const AbstractSyntaxTree& node) {

		}



	private:
		class Mapper {
			std::map<CopyHashString, Mapper*> _list;
			Mapper* _others;
			T& _value;
		};

		T* _find(const AbstractSyntaxTree& current_node, const Mapper& current_mapper) {
			if(current_node.type()) {

			}
		}

		std::vector<Handler> _comparator_list;


	};
}

#endif
