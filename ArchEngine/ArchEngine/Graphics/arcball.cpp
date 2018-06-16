/*===========================================================================*
 * Arch Engine - "Graphics/arcball.hpp"                                      *
 *                                                                           *
 * Arcball camera.                                                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 15/06/2018                                                       *
 * Last Modified: 16/06/2018                                                 *
 *===========================================================================*/


#include "arcball.hpp"


using namespace Utils;


namespace Graphics {
	Arcball::Arcball(const glm::vec3& pos, const glm::vec3& front,
		const glm::vec3& up) {
		m_position = pos;
		m_front = front;
		m_up = up;
		m_is_rotating = false;
	}

	Arcball::~Arcball() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Arcball destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void Arcball::setWidthHeight(int width, int height) {
		m_width = width;
		m_height = height;
		m_radius = std::min(width / 2, height / 2);
	}

	void Arcball::setRadius(float radius) {
		m_radius = radius;
	}

	void Arcball::startRotation(const glm::vec2& pos) {
		float x = (2.0f * pos.x) / m_width - 1.0f;
		float y = 1.0f - (2.0f * pos.y) / m_height;

		m_start_rotation_vector =
			glm::normalize(convertCoords(glm::vec2(x, y)));

		m_current_rotation_vector = m_start_rotation_vector;

		m_is_rotating = true;
	}

	void Arcball::updateRotation(const glm::vec2& pos) {
		float x = (2.0f * pos.x) / m_width - 1.0f;
		float y = 1.0f - (2.0f * pos.y) / m_height;

		m_current_rotation_vector =
			glm::normalize(convertCoords(glm::vec2(x, y)));
	}

	void Arcball::stopRotation() {
		auto a = m_start_rotation_vector;
		auto b = m_current_rotation_vector;
	}

	void Arcball::calc() {/*
		if (m_is_rotating) {
			if (glm::length((m_current_rotation_vector -
				m_start_rotation_vector)) > 1E-6) {
				glm::vec3 rotation_axis = glm::normalize(
					glm::cross(m_current_rotation_vector,
						m_start_rotation_vector));

				double value = glm::dot(m_current_rotation_vector,
					m_start_rotation_vector);

				value > (1 - 1E-10) ? value = 1.0 : value = value;

				float angle = acos(value) * 180.0f / PI;

				glm::vec3 right = glm::normalize(glm::cross(m_focus, m_up));

				m_position = (glm::rotate(right, angle, m_up) *
					(m_position - m_focus)) + m_focus;

				m_position = (Rotate(some other angle, (0, 1, 0)) * (position - target)) + target;

				view = LookAt(position, target, up);
			}
		}*/
	}

	glm::vec3 Arcball::convertCoords(const glm::vec2& coords) {
		float d = coords.x * coords.x + coords.y * coords.y;
		float radius_squared = m_radius * m_radius;

		if (d > radius_squared)
			return glm::vec3(coords.x, coords.y, 0.0f);
		else
			return glm::vec3(coords.x, coords.y, sqrt(radius_squared - d));
	}
}