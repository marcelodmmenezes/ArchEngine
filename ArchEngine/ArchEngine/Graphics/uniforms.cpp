/*===========================================================================*
 * Arch Engine - "Graphics/uniforms.cpp"                                     *
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


#include "uniforms.hpp"


namespace Graphics {
	IUniform::IUniform(unsigned id, unsigned location,
		const DirtyObserver& observer) :
		m_id(id), m_location(location), m_dirty(true), m_observer(observer) {
		m_observer.invoke(UniformPtr(this));
	}

	IUniform::~IUniform() {}
}