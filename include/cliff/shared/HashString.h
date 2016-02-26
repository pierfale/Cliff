#ifndef _CLIFF_HASH_STRING_H
#define _CLIFF_HASH_STRING_H

#include <cstring>

namespace cliff {

	typedef unsigned int Hash;

	class HashGenerator {

	public:

		static constexpr Hash base_offset = 2166136261;
		static constexpr Hash prime_number = 16777619;

		static constexpr Hash execute(const char* str) {
			return _execute(str, base_offset);
		}

	private:
		static constexpr Hash _execute(const char* str, Hash current_hash) {
			return *str == '\0' ? current_hash : _execute(str+1, current_hash*base_offset ^ (Hash)*str);
		}
	};

	template<typename T>
	class ExternalPointerPolicy {

	public:
		T* constructor(T* ptr) {
			return ptr;
		}

		void destructor(T* ptr) {

		}

	};

	template<typename T>
	class CopyPointerPolicy {

	public:
		T* constructor(T* ptr) {
			_ptr = new T(*ptr);
			return _ptr;
		}

		void destructor(T* ptr) {
			delete _ptr;
		}

	private:
		T* _ptr;

	};

	template<typename T>
	class TransferPointerPolicy {

	public:
		T* constructor(T* ptr) {
			_ptr = ptr;
			return _ptr;
		}

		void destructor(T* ptr) {
			delete _ptr;
		}

	private:
		T* _ptr;

	};


	template<typename T>
	class CopyPointerArrayPolicy {

	public:
		CopyPointerArrayPolicy(T* array, unsigned int size) {
			_ptr = new T[size];
			std::copy(array, array+size, _ptr);
		}

		~CopyPointerArrayPolicy() {
			delete[] _ptr;
		}

		T* get() {
			return _ptr;
		}

		const T* get() const {
			return _ptr;
		}

	private:
		T* _ptr;

	};

	template<template<typename> class PointerArrayPolicy>
	class HashString {

	public:

		constexpr HashString(const char* str) : _string(str, std::strlen(str)+1), _hash(HashGenerator::execute(str)) {

		}

		bool operator==(const HashString& that) {
			return _hash = that._hash && std::strcmp(_string.get(), that._string.get()) == 0;
		}

		bool operator!=(const HashString& that) {
			return _hash != that._hash || std::strcmp(_string.get(), that._string.get()) != 0;
		}

		bool operator<(const HashString& that) {
			return _hash < that._hash || (_hash == that._hash && std::strcmp(_string.get(), that._string.get()) < 0);
		}


	private:
		PointerArrayPolicy<const char*> _string;
		Hash _hash;

	};

	typedef HashString<CopyPointerArrayPolicy> CopyHashString;
}
#endif
