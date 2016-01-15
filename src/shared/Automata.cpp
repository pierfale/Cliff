#include "cliff/shared/Automata.h"

using namespace cliff;


//
//	LetterRange
//

LetterRange::LetterRange() : _start_range(Max_letter), _end_range(Max_letter) {

}

LetterRange::LetterRange(Letter unique_letter) : _start_range(unique_letter), _end_range(unique_letter) {

}

LetterRange::LetterRange(Letter start_letter, Letter end_letter) : _start_range(start_letter), _end_range(end_letter) {

}

Letter LetterRange::start_range() const {
	return _start_range;
}

Letter LetterRange::end_range() const {
	return _end_range;
}

bool LetterRange::is_epsilon() const {
	return _start_range == Max_letter && _end_range == Max_letter;
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
