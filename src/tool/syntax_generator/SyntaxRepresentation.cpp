#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"

using namespace cliff;

SyntaxRepresentation::SyntaxRepresentation(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& input_syntax) : _entry_rule(nullptr) {
	RawRuleList rule_list;
	construct(ebnf_syntax, generated_syntax, input_syntax, rule_list);

	inline_rule(ebnf_syntax, generated_syntax, rule_list);
}

SyntaxRepresentation::Rule& SyntaxRepresentation::get_rule_by_symbol(const TokenSymbol& symbol) {
	auto it = _rule_list.find(&symbol);
	if(it == std::end(_rule_list)) {
		THROW(exception::ElementNotFound, symbol.string());
	}
	else
		return it->second;
}

const SyntaxRepresentation::Rule& SyntaxRepresentation::get_rule_by_symbol(const TokenSymbol& symbol) const {
	auto it = _rule_list.find(&symbol);
	if(it == std::end(_rule_list)) {
		THROW(exception::ElementNotFound, symbol.string());
	}
	else
		return it->second;
}

void SyntaxRepresentation::construct(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node, RawRuleList& rule_list) {
	if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule")) {
		const TokenSymbol& rule_name = generated_syntax.get_symbol_from_name(current_node.children()[0]->content());

		if(_entry_rule == nullptr) { //first rule
			const TokenSymbol& root_symbol = generated_syntax.get_symbol_from_name(Syntax::Root_symbol);
			rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(&root_symbol), std::forward_as_tuple( RuleDefinition::NonTerminal, &rule_name));
			_entry_rule = &root_symbol;
		}


		if(rule_list.find(&rule_name) != std::end(rule_list)) {
			THROW(exception::UserMessage, exception::UserMessage::Error, "Rule "+std::string(rule_name.string())+" already declared"); //TODO add line number/file
		}
		auto it_rule = rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(&rule_name), std::forward_as_tuple(RuleDefinition::Alternative)).first;
		for(const AbstractSyntaxTree* child : current_node.children()[2]->children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child,  it_rule->second);
	}
	else {
		for(const AbstractSyntaxTree* child : current_node.children())
			construct(ebnf_syntax, generated_syntax, *child, rule_list);
	}
}

void SyntaxRepresentation::construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, RawRuleList& rule_list, const AbstractSyntaxTree& current_node, RuleDefinition& current_rule) {
	if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_alternative")) {
		RuleDefinition& new_child = current_rule.add_child(RuleDefinition::Alternative);
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, new_child);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_optional")) {
		RuleDefinition& new_child = current_rule.add_child(RuleDefinition::OptionalSequence);
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, new_child);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_sequence")) {
		RuleDefinition& new_child = current_rule.add_child(RuleDefinition::Sequence);
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, new_child);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_repetition")) {
		std::string rule_name = "rule_temporary_"+std::to_string(temporary_rule_name.size()+1);
		temporary_rule_name.emplace_back<const char*>(rule_name.c_str());
		auto rule_it = rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(&temporary_rule_name.back()), std::forward_as_tuple(RuleDefinition::Alternative, true)).first;
		current_rule.add_child(RuleDefinition::NonTerminal, &temporary_rule_name.back());
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, rule_it->second);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_non_terminal")) {
		current_rule.add_child(RuleDefinition::NonTerminal, &generated_syntax.get_symbol_from_name(current_node.content()));
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_terminal")) {
		current_rule.add_child(RuleDefinition::Terminal, &generated_syntax.get_symbol_from_name(current_node.content()));
	}
}

void SyntaxRepresentation::inline_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawRuleList& rule_list) {
	for(const auto& rule : rule_list) {
		_rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(rule.first), std::forward_as_tuple(*rule.first, rule.second.unbound_repetition()));
		auto it_rule = _rule_list.find(rule.first);
		std::vector<std::vector<Symbol>> rule_all_alternative;
		rule_all_alternative.emplace_back(); // start rule
		inline_rule_definition(ebnf_syntax, generated_syntax, rule.second, rule_all_alternative);
		for(const std::vector<Symbol>& alternative : rule_all_alternative)
			it_rule->second.add_alternative(alternative);
	}
}

void SyntaxRepresentation::inline_rule_definition(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RuleDefinition& current_input_rule, std::vector<std::vector<Symbol>>& alternative_list) {
	switch(current_input_rule.type()) {

	case RuleDefinition::Terminal:
	case RuleDefinition::NonTerminal:
		for(std::vector<Symbol>& alternative : alternative_list)
			alternative.emplace_back(current_input_rule.content(), current_input_rule.type() == RuleDefinition::Terminal);
		break;
	case RuleDefinition::RepetitionSequence:
	case RuleDefinition::RepetitionNotEmptySequence:
	case RuleDefinition::OptionalSequence:
	case RuleDefinition::Sequence:
		for(const RuleDefinition& child : current_input_rule.list())
			inline_rule_definition(ebnf_syntax, generated_syntax, child, alternative_list);
		break;
	case RuleDefinition::Alternative:
		std::vector<std::vector<Symbol>> alternative_list_save;
		std::swap(alternative_list, alternative_list_save);

		for(const RuleDefinition& child : current_input_rule.list()) {
			std::vector<std::vector<Symbol>> current_alternative_list(alternative_list_save);
			inline_rule_definition(ebnf_syntax, generated_syntax, child, current_alternative_list);
			std::copy(std::begin(current_alternative_list), std::end(current_alternative_list), std::back_inserter(alternative_list));
		}
		break;
	}

}


void SyntaxRepresentation::print(std::ostream& stream) const {
	for(const auto& rule : _rule_list) {
		if(rule.first == _entry_rule)
			stream << "Entry ";
		rule.second.print(stream);
		stream << std::endl;
	}
}

//
//	RuleDefinition
//

SyntaxRepresentation::RuleDefinition::RuleDefinition(Type type, bool unbound_repetition) : _unbound_repetition(unbound_repetition), _type(type), _symbol(nullptr) {

}

SyntaxRepresentation::RuleDefinition::RuleDefinition(Type type, const TokenSymbol* symbol, bool unbound_repetition) : _unbound_repetition(unbound_repetition), _type(type), _symbol(symbol) {

}

SyntaxRepresentation::RuleDefinition::RuleDefinition(RuleDefinition&& that) : _type(that._type), _symbol(that._symbol), _rule_list(std::move(that._rule_list)){
	that._symbol = nullptr;
}

SyntaxRepresentation::RuleDefinition& SyntaxRepresentation::RuleDefinition::add_child(Type type) {
	_rule_list.emplace_back(type);
	return _rule_list.back();
}

SyntaxRepresentation::RuleDefinition& SyntaxRepresentation::RuleDefinition::add_child(Type type, const TokenSymbol* symbol) {
	_rule_list.emplace_back(type, symbol);
	return _rule_list.back();
}

bool SyntaxRepresentation::RuleDefinition::is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int start_position, bool expend_non_terminal) const {
	std::map<const TokenSymbol*, bool> history;
	return _is_epsilon_productive(syntax_representation, start_position, expend_non_terminal, history);
}

bool SyntaxRepresentation::RuleDefinition::_is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int start_position, bool expend_non_terminal, std::map<const TokenSymbol*, bool>& history) const {
	switch(_type) {

	case Terminal:
		return start_position > 0;
	case NonTerminal:
		if(expend_non_terminal) {
			auto it_history = history.find(_symbol);
			if(it_history == std::end(history)) {
				history.insert(std::make_pair(_symbol, false));
				bool eps_production = syntax_representation.get_rule_by_symbol(*_symbol).root_rule_definition()._is_epsilon_productive(syntax_representation, 0, expend_non_terminal, history);
				history[_symbol] = eps_production;
				return eps_production;
			}
			return it_history->second;
		}
		else
			return start_position > 0;
	case Sequence:
	case RepetitionNotEmptySequence:
		for(const RuleDefinition& child : _rule_list) {
			if(!child._is_epsilon_productive(syntax_representation, 0, expend_non_terminal, history))
				return false;
		}
	case Alternative:
		for(const RuleDefinition& child : _rule_list)
			if(child._is_epsilon_productive(syntax_representation, 0, expend_non_terminal, history))
				return true;
	case OptionalSequence:
		return true;
	case RepetitionSequence:
		return true;

	}
}

void SyntaxRepresentation::RuleDefinition::next_definition_of_type_list(const SyntaxRepresentation& syntax_representation, unsigned int cursor, Type type, std::vector<const RuleDefinition*>& output) const {
	switch(_type) {

	case Terminal:
		if(type == Terminal && cursor == 0)
			output.push_back(this);
		break;
	case NonTerminal:
		if(type == NonTerminal && cursor == 0)
			output.push_back(this);
		break;
	case Sequence:
	case OptionalSequence:
	case RepetitionSequence:
	case RepetitionNotEmptySequence:
		if(type == _type && cursor == 0)
			output.push_back(this);
		else {
			while(cursor < _rule_list.size()) {
				_rule_list[cursor].next_definition_of_type_list(syntax_representation, 0, type, output);
				if(_rule_list[cursor].is_epsilon_productive(syntax_representation, false)) {
					cursor++;
				}
				else
					break;
			}
		}
		break;

	case Alternative:
		if(type == Alternative && cursor == 0)
			output.push_back(this);
		else if(cursor < _rule_list.size()) {
			for(const RuleDefinition& child : _rule_list)
				child.next_definition_of_type_list(syntax_representation, cursor, type, output);
		}
		break;

	}
}

bool SyntaxRepresentation::RuleDefinition::first_list(const SyntaxRepresentation& syntax_representation, unsigned int cursor, std::vector<const TokenSymbol*>& output) const {
	std::map<const TokenSymbol*, bool> history;
	_first_list(syntax_representation, cursor, output, history);
}

bool SyntaxRepresentation::RuleDefinition::_first_list(const SyntaxRepresentation& syntax_representation, unsigned int cursor, std::vector<const TokenSymbol*>& output, std::map<const TokenSymbol*, bool>& history) const {

	switch(_type) {

	case Terminal:
		output.push_back(_symbol);
		return false;
	case NonTerminal:
		{
			auto it_history = history.find(_symbol);
			if(it_history == std::end(history)) {
				history.insert(std::make_pair(_symbol, false));
				bool eps_production = syntax_representation.get_rule_by_symbol(*_symbol).root_rule_definition()._first_list(syntax_representation, 0, output, history);
				history[_symbol] = eps_production;
				return eps_production;
			}
			return it_history->second;
		}
	case Sequence:
		do {
			if(cursor >= _rule_list.size())
				return true;

			if(!_rule_list[cursor]._first_list(syntax_representation, 0, output, history))
				return false;

		} while(true);
	case Alternative:
		{
			bool eps_production = false;
			for(const RuleDefinition& child : _rule_list)
				eps_production = child._first_list(syntax_representation, cursor, output, history) | eps_production;
			return eps_production;

		}
	case OptionalSequence:
		_rule_list[cursor]._first_list(syntax_representation, 0, output, history);
		return true;
	case RepetitionSequence:
		_rule_list[cursor]._first_list(syntax_representation, 0, output, history);
		return true;
	case RepetitionNotEmptySequence:
		_rule_list[cursor]._first_list(syntax_representation, 0, output, history);
		return false;

	}
}

std::vector<SyntaxRepresentation::RuleDefinition>& SyntaxRepresentation::RuleDefinition::list() {
	return _rule_list;
}

const std::vector<SyntaxRepresentation::RuleDefinition>& SyntaxRepresentation::RuleDefinition::list() const {
	return _rule_list;
}

bool SyntaxRepresentation::RuleDefinition::unbound_repetition() const {
	return _unbound_repetition;
}

SyntaxRepresentation::RuleDefinition::Type SyntaxRepresentation::RuleDefinition::type() const {
	return _type;
}
const TokenSymbol& SyntaxRepresentation::RuleDefinition::content() const {
	return *_symbol;
}

void SyntaxRepresentation::RuleDefinition::print(std::ostream& stream) const {
	switch(_type) {
	case Terminal:
		stream << "\"" << _symbol->string() << "\"";
		break;
	case NonTerminal:
		stream << _symbol->string();
		break;
	case Sequence:
		for(const RuleDefinition& child : _rule_list) {
			child.print(stream);
			stream << " ";
		}
		break;
	case Alternative:
		for(const RuleDefinition& child : _rule_list) {
			child.print(stream);
			stream << "| ";
		}
		break;
	case OptionalSequence:
		stream << "[ ";
		for(const RuleDefinition& child : _rule_list) {
			child.print(stream);
			stream << " ";
		}
		stream << "] ";
		break;
	case RepetitionSequence:
		stream << "[ ";
		for(const RuleDefinition& child : _rule_list) {
			child.print(stream);
			stream << " ";
		}
		stream << "]* ";
		break;
	case RepetitionNotEmptySequence:
		stream << "[ ";
		for(const RuleDefinition& child : _rule_list) {
			child.print(stream);
			stream << " ";
		}
		stream << "]+ ";
		break;
	}
}

//
//	Symbol
//

SyntaxRepresentation::Symbol::Symbol(const TokenSymbol& content, bool is_terminal) : _content(content), _is_terminal(is_terminal) {

}

bool SyntaxRepresentation::Symbol::is_terminal() const {
	return _is_terminal;
}

const TokenSymbol& SyntaxRepresentation::Symbol::content() const {
	return _content;
}

//
//	InlinedAlternative
//

SyntaxRepresentation::InlinedAlternative::InlinedAlternative(const TokenSymbol& _rule_name, const std::vector<Symbol>& rule_definition, bool unbound_repetition) : _rule_name(_rule_name), _unbound_repetition(unbound_repetition), _sequence(rule_definition) {

}

void SyntaxRepresentation::InlinedAlternative::first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output) const {
	std::vector<const TokenSymbol*> history;
	_first_after(syntax_representation, position, output, history);
}

void SyntaxRepresentation::InlinedAlternative::_first_after(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const {
	if(position < _sequence.size()) {
		if(_sequence[position].is_terminal())
			output.push_back(&_sequence[position].content());
		else
			syntax_representation.get_rule_by_symbol(_sequence[position].content())._first(syntax_representation, output, history);
	}

	if(position > 0) {
		const Rule& rule = syntax_representation.get_rule_by_symbol(_sequence[position-1].content());
		if(rule.unbound_repetition()) {
			rule._first(syntax_representation, output, history);
		}
	}

}

bool SyntaxRepresentation::InlinedAlternative::is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position) const {
	std::vector<const TokenSymbol*> history;
	_is_epsilon_productive(syntax_representation, position, history);
}

bool SyntaxRepresentation::InlinedAlternative::_is_epsilon_productive(const SyntaxRepresentation& syntax_representation, unsigned int position, std::vector<const TokenSymbol*>& history) const {
	if(position < _sequence.size()) {
		if(_sequence[position].is_terminal())
			return false;
		else
			return syntax_representation.get_rule_by_symbol(_sequence[position].content())._is_epsilon_productive(syntax_representation, history);
	}
	else if(position == _sequence.size() && _unbound_repetition) {
		return true;
	}
}

bool SyntaxRepresentation::InlinedAlternative::unbound_repetition() const {
	return _unbound_repetition;
}

const std::vector<SyntaxRepresentation::Symbol>& SyntaxRepresentation::InlinedAlternative::sequence() const {
	return _sequence;
}

const TokenSymbol& SyntaxRepresentation::InlinedAlternative::rule_name() const {
	return _rule_name;
}

bool SyntaxRepresentation::InlinedAlternative::has_unbound_repetition() const {
	return _unbound_repetition;
}
//
//	Rule
//

SyntaxRepresentation::Rule::Rule(const TokenSymbol& rule_name, bool unbound_repetition) : _unbound_repetition(unbound_repetition), _rule_name(rule_name), _root_rule_definition(SyntaxRepresentation::RuleDefinition::Sequence) {

}

SyntaxRepresentation::Rule::Rule(const TokenSymbol& rule_name, RuleDefinition::Type root_type, bool unbound_repetition) : _unbound_repetition(unbound_repetition), _rule_name(rule_name), _root_rule_definition(root_type) {

}

SyntaxRepresentation::Rule::Rule(const TokenSymbol& rule_name, RuleDefinition::Type root_type, const TokenSymbol* root_symbol, bool unbound_repetition) : _unbound_repetition(unbound_repetition), _rule_name(rule_name), _root_rule_definition(root_type, root_symbol) {

}

void SyntaxRepresentation::Rule::add_alternative(const std::vector<Symbol>& rule_definition) {
	_alternative_list.emplace_back(_rule_name, rule_definition, _unbound_repetition);
}

void SyntaxRepresentation::Rule::first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output) const {
	std::vector<const TokenSymbol*> history;
	_first(syntax_representation, output, history);
}

void SyntaxRepresentation::Rule::_first(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& output, std::vector<const TokenSymbol*>& history) const {
	if(std::find(std::begin(history), std::end(history), &_rule_name) != std::end(history))
		return;

	history.push_back(&_rule_name);

	for(const InlinedAlternative& alternative : _alternative_list)
		alternative._first_after(syntax_representation, 0, output, history);
}

bool SyntaxRepresentation::Rule::is_epsilon_productive(const SyntaxRepresentation& syntax_representation) const {
	std::vector<const TokenSymbol*> history;
	_is_epsilon_productive(syntax_representation, history);
}

bool SyntaxRepresentation::Rule::_is_epsilon_productive(const SyntaxRepresentation& syntax_representation, std::vector<const TokenSymbol*>& history) const {
	if(std::find(std::begin(history), std::end(history), &_rule_name) != std::end(history))
		return false;

	history.push_back(&_rule_name);

	for(const InlinedAlternative& alternative : _alternative_list)
		if(alternative._is_epsilon_productive(syntax_representation, 0, history));
			return true;
	return false;
}

bool SyntaxRepresentation::Rule::unbound_repetition() const {
	return _unbound_repetition;
}

const TokenSymbol& SyntaxRepresentation::Rule::rule_name() const {
	return _rule_name;
}

const std::vector<SyntaxRepresentation::InlinedAlternative>& SyntaxRepresentation::Rule::alternatives() const {
	return _alternative_list;
}

SyntaxRepresentation::RuleDefinition& SyntaxRepresentation::Rule::root_rule_definition() {
	return _root_rule_definition;
}

const SyntaxRepresentation::RuleDefinition& SyntaxRepresentation::Rule::root_rule_definition() const {
	return _root_rule_definition;
}

void SyntaxRepresentation::Rule::print(std::ostream& stream) const {
	bool first = true;
	for(const auto& alternative : _alternative_list) {
		if(first) {
			first = false;
			stream << _rule_name.string() << " :=";
		}
		else {
			stream << std::string(_rule_name.size()+3, ' ');
		}

		for(const Symbol& symbol : alternative.sequence())
			stream << " " << symbol.content().string();
		stream << std::endl;
	}
}
