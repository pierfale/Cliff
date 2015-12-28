#ifndef _CLIFF_ENTRY_BOUND_REGISTER
#define _CLIFF_ENTRY_BOUND_REGISTER

#include <map>
#include <cstring>

#include "cliff/shared/EntryModule.h"

namespace cliff {

	class EntryBoundRegister {

		friend class ProgramOption;

	public:

		static void bind_module_to_option(EntryModule* module, const char* reduced_name, const char* full_name, Module::Order stage);
		static void bind_module(EntryModule* module, Module::Order stage);


	private:
		EntryBoundRegister();

		struct BindOption {

			struct StrCompare : public std::binary_function<const char*, const char*, bool> {
			public:
				bool operator() (const char* str1, const char* str2) const
				{ return std::strcmp(str1, str2) < 0; }
			};

			std::map<const char*, EntryModule*, StrCompare> _reduced_binds;
			std::map<const char*, EntryModule*, StrCompare> _full_binds;
		};

		static std::map<Module::Order, BindOption> _binds;

	};

}

#endif
