/*===========================================================================*
 * Arch Engine - "GUI/textBox.hpp"                                           *
 *                                                                           *
 * Textbox.                                                                  *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/06/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_TEXT_BOX_HPP
#define GUI_TEXT_BOX_HPP


#include "guiComponent.hpp"
#include "renderableComponent.hpp"
#include "writableComponent.hpp"


namespace GUI {
	class TextBox : public GUIComponent {
	public:
		TextBox();
		TextBox(int text_box_id,
			unsigned shader_id,
			const glm::vec2& screen_size,
			const glm::vec4& limits,
			unsigned font_id,
			float font_scale,
			const glm::vec3& text_color,
			const std::string& texture_path);
		~TextBox();

		void initialize(int button_id,
			unsigned shader_id,
			const glm::vec2& screen_size,
			const glm::vec4& limits,
			unsigned font_id,
			float font_scale,
			const glm::vec3& text_color,
			const std::string& texture_path);
	private:

		RenderableComponent m_renderable;
		WritableComponent m_writable;
	};
}


#endif	// GUI_TEXT_BOX_HPP