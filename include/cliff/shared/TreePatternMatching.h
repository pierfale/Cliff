#ifndef _CLIFF_TREE_PATTERN_MATCHING_H
#define _CLIFF_TREE_PATTERN_MATCHING_H

#include <map>
#include <stdexcept>

#include "cliff/shared/AbstractSyntaxTree.h"

namespace cliff {

	class TreePatternMatching {

	public:
		class Handle {

		public:
			Handle();

			//static const int ALL_CHILD = -1 ;

			Handle& type(const std::string& p_type);
			//Handle& children_number(unsigned int p_children_number);
			//Handle& with_child(int id, const Handle& child_handle);

			bool match_with(const AbstractSyntaxTree&) const;


		private:
			bool _has_type_constraint;
			//bool _has_children_number_constraint;

			//unsigned int _children_number_constraint;
			std::string _type_constraint;

			//std::map<int, Handle> _children_constraint;

		};

		TreePatternMatching() = delete;
		TreePatternMatching(const TreePatternMatching& that) = delete;
		TreePatternMatching& operator=(const TreePatternMatching& that) = delete;

		static TreePatternMatching::Handle node();
		static TreePatternMatching::Handle of_type(const std::string& p_type);

	};
}

#endif
