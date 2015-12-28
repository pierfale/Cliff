#ifndef _CLIFF_SINGLETON_H
#define _CLIFF_SINGLETON_H

namespace cliff {

#	define SINGLETON_HELPER(TYPE)	friend class Singleton<TYPE>;\
									static TYPE& instance() { Singleton<TYPE>::instance(); }

	template<typename Class>
	class Singleton {

	public:
		static Class& instance() {
			check_instance();
			return *_instance;
		}

	protected:
		Singleton() {}

	private:
		Singleton(const Singleton<Class>& that) = delete;

		static void check_instance() {
			if(_instance == nullptr)
				_instance = new Class;
		}

		static Class* _instance;
	};

	template<typename Class>
	Class* Singleton<Class>::_instance = nullptr;
}
#endif
