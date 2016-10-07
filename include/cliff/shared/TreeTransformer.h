#ifndef _CLIFF_TREE_TRANSFORMER_H
#define _CLIFF_TREE_TRANSFORMER_H

#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/Exception.h"
#include "cliff/shared/TreeComparatorMapper.h"
#include "cliff/shared/TreePatternMatching.h"
#include "cliff/front_end/Syntax.h"

namespace cliff {

	class TreeTransformer {

	public:
		class Context {

		public:
			Context(const Syntax& syntax, AbstractSyntaxTree& current_node);

			AbstractSyntaxTree& node();
			const AbstractSyntaxTree& node() const;

			Context child_at_position(unsigned int position);
			const Context child_at_position(unsigned int position) const;

			const Syntax& syntax() const;

			//
			//	Actions
			//


			void remove_recursive();
			void remove_and_ascend_children();
			void replace_node(const TokenSymbol& type, const char* content = nullptr);
			void replace_node_and_children(const TokenSymbol& type, const char* content = nullptr);

		private:
			AbstractSyntaxTree& _current_node;
			const Syntax& _syntax;

			// output properties
			std::string _new_symbol;
			std::string _new_content;

			enum ContextAction {

			};

			std::vector<ContextAction, const AbstractSyntaxTree*, > _children_actions;
		};

		typedef void(*ActionFunction)(TreeTransformer::Context&);

		TreeTransformer();

		void add_rule(TreePatternMatching::Handle filter, ActionFunction action);
		void add_symbol(const char* symbol_str, bool is_terminal);

		AbstractSyntaxTree* execute(const Syntax& input_syntax, Syntax& output_syntax, const AbstractSyntaxTree& input_tree, MemoryContainer<AbstractSyntaxTree>& output_container);

		//
		//	Implemented actions
		//
		struct Action {
			AbstractSyntaxTree* none(AbstractSyntaxTree* current_node);
			AbstractSyntaxTree* remove_recursive(AbstractSyntaxTree* current_node);
			AbstractSyntaxTree* ascend_children(AbstractSyntaxTree* current_node);
		};


	private:
		AbstractSyntaxTree* _execute(const Syntax& syntax, const AbstractSyntaxTree& current_input_node, MemoryContainer<AbstractSyntaxTree>& output_container);

		std::vector<std::pair<TreePatternMatching::Handle, ActionFunction>> _rules;

		enum SymbolAction {
			ADD,
			REMOVE
		};

		std::vector<std::tuple<const char*, bool, SymbolAction>> _symbol_transformation;
	};

}


#endif
