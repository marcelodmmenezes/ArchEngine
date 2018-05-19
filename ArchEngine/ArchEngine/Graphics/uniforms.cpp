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


#include "uniforms.hpp"


namespace Graphics {
	IUniform::IUniform(const std::string& name, unsigned location) :
		m_name(name), m_location(location), m_dirty(false) {}

	IUniform::~IUniform() {}
}