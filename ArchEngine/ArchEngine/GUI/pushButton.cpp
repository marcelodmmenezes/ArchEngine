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

	PushButton::PushButton(unsigned shader_id,
		const glm::vec2& screen_size,
		const glm::vec4& limits,
		unsigned font_id,
		float font_scale,
		int text_spacing,
		const glm::vec3& text_color,
		const std::string& texture_path) {
		initialize(shader_id, screen_size, limits, font_id,
			font_scale, text_spacing, text_color, texture_path);
	}

	PushButton::~PushButton() {}

	void PushButton::initialize(unsigned shader_id,
		const glm::vec2& screen_size,
		const glm::vec4& limits,
		unsigned font_id,
		float font_scale,
		int text_spacing,
		const glm::vec3& text_color,
		const std::string& texture_path) {
		m_position = glm::vec2(limits.x, limits.y);

		m_mouse_space = glm::vec4(
			limits.x,
			screen_size.y - limits.y - limits.w,
			limits.x + limits.z,
			screen_size.y - limits.y
		);

		m_renderable.initialize(shader_id, screen_size, limits, texture_path);

		m_writable.initialize(screen_size,
			m_position + glm::vec2(5.0f, m_position.y + limits.w - 5.0f),
			font_id, font_scale, text_spacing, text_color,
			glm::vec2(limits.z, limits.w)
		);
	}

	void PushButton::update(const glm::mat4& projection) {
		m_writable.update();
		m_renderable.render(projection);
	}

	void PushButton::click() {
		// TODO
		std::cout << "lalala" << std::endl;
	}

	void PushButton::setText(const std::string& text) {
		m_text = text;
		m_writable.write(text);
		m_writable.centralize();
	}

	std::string PushButton::getText() const {
		return m_text;
	}

	//---------------------------------------------------- Renderable component
	void PushButton::setRenderColor(const glm::vec4& color) {
		m_renderable.setColor(color);
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

	//------------------------------------------------------ Writable component
	void PushButton::setTextFont(unsigned id) {
		m_writable.setFont(id);
	}

	void PushButton::setTextScale(float scale) {
		m_writable.setScale(scale);
	}

	void PushButton::setTextPosition(const glm::vec2& pos) {
		m_writable.setPosition(pos);
	}

	void PushButton::setTextColor(const glm::vec3& color) {
		m_writable.setColor(color);
	}

	void PushButton::setTextHoverColor(const glm::vec3& color) {
		m_writable.setHoverColor(color);
	}

	void PushButton::setTextMaximumSize(const glm::vec2& size) {
		m_writable.setMaximumSize(size);
	}

	unsigned PushButton::getTextFont() const {
		return m_writable.getFont();
	}

	float PushButton::getTextScale() const {
		return m_writable.getScale();
	}

	glm::vec2 PushButton::getTextPosition() const {
		return m_writable.getPosition();
	}

	glm::vec3 PushButton::getTextColor() const {
		return m_writable.getColor();
	}

	glm::vec3 PushButton::getTextHoverColor() const {
		return m_writable.getHoverColor();
	}

	glm::vec2 PushButton::getTextMaximumSize() const {
		return m_writable.getMaximumSize();
	}
	//-------------------------------------------------------------------------

	void PushButton::mouseHover() {
		// TODO
	}

	void PushButton::mouseOut() {
		// TODO
	}
}