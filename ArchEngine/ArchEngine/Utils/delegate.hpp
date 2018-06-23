/*===========================================================================*
 * Arch Engine - "Utils/delegate.hpp"                                        *
 *                                                                           *
 * Class for wrapping function pointers and class methods for the event      *
 * system. Variable number of arguments via template will be implemented as  *
 * needed.                                                                   *
 *                                                                           *
 * Based in:                                                                 *
 * - (https://blog.molecular-matters.com/2011/09/19/                         *
 *    generic-type-safe-delegates-and-events-in-c/)                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 03/05/2018                                                 *
 *===========================================================================*/


#ifndef UTILS_DELEGATE_HPP
#define UTILS_DELEGATE_HPP


#include "../Config/engineMacros.hpp"

#include <cassert>
#include <utility>


namespace Utils {
	//----------------------------------------------------------- Base template
	template<typename T> class Delegate {};

	//--------------------------------------------------- No arguments delegate
	template<typename T>
	class Delegate<T()> {
	public:
		Delegate() : m_func(nullptr, nullptr) {}

		// Binds a function to this instance
		template<void(*F)()>
		void bind() {
			m_func.first = nullptr; // Functions doesn't have class instance
			m_func.second = &function<F>;
		}

		// Binds a method to this instance
		template<typename C, void(C::*M)()>
		void bind(C* instance) {
			m_func.first = instance;
			m_func.second = &method<C, M>;
		}

		// Calls the bound function or method
		void invoke() const {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
			// Must be called after a function or method is bound.
			assert(m_func.second);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

			m_func.second(m_func.first);
		}

	private:
		// Instance of the class in case of method.
		typedef void* Instance;
		typedef void(*Method)(Instance);

		// In case of C-Style function
		template<void(*F)()>
		static inline void function(Instance) {
			F();
		}

		// In case of Method
		template<typename C, void(C::*M)()>
		static inline void method(Instance instance) {
			// It's safe to cast the instance pointer
			// back to it's original class
			(static_cast<C*>(instance)->*M)();
		}

		std::pair<Instance, Method> m_func;
	};


	//--------------------------------------------------- One argument delegate
	template<typename T, typename ARG0>
	class Delegate<T(ARG0)> {
	public:
		Delegate() : m_func(nullptr, nullptr) {}

		bool operator==(const Delegate<T(ARG0)>& dlgt) {
			return m_func.first == dlgt.m_func.first &&
				m_func.second == dlgt.m_func.second;
		}

		// Binds a function to this instance
		template<void(*F)(ARG0)>
		void bind() {
			m_func.first = nullptr; // Functions doesn't have class instance
			m_func.second = &function<F>;
		}

		// Binds a method to this instance
		template<typename C, void(C::*M)(ARG0)>
		void bind(C* instance) {
			m_func.first = instance;
			m_func.second = &method<C, M>;
		}

		// Calls the bound function or method
		void invoke(ARG0 arg0) const {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
			// Must be called after a function or method is bound.
			assert(m_func.second);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

			m_func.second(m_func.first, arg0);
		}

	private:
		// Instance of the class in case of method.
		typedef void* Instance;
		typedef void(*Method)(Instance, ARG0);

		// In case of C-Style function
		template<void(*F)(ARG0)>
		static inline void function(Instance, ARG0 arg0) {
			F(arg0);
		}

		// In case of Method
		template<typename C, void(C::*M)(ARG0)>
		static inline void method(Instance instance, ARG0 arg0) {
			// It's safe to cast the instance pointer
			// back to it's original class
			(static_cast<C*>(instance)->*M)(arg0);
		}

		std::pair<Instance, Method> m_func;
	};


	//-------------------------------------------------- Two arguments delegate
	template<typename T, typename ARG0, typename ARG1>
	class Delegate<T(ARG0, ARG1)> {
	public:
		Delegate() : m_func(nullptr, nullptr) {}

		// Binds a function to this instance
		template<void(*F)(ARG0, ARG1)>
		void bind() {
			m_func.first = nullptr; // Functions doesn't have class instance
			m_func.second = &function<F>;
		}

		// Binds a method to this instance
		template<typename C, void(C::*M)(ARG0, ARG1)>
		void bind(C* instance) {
			m_func.first = instance;
			m_func.second = &method<C, M>;
		}

		// Calls the bound function or method
		void invoke(ARG0 arg0, ARG1 arg1) const {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
			// Must be called after a function or method is bound.
			assert(m_func.second);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

			m_func.second(m_func.first, arg0, arg1);
		}

	private:
		// Instance of the class in case of method.
		typedef void* Instance;
		typedef void(*Method)(Instance, ARG0, ARG1);

		// In case of C-Style function
		template<void(*F)(ARG0, ARG1)>
		static inline void function(Instance, ARG0 arg0, ARG1 arg1) {
			F(arg0, arg1);
		}

		// In case of Method
		template<typename C, void(C::*M)(ARG0, ARG1)>
		static inline void method(Instance instance, ARG0 arg0, ARG1 arg1) {
			// It's safe to cast the instance pointer
			// back to it's original class
			(static_cast<C*>(instance)->*M)(arg0, arg1);
		}

		std::pair<Instance, Method> m_func;
	};
}


#endif	// UTILS_DELEGATE_HPP