/*===========================================================================*
 * Arch Engine - "Graphics/skybox.hpp"                                       *
 *                                                                           *
 * Skybox.                                                                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/06/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_SKYBOX_HPP
#define GRAPHICS_SKYBOX_HPP


#include "glad_3_3_core.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>

#include <string>


namespace Graphics {
	const unsigned NUMBER_OF_INDICES = 36;

	class Skybox {
	public:
		Skybox();
		~Skybox();

		void initialize(unsigned shader_id, unsigned texture_id);
		void destroy();
		
		unsigned getShaderId() const;
		unsigned getTextureId() const;

		void setFogColor(const glm::vec3& color);
		glm::vec3 getFogColor() const;

		void draw(GLenum primitive) const;

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		void createVao();

		State m_state;

		unsigned m_shader_id;
		unsigned m_texture_id;

		unsigned m_vao_id;
		unsigned m_vbo_id;
		unsigned m_ebo_id;
				
		glm::vec3 m_fog_color;
	};
}


#endif	// GRAPHICS_SKYBOX_HPP