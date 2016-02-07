#include "cliff/tool/syntax_generator/SyntaxRepresentation.h"

using namespace cliff;

SyntaxRepresentation::SyntaxRepresentation(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& input_syntax) : _entry_rule(nullptr) {
	fill_regular_expression(ebnf_syntax, generated_syntax, input_syntax);
	construct_dummy_symbol(ebnf_syntax, generated_syntax, input_syntax);
	RawRuleList rule_list;
	_dummy_rule_cursor = 0;
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

bool SyntaxRepresentation::is_dummy_symbol_value(const TokenSymbol& symbol) const {
	return std::find(std::begin(_dummy_rule_name), std::end(_dummy_rule_name), symbol) != std::end(_dummy_rule_name);
}

const std::vector<TokenSymbol>& SyntaxRepresentation::dummy_rule_name() const {
	return _dummy_rule_name;
}

std::map<const TokenSymbol*, RegularExpressionRepresentation>& SyntaxRepresentation::regular_expression_list() {
	return _regular_expression_list;
}

const std::map<const TokenSymbol*, RegularExpressionRepresentation>& SyntaxRepresentation::regular_expression_list() const {
	return _regular_expression_list;
}


void SyntaxRepresentation::construct_dummy_symbol(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node) {
	if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_repetition")) {
		std::string rule_name = "%rule_dummy_"+std::to_string(_dummy_rule_name.size()+1);
		_dummy_rule_name.emplace_back(rule_name.c_str());
	}

	for(const AbstractSyntaxTree* child : current_node.children())
		construct_dummy_symbol(ebnf_syntax, generated_syntax, *child);
}

void SyntaxRepresentation::fill_regular_expression(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node) {
	if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule")) {
		if(current_node.children()[2]->type() == ebnf_syntax.get_symbol_from_name(SyntaxNodeName::RegularExpression)) {
			const TokenSymbol& name = generated_syntax.get_symbol_from_name(current_node.children()[0]->content());
			_regular_expression_list.emplace(std::piecewise_construct,
											 std::forward_as_tuple(&name),
											 std::forward_as_tuple(ebnf_syntax, *current_node.children()[2], name));
		}
	}
	else {
		for(const AbstractSyntaxTree* child : current_node.children())
			fill_regular_expression(ebnf_syntax, generated_syntax, *child);
	}
}

void SyntaxRepresentation::construct(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const AbstractSyntaxTree& current_node, RawRuleList& rule_list) {
	if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule")) {

		if(current_node.children()[2]->type() != ebnf_syntax.get_symbol_from_name("regular_expression")) {
			const TokenSymbol& rule_name = generated_syntax.get_symbol_from_name(current_node.children()[0]->content());

			if(_entry_rule == nullptr) { //first rule
				const TokenSymbol& root_symbol = generated_syntax.get_symbol_from_name(Syntax::Root_symbol);
				rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(&root_symbol), std::forward_as_tuple(RawSubRule::NonTerminal, &rule_name));
				_entry_rule = &root_symbol;
			}


			if(rule_list.find(&rule_name) != std::end(rule_list)) {
				THROW(exception::UserMessage, exception::UserMessage::Error, "Rule "+std::string(rule_name.string())+" already declared"); //TODO add line number/file
			}

			auto it_rule = rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(&rule_name), std::forward_as_tuple(RawSubRule::Alternative)).first;
			for(const AbstractSyntaxTree* child : current_node.children()[2]->children())
				construct_rule(ebnf_syntax, generated_syntax, rule_list, *child,  it_rule->second.root(), rule_name);
		}
	}
	else {
		for(const AbstractSyntaxTree* child : current_node.children())
			construct(ebnf_syntax, generated_syntax, *child, rule_list);
	}
}

void SyntaxRepresentation::construct_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, RawRuleList& rule_list, const AbstractSyntaxTree& current_node, RawSubRule& current_rule, const TokenSymbol& current_rule_name) {
	if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_alternative")) {
		RawSubRule& new_child = current_rule.add_child(RawSubRule::Alternative);
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, new_child, current_rule_name);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_optional")) {
		RawSubRule& new_child = current_rule.add_child(RawSubRule::OptionalSequence);
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, new_child, current_rule_name);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_sequence")) {
		RawSubRule& new_child = current_rule.add_child(RawSubRule::Sequence);
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, new_child, current_rule_name);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_repetition")) {
		const TokenSymbol& temporary_rule_symbol = _dummy_rule_name[_dummy_rule_cursor++];
		auto rule_it = rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(&temporary_rule_symbol), std::forward_as_tuple(RawSubRule::Alternative)).first;
		RawSubRule& element = rule_it->second.root().add_child(RawSubRule::Sequence, ListHead);
		for(const AbstractSyntaxTree* child : current_node.children())
			construct_rule(ebnf_syntax, generated_syntax, rule_list, *child, element, current_rule_name);

		RawSubRule& element2 = rule_it->second.root().add_child(RawSubRule::Sequence, ListTail);
		element2.add_child(RawSubRule::NonTerminal, &temporary_rule_symbol);

		RawSubRule& first_element = rule_it->second.root().list().front();

		std::copy(std::begin(first_element.list()), std::end(first_element.list()), std::back_inserter(element2.list()));

		current_rule.add_child(RawSubRule::NonTerminal, &temporary_rule_symbol, ListReduce);
	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_non_terminal")) {
		const TokenSymbol& symbol = generated_syntax.get_symbol_from_name(current_node.content());
		current_rule.add_child(_regular_expression_list.find(&symbol) == std::end(_regular_expression_list) ?
								   RawSubRule::NonTerminal : RawSubRule::Terminal, &symbol);


	}
	else if(current_node.type() == ebnf_syntax.get_symbol_from_name("rule_terminal")) {
		current_rule.add_child(RawSubRule::Terminal, &generated_syntax.get_symbol_from_name(current_node.content()));
	}
}

void SyntaxRepresentation::inline_rule(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawRuleList& rule_list) {
	for(const auto& rule : rule_list) {
		_rule_list.emplace(std::piecewise_construct, std::forward_as_tuple(rule.first), std::forward_as_tuple(*rule.first));
		auto it_rule = _rule_list.find(rule.first);
		std::vector<std::pair<std::vector<Symbol>, unsigned int>> rule_all_alternative;
		rule_all_alternative.emplace_back(); // start rule
		inline_rule_definition(ebnf_syntax, generated_syntax, rule.second.root(), rule_all_alternative);
		for(const std::pair<std::vector<Symbol>, unsigned int>& alternative : rule_all_alternative)
			it_rule->second.add_alternative(alternative.first, alternative.second);
	}
}

void SyntaxRepresentation::inline_rule_definition(const Syntax& ebnf_syntax, const Syntax& generated_syntax, const RawSubRule& current_input_rule, std::vector<std::pair<std::vector<Symbol>, unsigned int>>& alternative_list) {

	for(std::pair<std::vector<Symbol>, unsigned int>& alternative : alternative_list) {
		alternative.second |= current_input_rule.flags();
	}

	switch(current_input_rule.type()) {

	case RawSubRule::Terminal:
	case RawSubRule::NonTerminal:
		for(std::pair<std::vector<Symbol>, unsigned int>& alternative : alternative_list)
			alternative.first.emplace_back(current_input_rule.content(), current_input_rule.type() == RawSubRule::Terminal);
		break;
	case RawSubRule::OptionalSequence:
		{
			std::vector<std::pair<std::vector<Symbol>, unsigned int>> alternative_list_save(alternative_list);
			for(const RawSubRule& child : current_input_rule.list())
				inline_rule_definition(ebnf_syntax, generated_syntax, child, alternative_list);
			std::copy(std::begin(alternative_list_save), std::end(alternative_list_save), std::back_inserter(alternative_list));
			break;
		}
	case RawSubRule::RepetitionSequence:
	case RawSubRule::RepetitionNotEmptySequence:
	case RawSubRule::Sequence:
		for(const RawSubRule& child : current_input_rule.list())
			inline_rule_definition(ebnf_syntax, generated_syntax, child, alternative_list);
		break;
	case RawSubRule::Alternative:
		{
			std::vector<std::pair<std::vector<Symbol>, unsigned int>> alternative_list_save;
			std::swap(alternative_list, alternative_list_save);

			for(const RawSubRule& child : current_input_rule.list()) {
				std::vector<std::pair<std::vector<Symbol>, unsigned int>> current_alternative_list(alternative_list_save);
				inline_rule_definition(ebnf_syntax, generated_syntax, child, current_alternative_list);
				std::copy(std::begin(current_alternative_list), std::end(current_alternative_list), std::back_inserter(alternative_list));
			}
			break;
		}
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

SyntaxRepresentation::RawSubRule::RawSubRule(Type type, unsigned int flags) : _type(type), _flags(flags), _symbol(nullptr) {

}

SyntaxRepresentation::RawSubRule::RawSubRule(Type type, const TokenSymbol* symbol, unsigned int flags) : _type(type), _flags(flags), _symbol(symbol) {

}

SyntaxRepresentation::RawSubRule::RawSubRule(const RawSubRule& that) : _type(that._type), _flags(that._flags), _symbol(that._symbol), _rule_list(that._rule_list) {

}

SyntaxRepresentation::RawSubRule::RawSubRule(RawSubRule&& that) : _type(that._type), _flags(that._flags), _symbol(that._symbol), _rule_list(std::move(that._rule_list)){
	that._symbol = nullptr;
}

SyntaxRepresentation::RawSubRule& SyntaxRepresentation::RawSubRule::add_child(Type type, unsigned int flags) {
	_rule_list.emplace_back(type, flags);
	return _rule_list.back();
}

SyntaxRepresentation::RawSubRule& SyntaxRepresentation::RawSubRule::add_child(Type type, const TokenSymbol* symbol, unsigned int flags) {
	_rule_list.emplace_back(type, symbol, flags);
	return _rule_list.back();
}

SyntaxRepresentation::RawSubRule& SyntaxRepresentation::RawSubRule::add_child(const RawSubRule& that) {
	_rule_list.emplace_back(that);
	return _rule_list.back();
}

unsigned int SyntaxRepresentation::RawSubRule::flags() const {
	return _flags;
}

SyntaxRepresentation::RawSubRule::Type SyntaxRepresentation::RawSubRule::type() const {
	return _type;
}
const TokenSymbol& SyntaxRepresentation::RawSubRule::content() const {
	return *_symbol;
}

std::vector<SyntaxRepresentation::RawSubRule>& SyntaxRepresentation::RawSubRule::list() {
	return _rule_list;
}

const std::vector<SyntaxRepresentation::RawSubRule>& SyntaxRepresentation::RawSubRule::list() const {
	return _rule_list;
}

//
//	RawRule
//

SyntaxRepresentation::RawRule::RawRule(RawSubRule::Type root_type, unsigned int flags)
	: _root(root_type, flags), _ignore_sequences(RawSubRule::Alternative) {

}

SyntaxRepresentation::RawRule::RawRule(RawSubRule::Type root_type, const TokenSymbol* symbol, unsigned int flags)
	: _root(root_type, symbol, flags), _ignore_sequences(RawSubRule::Alternative) {

}

SyntaxRepresentation::RawSubRule& SyntaxRepresentation::RawRule::root() {
	return _root;
}

const SyntaxRepresentation::RawSubRule& SyntaxRepresentation::RawRule::root() const {
	return _root;
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

SyntaxRepresentation::InlinedAlternative::InlinedAlternative(const TokenSymbol& _rule_name, const std::vector<Symbol>& rule_definition, unsigned int flags) : _flags(flags), _rule_name(_rule_name), _sequence(rule_definition) {

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
}

unsigned int SyntaxRepresentation::InlinedAlternative::flags() const {
	return _flags;
}

const std::vector<SyntaxRepresentation::Symbol>& SyntaxRepresentation::InlinedAlternative::sequence() const {
	return _sequence;
}

const TokenSymbol& SyntaxRepresentation::InlinedAlternative::rule_name() const {
	return _rule_name;
}

//
//	Rule
//

SyntaxRepresentation::Rule::Rule(const TokenSymbol& rule_name) : _rule_name(rule_name) {

}

void SyntaxRepresentation::Rule::add_alternative(const std::vector<Symbol>& rule_definition, unsigned int flags) {
	_alternative_list.emplace_back(_rule_name, rule_definition, flags);
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

const TokenSymbol& SyntaxRepresentation::Rule::rule_name() const {
	return _rule_name;
}


const std::vector<SyntaxRepresentation::InlinedAlternative>& SyntaxRepresentation::Rule::alternatives() const {
	return _alternative_list;
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
