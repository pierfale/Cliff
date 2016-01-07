#ifndef _CLIFF_TREE_TRANSFORMER_H
#define _CLIFF_TREE_TRANSFORMER_H

#include "cliff/shared/AbstractSyntaxTree.h"

namespace cliff {

	class TreeTransformer {

	public:
		class Handler {

		public:

			Handler(const AbstractSyntaxTree& tree_root);

			Handler& on_node_of_type(const TokenSymbol& symbol, void(*)(const AbstractSyntaxTree&), bool(*)(const AbstractSyntaxTree&, unsigned int));

			void execute();

		};

		static Handler transform(const AbstractSyntaxTree& tree_root);

		static bool browse_all_children(const AbstractSyntaxTree&, unsigned int);

	};

}


#endif
