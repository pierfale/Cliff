#include "cliff/front_end/Parser.h"

using namespace cliff;

Parser::Parser(const Syntax& syntax) : _syntax(syntax) {

}

AbstractSyntaxTree& Parser::execute(const std::vector<Token>& input, MemoryContainer<AbstractSyntaxTree>& tree_memory) {

	unsigned int token_cursor = 0;
	std::stack<Syntax::State> state_stack;
	std::stack<AbstractSyntaxTree*> tree_stack;
	state_stack.push(Syntax::Parser_init_state);

	while(true) {
		if(state_stack.empty()) {
			//error
			std::cout << "end of stack" << std::endl;
			std::exit(0);
		}

		Syntax::State current_state = state_stack.top();

		Syntax::Index next_action = _syntax.next_parser_action(current_state, input[token_cursor].type());
		if(next_action == Syntax::Parser_unaccepting_state)
			THROW(exception::Exception, "Parser : Unrecognized input");

		if(next_action & Syntax::Parser_action_reduce_mask) {
			unsigned int n_child = _syntax.parser_reduce_number(current_state, input[token_cursor].type())
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

			TokenSymbol* tmp = new TokenSymbol("tmp...");
			if(next_action & Syntax::Parser_action_dummy_state_mask)
				current_tree = &tree_memory.emplace(tree_memory, *tmp);
			else if(next_action & (Syntax::Parser_action_skip_state_mask | Syntax::Parser_action_replace_state_mask)) {
				current_tree = tree_stack.top();
				tree_stack.pop();
				state_stack.pop();
			}
			else
				current_tree = &tree_memory.emplace(tree_memory, _syntax.parser_reduce_symbol(current_state, input[token_cursor].type()));

			if(next_action & Syntax::Parser_action_replace_state_mask) {
				current_tree->set(_syntax.parser_reduce_symbol(current_state, input[token_cursor].type()));
			}


			for(int i=n_child-1; i>=0; i--)
				current_tree->add_child(children[i]);

			tree_stack.push(current_tree);

			if(next_action & (Syntax::Parser_action_dummy_state_mask | Syntax::Parser_action_skip_state_mask))
				state_stack.push(_syntax.next_parser_dummy_goto(current_state, input[token_cursor].type(), state_stack.top()));
			else
				state_stack.push(_syntax.next_parser_goto(state_stack.top(), _syntax.parser_reduce_symbol(current_state, input[token_cursor].type())));
		}
		else if(next_action & Syntax::Parser_action_shift_mask) {
			if(next_action & Syntax::Parser_action_replace_state_mask) {
				std::cout << "shit/replace" << std::endl;
				AbstractSyntaxTree* current_tree = tree_stack.top();
				tree_stack.pop();
				state_stack.pop();

				for(AbstractSyntaxTree* child : current_tree->children()) {
					tree_stack.push(child);
					state_stack.push(child == current_tree->children().front() ? 0 : Syntax::Parser_unaccepting_state);
				}
			}

			AbstractSyntaxTree& current_tree = tree_memory.emplace(tree_memory, input[token_cursor]);
			tree_stack.push(&current_tree);
			token_cursor++;
			state_stack.push(next_action & Syntax::Parser_action_content_mask);

		}
		else if(next_action & Syntax::Parser_action_accept_mask) {
			return *tree_stack.top();
		}

	}
}
