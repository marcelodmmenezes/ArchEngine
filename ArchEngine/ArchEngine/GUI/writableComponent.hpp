/*===========================================================================*
 * Arch Engine - "GUI/writableComponent.hpp"                                 *
 *                                                                           *
 * Component to manage text input.                                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 23/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_WRITABLE_COMPONENT_HPP
#define GUI_WRITABLE_COMPONENT_HPP


#include "../Config/engineMacros.hpp"
#include "guiComponent.hpp"
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

	class WritableComponent : public GUIComponent {
	public:
		WritableComponent();
		WritableComponent(const glm::vec2& screen_size,
			const glm::vec2& position, unsigned font_id,
			float scale, int spacing, const glm::vec3& color,
			const glm::vec2& maximum_size);
		~WritableComponent();

		void initialize(const glm::vec2& screen_size,
			const glm::vec2& position, unsigned font_id,
			float scale, int spacing, const glm::vec3& color,
			const glm::vec2& maximum_size);

		void write(char c);
		void write(const std::string& text);

		void update(float delta_time) override;

		void setFont(unsigned id);
		void setScale(float scale);
		void setPosition(const glm::vec2& pos);
		void setColor(const glm::vec3& color);
		void setHoverColor(const glm::vec3& color);
		void setMaximumSize(const glm::vec2& size);

		void anchor(bool value);
		bool isAnchored();

		unsigned getFont() const;
		float getScale() const;
		glm::vec2 getPosition() const;
		glm::vec3 getColor() const;
		glm::vec3 getHoverColor() const;
		glm::vec2 getMaximumSize() const;

	private:
		//------------------------------------------------------ Mouse tracking
		void mouseHover(int x, int y) override;
		void mouseOut() override;
		void mouseDown(int x, int y, int button) override;
		void mouseUp(int x, int y, int button) override;
		//---------------------------------------------------------------------

		void format();

		unsigned m_font_id;
		float m_text_scale;
		int m_spacing;
		glm::vec3 m_text_color;
		glm::vec3 m_hover_color;
		glm::vec3 m_current_color;
		glm::vec2 m_maximum_size;

		bool m_anchored;

		int m_font_size;
		int m_current_line;
		int m_max_show_line_number;

		std::vector<TextLine> m_text;
	};
}


#endif	// GUI_WRITABLE_COMPONENT_HPP