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
		static LetterRange Epsilon_range;

		LetterRange();
		LetterRange(Letter unique_letter);
		LetterRange(Letter start_letter, Letter end_letter);
		LetterRange(const LetterRange& that);
		LetterRange(LetterRange&& that);

		const std::vector<std::pair<Letter, Letter>>& range() const;

		bool is_epsilon() const;
		bool is_in_range(Letter letter) const;


		void reset();
		void intersection(const LetterRange& that, LetterRange& output) const;

		LetterRange& operator-(const LetterRange& that);
		LetterRange& operator+(const LetterRange& that);
		LetterRange& operator^(const LetterRange& that);

		LetterRange& operator-=(const LetterRange& that);

		bool operator==(const LetterRange& that) const;
		bool operator!=(const LetterRange& that) const;
		bool operator<(const LetterRange& that) const;

		void to_letter_list(std::vector<Letter>& list) const;


	private:
		void reorder();

		std::vector<std::pair<Letter, Letter>> _range;

	};

	std::ostream& operator<<(std::ostream& stream, const LetterRange& that);

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

			stream  << &model << (model.is_accepting_state() ? " [accepting_state :" : "");
			for(const auto& state : model.accepting_state())
				stream << " \"" << state->string() << "\"";
			stream << (model.is_accepting_state() ? "]" : "") << std::endl;

			for(const auto& transition : model.transitions()) {
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

		void create_output_transition_with(AutomataNode& node, const LetterRange& range) {
			_transitions.emplace(std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(&node));
		}

		void create_input_transition_with(AutomataNode& node, const LetterRange& range) {
			node._transitions.emplace(std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(this));
		}

		void change_transition(const LetterRange& range, AutomataNode& node) {
			_transitions[range] = &node;
		}


		AutomataNode<Type>& create_output_node(const LetterRange& range) {
			AutomataNode<Type>& node = create_node();
			create_output_transition_with(node, range);
			return node;
		}

		AutomataNode<Type>& create_input_node(const LetterRange& range) {
			AutomataNode& node = create_node();
			create_input_transition_with(node, range);
			return node;
		}

		AutomataNode<Type>& create_node() {
			return _memory_container.emplace(_memory_container);
		}

		const std::map<LetterRange, AutomataNode<Type>*>& transitions() const {
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
		std::map<LetterRange, AutomataNode<Type>*> _transitions;
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

		void create_output_transition_with(NonDeterministeFiniteAutomataNode& node, const LetterRange& range) {
			_transitions.emplace_back(range, &node);
		}

		void create_input_transition_with(NonDeterministeFiniteAutomataNode& node, const LetterRange& range) {
			node._transitions.emplace_back(range, this);
		}

		NonDeterministeFiniteAutomataNode& create_output_node(const LetterRange& range) {
			NonDeterministeFiniteAutomataNode& node = create_node();
			create_output_transition_with(node, range);
			return node;
		}

		NonDeterministeFiniteAutomataNode& create_input_node(const LetterRange& range) {
			AutomataNode& node = create_node();
			create_input_transition_with(node, range);
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
