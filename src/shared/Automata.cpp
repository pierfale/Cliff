#include "cliff/shared/Automata.h"

using namespace cliff;


//
//	LetterRange
//

LetterRange LetterRange::Epsilon_range;

LetterRange::LetterRange() : _range() {

}

LetterRange::LetterRange(Letter unique_letter) : _range() {
	_range.emplace_back(unique_letter, unique_letter);
}

LetterRange::LetterRange(Letter start_letter, Letter end_letter) : _range() {
	_range.emplace_back(start_letter, end_letter);
}

LetterRange::LetterRange(const LetterRange& that) : _range(that._range) {

}

LetterRange::LetterRange(LetterRange&& that) : _range(std::move(that._range)) {

}

const std::vector<std::pair<Letter, Letter>>& LetterRange::range() const {
	return _range;
}

bool LetterRange::is_epsilon() const {
	return _range.empty();
}

bool LetterRange::is_in_range(Letter letter) const {
	for(const auto& range : _range)
		if(range.first >= letter && range.second <= letter)
			return true;
	return false;
}

void LetterRange::reset() {
	_range.clear();
}

void LetterRange::intersection(const LetterRange& that, LetterRange& output) const {
	output.reset();

	auto this_it = std::begin(_range);
	auto that_it = std::begin(that._range);

	while(this_it != std::end(_range) && that_it != std::end(that._range)) {
		if(this_it->second < that_it->first) {
			++this_it;
		}
		else if(that_it->second < this_it->first) {
			++that_it;
		}
		else {
			output._range.emplace_back(std::max(this_it->first, that_it->first), std::min(this_it->second, that_it->second));

			if(this_it->second < that_it->second) {
				++this_it;
			}
			else {
				++that_it;
			}
		}
	}
}

LetterRange& LetterRange::operator-(const LetterRange& that) {
	return *this;
}

LetterRange& LetterRange::operator+(const LetterRange& that) {
	return *this;
}

LetterRange& LetterRange::operator^(const LetterRange& that) {
	return *this;
}

LetterRange& LetterRange::operator-=(const LetterRange& that) {
	for(const auto& that_range : that.range()) {
		for(unsigned int range_cursor=0; range_cursor < _range.size(); range_cursor++) {
			if(that_range.first > _range[range_cursor].first && that_range.second < _range[range_cursor].second) { // that inner this
				_range.emplace_back(that_range.second+1, _range[range_cursor].second);
				_range[range_cursor].second = that_range.first-1;
			}
			else if(that_range.first <= _range[range_cursor].first && that_range.second >= _range[range_cursor].second) { // this inner that
				_range.erase(std::begin(_range)+range_cursor);
				range_cursor--;
			}
			else if(that_range.first <= _range[range_cursor].first && that_range.second >= _range[range_cursor].first) { // this.first inner that
				_range[range_cursor].first = that_range.second+1;
			}
			else if(that_range.first <= _range[range_cursor].second && that_range.second >= _range[range_cursor].second) { // this.second inner that
				_range[range_cursor].second = that_range.first-1;
			}
		}
	}
	reorder();
	return *this;
}

bool LetterRange::operator==(const LetterRange& that) const {
	return std::is_permutation(std::begin(_range), std::end(_range), std::begin(that._range), std::end(that._range), [](const std::pair<Letter, Letter>& range1, const std::pair<Letter, Letter>& range2){
		return range1.first == range2.first && range1.second == range2.second;
	});
}

bool LetterRange::operator!=(const LetterRange& that) const {
	return !operator==(that);
}

bool LetterRange::operator<(const LetterRange& that) const {
	if(_range.size() != that._range.size())
		return _range.size() < that._range.size();

	for(unsigned int i=0; i<_range.size(); i++) {
		if(_range[i].first != that._range[i].first)
			return _range[i].first < that._range[i].first;
		if(_range[i].second != that._range[i].second)
			return _range[i].second < that._range[i].second;
	}
	return false;
}

void LetterRange::to_letter_list(std::vector<Letter>& list) const {
	for(const auto& range : _range) {
		for(Letter letter = range.first; letter <= range.second; letter++)
			list.push_back(letter);
	}
}

void LetterRange::reorder() {
	std::sort(std::begin(_range), std::end(_range), [](const std::pair<Letter, Letter>& range1, const std::pair<Letter, Letter>& range2) {
		return range1.first < range2.first;
	});
}

std::ostream& cliff::operator<<(std::ostream& stream, const LetterRange& that) {
	if(that.is_epsilon())
		stream << "Epsilon";
	for(unsigned int range_cursor=0; range_cursor < that.range().size(); range_cursor++) {
		if(std::isprint(that.range()[range_cursor].first))
			stream << (char)that.range()[range_cursor].first;
		else
			stream << "0x" << std::hex << that.range()[range_cursor].first << std::dec;

		if(that.range()[range_cursor].first != that.range()[range_cursor].second) {
			stream << "-";

			if(std::isprint(that.range()[range_cursor].second))
				stream << (char)that.range()[range_cursor].second;
			else
				stream << "0x" << std::hex << that.range()[range_cursor].second << std::dec;
		}

		if(range_cursor != that.range().size()-1)
			stream << "+";
	}
	return stream;
}
