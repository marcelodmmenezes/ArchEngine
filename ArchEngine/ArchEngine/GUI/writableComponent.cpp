/*===========================================================================*
 * Arch Engine - "Graphics/writableComponent.cpp"                            *
 *                                                                           *
 * Component to manage text input.                                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 21/06/2018                                                 *
 *===========================================================================*/


#include "writableComponent.hpp"


using namespace Utils;


namespace GUI {
	WritableComponent::WritableComponent(unsigned font_id, float scale,
		const glm::vec2& position, int spacing, const glm::vec3& color,
		const glm::vec4& maximum_box) :
		m_font_id(font_id), m_text_scale(scale),
		m_start_position(position), m_spacing(spacing),
		m_text_color(color), m_maximum_box(maximum_box), m_current_line(0) {
		TextLine tl;
		tl.text = "";
		tl.start_position = m_start_position;
		tl.m_width = 0;

		m_text.push_back(tl);
	}

	void WritableComponent::write(char c) {
		if (c == 8) { // Backspace
			std::string& text = m_text[m_current_line].text;

			if (m_text[m_current_line].text.size() >= 1) {
				m_text[m_current_line].m_width -= GUIManager::getInstance().
					getCharLength(text.back(), m_font_id, m_text_scale);
				m_text[m_current_line].text.pop_back();
			}
			else if (m_current_line > 0) {
				m_current_line--;
				m_text.pop_back();
			}
		}
		else {
			float width = GUIManager::getInstance().getCharLength(c,
				m_font_id, m_text_scale);

			if (m_text[m_current_line].m_width + width > m_maximum_box[1]) {
				TextLine tl;
				tl.text = "";
				tl.start_position = glm::vec2(m_start_position.x,
					m_text[m_current_line].start_position.y - m_spacing);
				tl.m_width = 0;
				m_text.push_back(tl);
				m_current_line++;
			}

			m_text[m_current_line].text += c;
			m_text[m_current_line].m_width += width;
		}
	}

	void WritableComponent::update() {
		for (auto& it : m_text)
			GUIManager::getInstance().renderText(m_font_id, it.text,
				it.start_position.x, it.start_position.y, m_text_scale,
				m_text_color);
	}

	void WritableComponent::setFont(unsigned id) {
		m_font_id = id;
	}

	void WritableComponent::setScale(float scale) {
		m_text_scale = scale;
	}

	void WritableComponent::setPosition(const glm::vec2& pos) {
		m_start_position = pos;

		for (unsigned i = 0; i < m_text.size(); i++)
			m_text[i].start_position = glm::vec2(m_start_position.x,
				m_start_position.y + i * m_spacing);
	}

	void WritableComponent::setColor(const glm::vec3& color) {
		m_text_color = color;
	}

	unsigned WritableComponent::getFont() const {
		return m_font_id;
	}

	float WritableComponent::getScale() const {
		return m_text_scale;
	}

	glm::vec2 WritableComponent::getPosition() const {
		return m_start_position;
	}

	glm::vec3 WritableComponent::getColor() const {
		return m_text_color;
	}
}