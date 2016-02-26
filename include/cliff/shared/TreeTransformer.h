#ifndef _CLIFF_TREE_TRANSFORMER_H
#define _CLIFF_TREE_TRANSFORMER_H

#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/Exception.h"
#include "cliff/shared/TreeComparatorMapper.h"

namespace cliff {

	class TreeTransformer {

	public:

		typedef bool(*FilterFunction)(const TreeComparatorMapper<unsigned int>::Handler&);

		enum Action {
			None,
			Remove_recursive,
			Ascend
		};

		TreeTransformer();

		void add_rule(FilterFunction filter, Action action);

		AbstractSyntaxTree* execute(const AbstractSyntaxTree& tree_root, MemoryContainer<AbstractSyntaxTree>& output_container);

	private:
		AbstractSyntaxTree* _execute(const AbstractSyntaxTree& tree_root, MemoryContainer<AbstractSyntaxTree>& output_container, AbstractSyntaxTree* parent_node);

		std::vector<std::pair<FilterFunction, Action>> _rules;
	};

}


#endif
