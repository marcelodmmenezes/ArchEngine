/*===========================================================================*
 * Arch Engine - "GUI/renderableComponent.hpp"                               *
 *                                                                           *
 * Component to allow GUI rendering.                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 22/06/2018                                                       *
 * Last Modified: 22/06/2018                                                 *
 *===========================================================================*/


#include "renderableComponent.hpp"


using namespace Graphics;


namespace GUI {
	RenderableComponent::RenderableComponent(unsigned shader_id,
		const glm::vec4& limits, const std::string& texture_path) :
		m_shader_id(shader_id), m_limits(limits), m_has_color(false),
		m_has_border(false) {
		if (texture_path != "") {
			m_has_texture = true;
			m_texture_id =
				MaterialManager::getInstance().add2DTexture(texture_path);
		}
		else m_has_texture = false;
	}

	RenderableComponent::~RenderableComponent() {
		MaterialManager::getInstance().remove2DTexture(m_texture_id);
	}

	void RenderableComponent::setColor(const glm::vec4& color) {
		m_color = color;
		m_has_color = true;
	}

	void RenderableComponent::setBorderColor(const glm::vec4& color) {
		m_border_color = color;
		m_has_border = true;
	}

	void RenderableComponent::setBorderWidth(int width) {
		m_border_width = width;
	}

	void RenderableComponent::setLimits(const glm::vec4& limits) {
		m_limits = limits;
	}

	void RenderableComponent::removeTexture() {
		m_has_texture = false;
	}

	void RenderableComponent::removeColor() {
		m_has_color = false;
	}

	void RenderableComponent::removeBorder() {
		m_has_border = false;
	}

	unsigned RenderableComponent::getTextureId() const {
		return m_texture_id;
	}

	glm::vec4 RenderableComponent::getColor() const {
		return m_color;
	}

	glm::vec4 RenderableComponent::getBorderColor() const {
		return m_border_color;
	}

	int RenderableComponent::getBorderWidth() const {
		return m_border_width;
	}

	glm::vec4 RenderableComponent::getLimits() const {
		return m_limits;
	}

	bool RenderableComponent::hasTexture() const {
		return m_has_texture;
	}

	bool RenderableComponent::hasColor() const {
		return m_has_color;
	}

	bool RenderableComponent::hasBorder() const {
		return m_has_border;
	}

	void RenderableComponent::render(const glm::mat4& projection) {
		auto sh = GraphicsManager::getInstance().getShader(m_shader_id);
		sh->bind();

		sh->setMat4("u_projection_matrix", projection);
		
		sh->setBool("u_has_texture", m_has_texture);
		sh->setBool("u_has_color", m_has_color);

		if (m_has_texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_texture_id);
			sh->setInt("u_texture", 0);
		}

		if (m_has_color)
			sh->setVec4("u_color", m_color);

		sh->update();

		GraphicsManager::getInstance().drawQuad(m_limits);

		if (m_has_texture) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		if (m_has_border) {
			sh->setBool("u_has_texture", false);
			sh->setBool("u_has_color", true);
			sh->setVec4("u_color", m_border_color);
			sh->update();

			glDepthFunc(GL_NOTEQUAL);

			GraphicsManager::getInstance().drawQuad(m_limits +
				glm::vec4(-m_border_width, -m_border_width,
					2 * m_border_width, 2 * m_border_width));

			glDepthFunc(GL_LESS);
		}
	}
}