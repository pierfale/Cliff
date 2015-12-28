#ifndef _CLIFF_AUTOMATA_H
#define _CLIFF_AUTOMATA_H

#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>

#include "cliff/shared/MemoryContainer.h"
#include "cliff/shared/Token.h"

namespace cliff {

	class Automata {

	public:
		typedef int Symbol;
		static const Symbol Epsilon = -1;
	};

	template<typename Type>
	class AutomataNode {

	public:
		AutomataNode(MemoryContainer<AutomataNode<Type>>& memory_container) : _memory_container(memory_container) {

		}

		void create_output_transition_with(AutomataNode& node, Automata::Symbol symbol) {
			_transitions.insert(std::make_pair(symbol, &node));
		}

		void create_input_transition_with(AutomataNode& node, Automata::Symbol symbol) {
			node._transitions.insert(std::make_pair(symbol, this));
		}

		void change_transition(Automata::Symbol symbol, AutomataNode& node) {
			_transitions[symbol] = &node;
		}


		AutomataNode<Type>& create_output_node(Automata::Symbol symbol) {
			AutomataNode<Type>& node = create_node();
			create_output_transition_with(node, symbol);
			return node;
		}

		AutomataNode<Type>& create_input_node(Automata::Symbol symbol) {
			AutomataNode& node = create_node();
			create_input_transition_with(node, symbol);
			return node;
		}

		AutomataNode<Type>& create_node() {
			return _memory_container.template emplace<MemoryContainer<AutomataNode<Type>>&>(_memory_container);
		}

		const std::map<Automata::Symbol, AutomataNode<Type>*>& transitions() const {
			return _transitions;
		}

		void add_accepting_state(const TokenSymbol& symbol) {
			_accepting_state.push_back(&symbol);
		}

		bool is_accepting_state() const {
			return !_accepting_state.empty();
		}


		const std::vector<const TokenSymbol*>& accepting_state() const {
			return _accepting_state;
		}

		void print(std::ostream& stream) {
			std::vector<AutomataNode<Type>*> trace;
			_print(stream, trace);
		}
	private:
		void _print(std::ostream& stream, std::vector<AutomataNode<Type>*>& trace) {
			if(std::find(std::begin(trace), std::end(trace), this) != std::end(trace))
				return;

			trace.push_back(this);

			stream  << this << (is_accepting_state() ? " [final state]" : "") << " : " << std::endl;

			for(const auto& transition : _transitions) {
				std::string t_label;

				switch(transition.first) {

				case Automata::Epsilon:
					t_label = "Epsilon";
					break;
				default:
					t_label = (char)transition.first;
				}

				stream  << "--[" << t_label << "]-->" << transition.second << std::endl;
			}

			for(const auto& transition : _transitions) {
				transition.second->_print(stream, trace);
			}
		}

		MemoryContainer<AutomataNode<Type>>& _memory_container;
		std::map<Automata::Symbol, AutomataNode<Type>*> _transitions;
		std::vector<const TokenSymbol*> _accepting_state;

	};

	class DeterministeFiniteAutomata {};
	typedef AutomataNode<DeterministeFiniteAutomata> DeterministeFiniteAutomataNode;

	class NonDeterministeFiniteAutomata {};

	typedef AutomataNode<NonDeterministeFiniteAutomata> NonDeterministeFiniteAutomataNode;

	template<>
	class AutomataNode<NonDeterministeFiniteAutomata> {

	public:
		AutomataNode<NonDeterministeFiniteAutomata>(MemoryContainer<NonDeterministeFiniteAutomataNode>& memory_container) : _memory_container(memory_container) {

		}

		void create_output_transition_with(NonDeterministeFiniteAutomataNode& node, Automata::Symbol symbol) {
			_transitions.push_back(std::make_pair(symbol, &node));
		}

		void create_input_transition_with(NonDeterministeFiniteAutomataNode& node, Automata::Symbol symbol) {
			node._transitions.push_back(std::make_pair(symbol, this));
		}

		NonDeterministeFiniteAutomataNode& create_output_node(Automata::Symbol symbol) {
			NonDeterministeFiniteAutomataNode& node = create_node();
			create_output_transition_with(node, symbol);
			return node;
		}

		NonDeterministeFiniteAutomataNode& create_input_node(Automata::Symbol symbol) {
			AutomataNode& node = create_node();
			create_input_transition_with(node, symbol);
			return node;
		}


		NonDeterministeFiniteAutomataNode& create_node() {
			return _memory_container.emplace<MemoryContainer<NonDeterministeFiniteAutomataNode>&>(_memory_container);
		}

		const std::vector<std::pair<Automata::Symbol, NonDeterministeFiniteAutomataNode*>>& transitions() const {
			return _transitions;
		}


		void add_accepting_state(const TokenSymbol& symbol) {
			_accepting_state.push_back(&symbol);
		}

		bool is_accepting_state() const {
			return !_accepting_state.empty();
		}

		const std::vector<const TokenSymbol*>& accepting_state() const {
			return _accepting_state;
		}

		void print(std::ostream& stream) {
			std::vector<NonDeterministeFiniteAutomataNode*> trace;
			_print(stream, trace);
		}
	private:
		void _print(std::ostream& stream, std::vector<NonDeterministeFiniteAutomataNode*>& trace) {
			if(std::find(std::begin(trace), std::end(trace), this) != std::end(trace))
				return;

			trace.push_back(this);

			stream  << this << (is_accepting_state() ? " [final state]" : "") << " : " << std::endl;

			for(const auto& transition : _transitions) {
				std::string t_label;

				switch(transition.first) {

				case Automata::Epsilon:
					t_label = "Epsilon";
					break;
				default:
					t_label = (char)transition.first;
				}

				stream  << "--[" << t_label << "]-->" << transition.second << std::endl;
			}

			for(const auto& transition : _transitions) {
				transition.second->_print(stream, trace);
			}
		}
		MemoryContainer<NonDeterministeFiniteAutomataNode>& _memory_container;
		std::vector<std::pair<Automata::Symbol, NonDeterministeFiniteAutomataNode*>> _transitions;
		std::vector<const TokenSymbol*> _accepting_state;
	};
}
#endif
