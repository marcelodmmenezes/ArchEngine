/*===========================================================================*
 * Arch Engine - "GUI/renderableComponent.hpp"                               *
 *                                                                           *
 * Component to allow GUI rendering.                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 22/06/2018                                                       *
 * Last Modified: 22/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_RENDERABLE_COMPONENT_HPP
#define GUI_RENDERABLE_COMPONENT_HPP


#include "../Graphics/graphicsManager.hpp"
#include "../Graphics/materialManager.hpp"
#include "guiComponent.hpp"

#include <glm/glm.hpp>


namespace GUI {
	class RenderableComponent : public GUIComponent {
	public:
		RenderableComponent(unsigned shader_id,
			const glm::vec4& limits,
			const std::string& texture_path = "");
		~RenderableComponent();

		void setColor(const glm::vec4& color);
		void setLimits(const glm::vec4& limits);
		
		unsigned getTextureId() const;
		glm::vec4 getColor() const;
		glm::vec4 getLimits() const;

		bool hasTexture() const;
		bool hasColor() const;

		void render(const glm::mat4& projection);

	private:
		unsigned m_texture_id;
		glm::vec4 m_color;
		glm::vec4 m_limits;

		bool m_has_texture;
		bool m_has_color;

		unsigned m_shader_id;
	};
}


#endif	// GUI_RENDERABLE_COMPONENT_HPP