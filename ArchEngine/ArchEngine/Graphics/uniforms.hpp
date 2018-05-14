/*===========================================================================*
 * Arch Engine - "Graphics/uniforms.hpp"                                     *
 *                                                                           *
 * Classes for shader's uniform variables representation. Using a delayed    *
 * OpenGL call system, based in a dirty flag, to avoid unnecessary calls to  *
 * the GPU.                                                                  *
 *                                                                           *
 * Based in:                                                                 *
 * - Game Engine Gems 2                                                      *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 13/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_UNIFORMS_HPP
#define GRAPHICS_UNIFORMS_HPP


#include "glad_3_3_core.hpp"
#include "../Utils/delegate.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>


namespace Graphics {
	class IUniform;

	typedef std::shared_ptr<IUniform> UniformPtr;

	// For observer pattern
	// The string represents the uniform`s name
	typedef Utils::Delegate<void(const std::string&)> DirtyObserver;

	class IUniform {
	public:
		IUniform(const std::string& name, unsigned location,
			const DirtyObserver& observer);

		virtual ~IUniform() = 0;

		// Sends value to GPU
		virtual void update() = 0;

	protected:
		std::string m_name;
		unsigned m_location;
		bool m_dirty;

		DirtyObserver m_observer;
	};

	//----------------------------------------------------------------- Uniform
	template<typename T>
	class Uniform : public IUniform {
	public:
		Uniform(const std::string& name, unsigned location,
			const DirtyObserver& observer);

		~Uniform() override;

		void update() override;

		void setValue(const T& value);
		T getValue();

	private:
		T m_value;
	};

	template<typename T>
	Uniform<T>::Uniform(const std::string& name, unsigned location,
		const DirtyObserver& observer) : IUniform(name, location, observer) {}

	template<typename T>
	Uniform<T>::~Uniform() {}

	//----------------------------------------- update template specializations
	template<>
	inline void Uniform<bool>::update() {
		glUniform1i(m_location, m_value);
		m_dirty = false;
	}

	template<>
	inline void Uniform<int>::update() {
		glUniform1i(m_location, m_value);
		m_dirty = false;
	}

	template<>
	inline void Uniform<float>::update() {
		glUniform1f(m_location, m_value);
		m_dirty = false;
	}

	template<>
	inline void Uniform<glm::vec3>::update() {
		glUniform3f(m_location, m_value.x, m_value.y, m_value.z);
		m_dirty = false;
	}

	template<>
	inline void Uniform<glm::mat3>::update() {
		glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(m_value));
		m_dirty = false;
	}

	template<>
	inline void Uniform<glm::mat4>::update() {
		glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(m_value));
		m_dirty = false;
	}

	//-------------------------------------------------------------------------

	template<typename T>
	void Uniform<T>::setValue(const T& value) {
		if (!m_dirty && m_value != value) {
			m_dirty = true;
			m_observer.invoke(m_name);
		}

		m_value = value;
	}

	template<typename T>
	T Uniform<T>::getValue() {
		return m_value;
	}
}


#endif	// GRAPHICS_UNIFORMS_HPP