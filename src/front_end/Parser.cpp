#include "cliff/front_end/Parser.h"

using namespace cliff;

Parser::Parser(const Syntax& syntax) : _syntax(syntax) {

}

AbstractSyntaxTree& Parser::execute(const std::vector<Token>& input, MemoryContainer<AbstractSyntaxTree>& tree_memory) {

	unsigned int token_cursor = 0;
	std::stack<Syntax::State> state_stack;
	std::stack<AbstractSyntaxTree*> tree_stack;
	state_stack.push(Syntax::Parser_init_state);

	TokenSymbol* tmp = new TokenSymbol("tmp");


	while(true) {
		if(state_stack.empty()) {
			//error
			std::cout << "end of stack" << std::endl;
			std::exit(0);
		}

		Syntax::State current_state = state_stack.top();

		std::cout << input[token_cursor].type().string() << "-" << current_state << std::endl;

		Syntax::Index next_action = _syntax.next_parser_action(current_state, input[token_cursor].type());
		if(next_action == Syntax::Parser_unaccepting_state)
			THROW(exception::Exception, "Parser : Unrecognized input");

		if(next_action & Syntax::Parser_action_reduce_mask) {

			unsigned int n_child = _syntax.parser_reduce_number(current_state, input[token_cursor].type());


			if(next_action & Syntax::Parser_action_dummy_state_mask) {
				const TokenSymbol& left_member = *tmp;
				AbstractSyntaxTree& current_tree = tree_memory.emplace(tree_memory, left_member);
				std::cout << "reduce " << left_member.string() << " of " << n_child << std::endl;
				std::vector<AbstractSyntaxTree*> children;
				children.reserve(n_child);
				for(unsigned int i=0; i<n_child; i++) {
					children.push_back(tree_stack.top());
					tree_stack.pop();
					state_stack.pop();
				}

				for(int i=n_child-1; i>=0; i--)
					current_tree.add_child(children[i]);

				tree_stack.push(&current_tree);
				state_stack.push(_syntax.next_parser_dummy_goto(current_state, input[token_cursor].type(), state_stack.top()));
			}
			else if(next_action & Syntax::Parser_action_skip_state_mask) {
				std::vector<AbstractSyntaxTree*> children;
				children.reserve(n_child-1);
				for(unsigned int i=0; i<n_child-1; i++) {
					children.push_back(tree_stack.top());
					tree_stack.pop();
					state_stack.pop();
				}
				AbstractSyntaxTree* current_tree = tree_stack.top();
				tree_stack.pop();
				state_stack.pop();

				for(int i=n_child-2; i>=0; i--)
					current_tree->add_child(children[i]);

				tree_stack.push(current_tree);
				state_stack.push(_syntax.next_parser_dummy_goto(current_state, input[token_cursor].type(), state_stack.top()));
			}
			else if(next_action & Syntax::Parser_action_replace_state_mask) {

				const TokenSymbol& left_member = _syntax.parser_reduce_symbol(current_state, input[token_cursor].type());
				std::vector<AbstractSyntaxTree*> children;
				children.reserve(n_child-1);
				for(unsigned int i=0; i<n_child-1; i++) {
					children.push_back(tree_stack.top());
					tree_stack.pop();
					state_stack.pop();
				}
				AbstractSyntaxTree* current_tree = tree_stack.top();
				current_tree->set(left_member);
				tree_stack.pop();
				state_stack.pop();

				for(int i=n_child-2; i>=0; i--)
					current_tree->add_child(children[i]);

				tree_stack.push(current_tree);
				state_stack.push(_syntax.next_parser_goto(state_stack.top(), left_member));
			}
			else {
				const TokenSymbol& left_member = _syntax.parser_reduce_symbol(current_state, input[token_cursor].type());
				AbstractSyntaxTree& current_tree = tree_memory.emplace(tree_memory, left_member);
				std::cout << "reduce " << left_member.string() << " of " << n_child << std::endl;
				std::vector<AbstractSyntaxTree*> children;
				children.reserve(n_child);
				for(unsigned int i=0; i<n_child; i++) {
					children.push_back(tree_stack.top());
					tree_stack.pop();
					state_stack.pop();
				}

				for(int i=n_child-1; i>=0; i--)
					current_tree.add_child(children[i]);

				tree_stack.push(&current_tree);
				std::cout << "goto " << state_stack.top() << ", " << left_member.string() << std::endl;
				state_stack.push(_syntax.next_parser_goto(state_stack.top(), left_member));
			}
/*
			if(false) {

				//TODO add end case : Flag on 'List El @' reduce -> change tree temporary to left member
				if(n_child == 1) {
					const TokenSymbol& left_member = *tmp;
					AbstractSyntaxTree& current_tree = tree_memory.emplace(tree_memory, left_member);
					std::cout << "reduce " << left_member.string() << " of " << n_child << std::endl;
					std::vector<AbstractSyntaxTree*> children;
					children.reserve(n_child);
					for(unsigned int i=0; i<n_child; i++) {
						children.push_back(tree_stack.top());
						tree_stack.pop();
						state_stack.pop();
					}

					for(int i=n_child-1; i>=0; i--)
						current_tree.add_child(children[i]);

					tree_stack.push(&current_tree);
				}
				else {
					std::vector<AbstractSyntaxTree*> children;
					children.reserve(n_child-1);
					for(unsigned int i=0; i<n_child-1; i++) {
						children.push_back(tree_stack.top());
						tree_stack.pop();
						state_stack.pop();
					}
					AbstractSyntaxTree* current_tree = tree_stack.top();
					tree_stack.pop();
					state_stack.pop();

					for(int i=n_child-2; i>=0; i--)
						current_tree->add_child(children[i]);

					tree_stack.push(current_tree);
				}
				state_stack.push(_syntax.next_parser_dummy_goto(current_state, input[token_cursor].type(), state_stack.top()));
			}
			else {
				const TokenSymbol& left_member = _syntax.parser_reduce_symbol(current_state, input[token_cursor].type());
				AbstractSyntaxTree& current_tree = tree_memory.emplace(tree_memory, left_member);
				std::cout << "reduce " << left_member.string() << " of " << n_child << std::endl;
				std::vector<AbstractSyntaxTree*> children;
				children.reserve(n_child);
				for(unsigned int i=0; i<n_child; i++) {
					children.push_back(tree_stack.top());
					tree_stack.pop();
					state_stack.pop();
				}

				for(int i=n_child-1; i>=0; i--)
					current_tree.add_child(children[i]);

				tree_stack.push(&current_tree);
				std::cout << "goto " << state_stack.top() << ", " << left_member.string() << std::endl;
				state_stack.push(_syntax.next_parser_goto(state_stack.top(), left_member));
			}
*/
		}
		else if(next_action & Syntax::Parser_action_shift_mask) {
			std::cout << "shift " << input[token_cursor].type().string() << " to " << (next_action & Syntax::Parser_action_content_mask) << std::endl;
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
