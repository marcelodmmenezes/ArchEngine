/*===========================================================================*
 * Arch Engine - "GUI/writableComponent.cpp"                                 *
 *                                                                           *
 * Component to manage text input.                                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
 *===========================================================================*/


#include "writableComponent.hpp"


using namespace Core;
using namespace OS;
using namespace Utils;


namespace GUI {
	WritableComponent::WritableComponent() {}

	WritableComponent::WritableComponent(const glm::vec2& screen_size,
		const glm::vec2& position, unsigned font_id,
		float scale, int spacing, const glm::vec3& color,
		const glm::vec2& maximum_size)  {
		initialize(screen_size, position, font_id,
			scale, spacing, color, maximum_size);
	}

	WritableComponent::~WritableComponent() {}

	void WritableComponent::initialize(const glm::vec2& screen_size,
		const glm::vec2& position, unsigned font_id,
		float scale, int spacing, const glm::vec3& color,
		const glm::vec2& maximum_size) {
		m_font_id = font_id;
		m_text_scale = scale;
		m_spacing = spacing;
		m_text_color = color;
		m_maximum_size = maximum_size;
		m_current_color = m_text_color;
		m_hover_color = m_text_color;
		m_position = position;

		m_font_size = GUIManager::getInstance().getFontSize(m_font_id);

		m_mouse_space = glm::vec4(
			m_position.x,
			screen_size.y - m_position.y,
			m_position.x + m_maximum_size.x,
			screen_size.y - m_position.y + m_maximum_size.y
		);

		m_max_show_line_number = 1 + (int)((m_maximum_size.y -
			m_font_size * m_text_scale) / m_spacing);
	}

	void WritableComponent::update(float delta_time) {
		int start = std::max(0, m_current_line - m_max_show_line_number + 1);
		int offset = start * std::max(m_spacing,
			(int)(m_font_size * m_text_scale));

		for (unsigned i = start; i < m_text.size(); i++) {
			GUIManager::getInstance().renderText(
				m_font_id,
				m_text[i].text,
				m_text[i].start_position.x,
				m_text[i].start_position.y + offset,
				m_text_scale,
				m_current_color
			);
		}
	}

	void WritableComponent::write(char c) {
		if (m_text.size() == 0) {
			TextLine tl;
			tl.text = "";
			tl.start_position = glm::vec2(m_position.x,
				m_position.y - m_font_size * m_text_scale);
			tl.m_width = 0;
			m_text.push_back(tl);
			m_current_line = 0;
		}

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
				m_text[m_current_line].text.pop_back();
			}
		}
		else if (c == 10) { // New line
			TextLine tl;
			tl.text = "";
			tl.start_position = glm::vec2(m_position.x,
				m_text[m_current_line].start_position.y - m_spacing);
			tl.m_width = 0;
			m_text.push_back(tl);
			m_current_line++;
		}
		else {
			float width = GUIManager::getInstance().getCharLength(c,
				m_font_id, m_text_scale);

			if (m_text[m_current_line].m_width + width >
				m_maximum_size[0]) {
				TextLine tl;
				tl.text = "";
				tl.start_position = glm::vec2(m_position.x,
					m_text[m_current_line].start_position.y - m_spacing);
				tl.m_width = 0;
				m_text.push_back(tl);
				m_current_line++;
			}

			m_text[m_current_line].text += c;
			m_text[m_current_line].m_width += width;
		}
	}

	void WritableComponent::write(const std::string& text) {
		m_text.clear();

		for (auto& it : text)
			write(it);
	}

	void WritableComponent::setFont(unsigned id) {
		m_font_id = id;
	}

	void WritableComponent::setScale(float scale) {
		m_text_scale = scale;
	}

	void WritableComponent::setPosition(const glm::vec2& pos) {
		m_position = pos;

		for (unsigned i = 0; i < m_text.size(); i++)
			m_text[i].start_position = glm::vec2(m_position.x,
				m_position.y + i * m_spacing);
	}

	void WritableComponent::setColor(const glm::vec3& color) {
		m_text_color = color;
	}

	void WritableComponent::setHoverColor(const glm::vec3& color) {
		m_hover_color = color;
	}

	void WritableComponent::setMaximumSize(const glm::vec2& size) {
		m_maximum_size = size;
		format();
	}

	void WritableComponent::anchor(bool value) {
		m_anchored = value;
	}

	bool WritableComponent::isAnchored() {
		return m_anchored;
	}

	unsigned WritableComponent::getFont() const {
		return m_font_id;
	}

	float WritableComponent::getScale() const {
		return m_text_scale;
	}

	glm::vec2 WritableComponent::getPosition() const {
		return m_position;
	}

	glm::vec3 WritableComponent::getColor() const {
		return m_text_color;
	}

	glm::vec3 WritableComponent::getHoverColor() const {
		return m_hover_color;
	}

	glm::vec2 WritableComponent::getMaximumSize() const {
		return m_maximum_size;
	}

	void WritableComponent::mouseHover(int x, int y) {
		m_current_color = m_hover_color;
	}

	void WritableComponent::mouseOut() {
		m_current_color = m_text_color;
	}

	void WritableComponent::mouseDown(int x, int y, int button) {
		// TODO
	}

	void WritableComponent::mouseUp(int x, int y, int button) {
		// TODO
	}

	void WritableComponent::format() {
		std::string text;

		for (auto& it : m_text)
			text += it.text;

		m_text.clear();

		for (auto& it : text)
			write(it);
	}
}