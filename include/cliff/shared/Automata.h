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

	typedef uint32_t Letter;

	class LetterRange {

	public:
		static const Letter Min_letter = 0x0;
		static const Letter Max_letter = 0xFFFFFFFF;

		LetterRange();
		LetterRange(Letter unique_letter);
		LetterRange(Letter start_letter, Letter end_letter);

		Letter start_range() const;
		Letter end_range() const;

		bool is_epsilon() const;


		LetterRange& operator-(const LetterRange& that);
		LetterRange& operator+(const LetterRange& that);
		LetterRange& operator^(const LetterRange& that);

	private:
		Letter _start_range;
		Letter _end_range;
	};

	std::ostream& operator<<(std::ostream& stream, const LetterRange& that) {
		if(that.is_epsilon())
			stream << "Epsilon";
		else if(that.start_range() == that.end_range())
			stream << (char)that.start_range();
		else
			stream << (char)that.start_range() << "-" << (char)that.end_range();
		return stream;
	}

	class Automata {

	public:
		typedef int Symbol;
		static const Symbol Epsilon = -1;
	};

	class AutomaUtility {

	public:
		template<typename Automata>
		static void print(Automata& model, std::ostream& stream) {
			std::vector<Automata*> trace;
			_print(model, stream, trace);
		}

	private:
		template<typename Automata>
		static void _print(Automata& model, std::ostream& stream, std::vector<Automata*>& trace) {
			if(std::find(std::begin(trace), std::end(trace), &model) != std::end(trace))
				return;

			trace.push_back(&model);

			stream  << &model << (model.is_accepting_state() ? " [final state]" : "") << " : " << std::endl;

			for(const auto& transition : model.transitions()) {
				std::string t_label;
/*
				switch(transition.first) {

				case Automata::Epsilon:
					t_label = "Epsilon";
					break;
				default:
					t_label = (char)transition.first;
				}*/



				stream  << "--[" << transition.first << "]-->" << transition.second << std::endl;
			}

			for(const auto& transition : model.transitions()) {
				_print(*transition.second, stream, trace);
			}
		}


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
			return _memory_container.emplace(_memory_container);
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
			AutomaUtility::print(*this, stream);
		}
	private:
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

		void create_output_transition_with(NonDeterministeFiniteAutomataNode& node, LetterRange range) {
			_transitions.push_back(std::make_pair(range, &node));
		}

		void create_input_transition_with(NonDeterministeFiniteAutomataNode& node, Automata::Symbol symbol) {
			node._transitions.push_back(std::make_pair(symbol, this));
		}

		NonDeterministeFiniteAutomataNode& create_output_node(LetterRange range) {
			NonDeterministeFiniteAutomataNode& node = create_node();
			create_output_transition_with(node, range);
			return node;
		}

		NonDeterministeFiniteAutomataNode& create_input_node(Automata::Symbol symbol) {
			AutomataNode& node = create_node();
			create_input_transition_with(node, symbol);
			return node;
		}


		NonDeterministeFiniteAutomataNode& create_node() {
			return _memory_container.emplace(_memory_container);
		}

		const std::vector<std::pair<LetterRange, NonDeterministeFiniteAutomataNode*>>& transitions() const {
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
			AutomaUtility::print(*this, stream);
		}
	private:
		MemoryContainer<NonDeterministeFiniteAutomataNode>& _memory_container;
		std::vector<std::pair<LetterRange, NonDeterministeFiniteAutomataNode*>> _transitions;
		std::vector<const TokenSymbol*> _accepting_state;
	};
}
#endif
