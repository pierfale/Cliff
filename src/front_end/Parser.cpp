#include "cliff/front_end/Parser.h"

using namespace cliff;

Parser::Parser(const Syntax& syntax, Lexer& input) :  _syntax(syntax), _input(input) {

}

AbstractSyntaxTree& Parser::execute(MemoryContainer<AbstractSyntaxTree>& tree_memory) {

	std::stack<Syntax::State> state_stack;
	std::stack<AbstractSyntaxTree*> tree_stack;
	state_stack.push(Syntax::Parser_init_state);

	TokenSymbol dummy_symbol("");

	Token current_input;
	_input.process_next(Syntax::Parser_init_state, current_input);

	while(true) {
		if(state_stack.empty()) {
			//error
			std::cout << "end of stack" << std::endl;
		}

		Syntax::State current_state = state_stack.top();

		Syntax::Index next_action = _syntax.next_parser_action(current_state, current_input.type());
        if(next_action == Syntax::Parser_unaccepting_state) {
			THROW(exception::Exception, "Parser : Unrecognized input");
        }

		if(next_action & Syntax::Parser_action_reduce_mask) {
			unsigned int n_child = _syntax.parser_reduce_number(current_state, current_input.type())
					-(next_action & (Syntax::Parser_action_skip_state_mask | Syntax::Parser_action_replace_state_mask) ? 1 : 0) ;

			std::vector<AbstractSyntaxTree*> children;
			children.reserve(n_child);
			for(unsigned int i=0; i<n_child; i++) {
				children.push_back(tree_stack.top());
				tree_stack.pop();

				if(state_stack.top() == Syntax::Parser_unaccepting_state)
					n_child++;

				state_stack.pop();
			}

			AbstractSyntaxTree* current_tree = nullptr;


			if(next_action & Syntax::Parser_action_dummy_state_mask)
				current_tree = &tree_memory.emplace(tree_memory, dummy_symbol);
			else if(next_action & (Syntax::Parser_action_skip_state_mask | Syntax::Parser_action_replace_state_mask)) {
				current_tree = tree_stack.top();
				tree_stack.pop();
				state_stack.pop();
			}
			else
				current_tree = &tree_memory.emplace(tree_memory, _syntax.parser_reduce_symbol(current_state, current_input.type()));

			if(next_action & Syntax::Parser_action_replace_state_mask) {
				current_tree->set(_syntax.parser_reduce_symbol(current_state, current_input.type()));
			}


			for(int i=n_child-1; i>=0; i--)
				current_tree->add_child(children[i]);

			tree_stack.push(current_tree);

			if(next_action & (Syntax::Parser_action_dummy_state_mask | Syntax::Parser_action_skip_state_mask))
				state_stack.push(_syntax.next_parser_dummy_goto(current_state, current_input.type(), state_stack.top()));
			else
				state_stack.push(_syntax.next_parser_goto(state_stack.top(), _syntax.parser_reduce_symbol(current_state, current_input.type())));
		}
		else if(next_action & Syntax::Parser_action_shift_mask) {
			if(next_action & Syntax::Parser_action_replace_state_mask) {
				AbstractSyntaxTree* current_tree = tree_stack.top();
				tree_stack.pop();
				state_stack.pop();

				for(AbstractSyntaxTree* child : current_tree->children()) {
					tree_stack.push(child);
					state_stack.push(child == current_tree->children().front() ? 0 : Syntax::Parser_unaccepting_state);
				}
			}

			AbstractSyntaxTree& current_tree = tree_memory.emplace(tree_memory, current_input);
			tree_stack.push(&current_tree);
			state_stack.push(next_action & Syntax::Parser_action_content_mask);
			_input.process_next(state_stack.top(), current_input);

		}
		else if(next_action & Syntax::Parser_action_accept_mask) {
			return *tree_stack.top();
		}

	}
}
