/*===========================================================================*
 * Arch Engine - "Graphics/writableComponent.hpp"                            *
 *                                                                           *
 * Component to manage text input.                                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 21/06/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"
#include "guiManager.hpp"
#include "../Utils/serviceLocator.hpp"


#include <glm/glm.hpp>

#include <vector>


namespace GUI {
	struct TextLine {
		std::string text;
		glm::vec2 start_position;
		float m_width;

	};

	class WritableComponent {
	public:
		WritableComponent(unsigned font_id, float scale,
			const glm::vec2& position, int spacing,
			const glm::vec3& color, const glm::vec4& maximum_box);

		void write(char c);

		void update();

		void setFont(unsigned id);
		void setScale(float scale);
		void setPosition(const glm::vec2& pos);
		void setColor(const glm::vec3& color);

		unsigned getFont() const;
		float getScale() const;
		glm::vec2 getPosition() const;
		glm::vec3 getColor() const;

	private:
		unsigned m_font_id;
		float m_text_scale;
		int m_spacing;
		glm::vec2 m_start_position;
		glm::vec3 m_text_color;
		glm::vec4 m_maximum_box;

		int m_current_line;

		std::vector<TextLine> m_text;
	};
}