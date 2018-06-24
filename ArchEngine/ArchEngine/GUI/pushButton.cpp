/*===========================================================================*
 * Arch Engine - "GUI/pushButton.hpp"                                        *
 *                                                                           *
 * Clickable button.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 23/06/2018                                                       *
 * Last Modified: 23/06/2018                                                 *
 *===========================================================================*/


#include "pushButton.hpp"


using namespace Core;


namespace GUI {
	PushButton::PushButton() {}

	PushButton::PushButton(int button_id,
		unsigned shader_id,
		const glm::vec2& screen_size,
		const glm::vec4& limits,
		unsigned font_id,
		float font_scale,
		const glm::vec3& text_color,
		const std::string& texture_path) {
		initialize(button_id, shader_id, screen_size, limits,
			font_id, font_scale, text_color, texture_path);
	}

	PushButton::~PushButton() {}

	void PushButton::initialize(int button_id,
		unsigned shader_id,
		const glm::vec2& screen_size,
		const glm::vec4& limits,
		unsigned font_id,
		float font_scale,
		const glm::vec3& text_color,
		const std::string& texture_path) {
		m_id = button_id;
		m_position = glm::vec2(limits.x, limits.y);
		m_size = glm::vec2(limits.z, limits.w);
		m_screen_size = screen_size;
		m_limits = limits;

		m_mouse_pos = glm::vec2(-1, -1);

		m_font_id = font_id;
		m_font_scale = font_scale;

		float text_y_pos = m_font_scale *
			GUIManager::getInstance().getFontSize(m_font_id);
		text_y_pos /= 2;
		text_y_pos = m_position.y + (m_size.y / 2.0) - text_y_pos;
		m_text_pos = glm::vec2(limits.x + 5.0f, text_y_pos);

		m_text_color = text_color;

		m_mouse_space = glm::vec4(
			limits.x,
			screen_size.y - limits.y - limits.w,
			limits.x + limits.z,
			screen_size.y - limits.y
		);

		m_renderable.initialize(shader_id, screen_size, limits, texture_path);

		m_window_resize_listener.bind<PushButton,
			&PushButton::onWindowResizeEvent>(this);
		EventManager::getInstance().addListener(
			m_window_resize_listener, EVENT_WINDOW_RESIZE);
	}

	void PushButton::update(const glm::mat4& projection) {
		m_renderable.render(projection);

		bool depth_test = false;
		if (glIsEnabled(GL_DEPTH_TEST))
			depth_test = true;

		glDisable(GL_DEPTH_TEST);
		GUIManager::getInstance().renderText(m_font_id, m_text,
			m_text_pos.x, m_text_pos.y, m_font_scale, m_text_color);

		if (depth_test)
			glEnable(GL_DEPTH_TEST);
	}

	void PushButton::click() {
		if (m_mouse_pos.x >= 0 && m_mouse_pos.y >= 0) {
			EventPtr evnt = std::make_shared<ButtonClickedEvent>(
				ButtonClickedEvent(m_id));
			EventManager::getInstance().sendEvent(evnt);
		}
	}

	//-------------------------------------------------------------------- Text
	void PushButton::setText(const std::string& text) {
		m_text = text;

		m_text_width = 0;

		for (auto& it : m_text)
			m_text_width +=
				GUIManager::getInstance().getCharLength(
					it, m_font_id, m_font_scale);

		centralize();
	}

	std::string PushButton::getText() const {
		return m_text;
	}

	void PushButton::justifyLeft() {
		m_text_pos.x = m_position.x + 5.0f;
		m_text_justification = -1;
	}

	void PushButton::centralize() {
		m_text_pos.x = m_position.x + (m_size.x - m_text_width) / 2.0f;
		m_text_justification = 0;
	}

	void PushButton::justifyRight() {
		m_text_pos.x = m_position.x + (m_size.x - m_text_width) - 5.0f;
		m_text_justification = 1;
	}
	//-------------------------------------------------------------------------

	//--------------------------------------------------------------- Overrides
	void PushButton::trackMouse() {
		m_renderable.trackMouse();

		EventManager::getInstance().addListener(
			m_mouse_moved_listener, EVENT_MOUSE_MOVED);
	}

	void PushButton::untrackMouse() {
		m_renderable.untrackMouse();

		EventManager::getInstance().removeListener(
			m_mouse_moved_listener, EVENT_MOUSE_MOVED);
	}
	//-------------------------------------------------------------------------

	//---------------------------------------------------- Renderable component
	void PushButton::setRenderColor(const glm::vec4& color) {
		m_renderable.setColor(color);
		m_body_color = color;
	}

	void PushButton::setRenderHoverColor(const glm::vec4& color) {
		m_renderable.setHoverColor(color);
	}

	void PushButton::setRenderBorderColor(const glm::vec4& color) {
		m_renderable.setBorderColor(color);
	}

	void PushButton::setRenderBorderWidth(int width) {
		m_renderable.setBorderWidth(width);
	}

	void PushButton::setRenderLimits(const glm::vec4& limits) {
		m_renderable.setLimits(limits);
	}

	void PushButton::removeRenderTexture() {
		m_renderable.removeTexture();
	}

	void PushButton::removeRenderColor() {
		m_renderable.removeColor();
	}

	void PushButton::removeRenderBorder() {
		m_renderable.removeBorder();
	}

	unsigned PushButton::getRenderTextureId() const {
		return m_renderable.getTextureId();
	}

	glm::vec4 PushButton::getRenderColor() const {
		return m_renderable.getColor();
	}

	glm::vec4 PushButton::getRenderHoverColor() const {
		return m_renderable.getHoverColor();
	}

	glm::vec4 PushButton::getRenderBorderColor() const {
		return m_renderable.getBorderColor();
	}

	int PushButton::getRenderBorderWidth() const {
		return m_renderable.getBorderWidth();
	}

	glm::vec4 PushButton::getRenderLimits() const {
		return m_renderable.getLimits();
	}

	bool PushButton::hasRenderTexture() const {
		return m_renderable.hasTexture();
	}

	bool PushButton::hasRenderColor() const {
		return m_renderable.hasColor();
	}

	bool PushButton::hasRenderBorder() const {
		return m_renderable.hasBorder();
	}
	//-------------------------------------------------------------------------

	void PushButton::mouseHover(int x, int y) {
		m_renderable.setColor(m_body_color +
			glm::vec4(0.1f, 0.1f, 0.1f, 0.0f));

		m_mouse_pos = glm::vec2(x, y);
	}

	void PushButton::mouseOut() {
		m_renderable.setColor(m_body_color);

		m_mouse_pos = glm::vec2(-1, -1);
	}

	void PushButton::onWindowResizeEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<WindowResizeEvent>(e);

		int w, h;

		evnt->getSize(w, h);
		
		m_position.y += (h - m_screen_size.y);
		m_limits.y = m_position.y;

		float text_y_pos = m_font_scale *
			GUIManager::getInstance().getFontSize(m_font_id);
		text_y_pos /= 2;
		text_y_pos = m_position.y + (m_size.y / 2.0) - text_y_pos;
		m_text_pos = glm::vec2(m_limits.x + 5.0f, text_y_pos);

		if (m_text_justification == -1)
			justifyLeft();
		else if (m_text_justification == 0)
			centralize();
		else
			justifyRight();

		m_renderable.setLimits(m_limits);

		m_screen_size = glm::vec2(w, h);
	}
}