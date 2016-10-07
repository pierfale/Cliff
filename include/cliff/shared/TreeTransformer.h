#ifndef _CLIFF_TREE_TRANSFORMER_H
#define _CLIFF_TREE_TRANSFORMER_H

#include "cliff/shared/AbstractSyntaxTree.h"
#include "cliff/shared/Exception.h"
#include "cliff/shared/TreeComparatorMapper.h"
#include "cliff/shared/TreePatternMatching.h"
#include "cliff/front_end/Syntax.h"

namespace cliff {

	class TreeTransformer {

		friend class Context;

	public:
		class Context {

			friend class TreeTransformer;

		public:
			Context(TreeTransformer& transformer, const AbstractSyntaxTree& input_node);

			const AbstractSyntaxTree& node() const;

			Context& child_at_position(unsigned int position);

			//
			//	Actions
			//


			void remove_recursive();
			void remove_and_ascend_children();
			void remove_all_children();

			void replace_node(const std::string& type);
			void replace_node(const std::string& type, const std::string& content);

		private:
			Token _create();

			TreeTransformer& _transformer;
			const AbstractSyntaxTree& _input_node;

			// output properties
			bool _to_be_deleted;

			std::string _new_symbol;

			bool _has_new_content;
			std::string _new_content;

			enum ContextAction {
				DELETE,
				ASCEND
			};

			std::vector<std::pair<const AbstractSyntaxTree*, ContextAction>> _children_actions;
		};

		typedef void(*ActionFunction)(TreeTransformer::Context&);

		TreeTransformer();

		void add_rule(TreePatternMatching::Handle filter, ActionFunction action);
		void add_symbol(const char* symbol_str, bool is_terminal);

		AbstractSyntaxTree& execute(const Syntax& input_syntax, Syntax& output_syntax, const AbstractSyntaxTree& input_tree, MemoryContainer<AbstractSyntaxTree>& output_container);

		//
		//	Implemented actions
		//
		struct Action {
			AbstractSyntaxTree* none(AbstractSyntaxTree* current_node);
			AbstractSyntaxTree* remove_recursive(AbstractSyntaxTree* current_node);
			AbstractSyntaxTree* ascend_children(AbstractSyntaxTree* current_node);
		};


	private:
		AbstractSyntaxTree& _execute(const Syntax& syntax, const AbstractSyntaxTree& current_input_node, AbstractSyntaxTree* parent_node, MemoryContainer<AbstractSyntaxTree>& output_container);

		std::vector<std::pair<TreePatternMatching::Handle, ActionFunction>> _rules;

		//
		//	Synbol transformations
		//
		enum SymbolAction {
			ADD,
			REMOVE
		};

		std::vector<std::tuple<const char*, bool, SymbolAction>> _symbol_transformation;

		//
		//	Delegated context taks
		//
		std::map<const AbstractSyntaxTree*, Context> _delegate_context;


	};

}


#endif
