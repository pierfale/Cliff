#ifndef _CLIFF_MODULE_H
#define _CLIFF_MODULE_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "cliff/shared/Singleton.h"

namespace cliff {

	class Module {

	public:
		enum Order {
			PreCompiler,
			Optimizer,
			PostCompiler
		};

		Module();
		virtual void initialize();

	};

	template<unsigned int Order>
	class ModuleList : public Singleton<ModuleList<Order>>{

		SINGLETON_HELPER(ModuleList<Order>)

		friend class Core;

	public:

		static void add(Module* module) {
			instance()._module_list.push_back(module);
		}

		static void initialize() {
			std::for_each(std::begin(instance()._module_list), std::end(instance()._module_list), [](Module* module){ module->initialize(); });
		}


	private:
		ModuleList() {

		}

		std::vector<Module*> _module_list;
	};


	template<typename ModuleType, unsigned int Order>
	class RegisterModule {

	public:
		RegisterModule() {
			ModuleList<Order>::add(new ModuleType);
		}

		~RegisterModule() {

		}

	private:

		RegisterModule(const RegisterModule& that) = delete;

		static std::vector<Module*> _module_list;

	};
}
#endif
