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
	Arcball::Arcball(int width, int height, float radius,
		const glm::vec3& pos, const glm::vec3& front,
		const glm::vec3& up) {
		m_width = width;
		m_height = height;
		m_radius = radius;
		m_position = pos;
		m_front = front;
		m_up = up;
		m_is_rotating = false;
	}

	Arcball::~Arcball() {}

	void Arcball::setWidthHeight(int width, int height) {
		m_width = width;
		m_height = height;
		m_radius = std::min(width / 2, height / 2);
	}

	void Arcball::setRadius(float radius) {
		m_radius = radius;
	}
	
	void Arcball::updateRotation(const glm::vec2& pos) {
		if (!m_is_rotating)
			startRotation(pos);
		else {
			float x = pos.x - m_width / 2.0f;
			float y = m_height / 2.0f - pos.y;

			m_current_rotation_vector =
				glm::normalize(convertCoords(glm::vec2(x, y)));

			calc();
		}
	}

	void Arcball::stopRotation() {
		auto a = m_start_rotation_vector;
		auto b = m_current_rotation_vector;

		m_is_rotating = false;
	}

	void Arcball::calc() {
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

				glm::vec3 right = glm::normalize(glm::cross(m_front, m_up));


				std::cout << "(" << right.x << "," << right.y << "," << right.z << ")                \r";
				//std::cout << "(" << m_current_rotation_vector.x << "," << m_current_rotation_vector.y << "," << m_current_rotation_vector.z << ")                 \r";
			}
		}
	}

	void Arcball::startRotation(const glm::vec2& pos) {
		float x = pos.x - m_width / 2.0f;
		float y = m_height / 2.0f - pos.y;

		m_start_rotation_vector =
			glm::normalize(convertCoords(glm::vec2(x, y)));

		m_current_rotation_vector = m_start_rotation_vector;

		m_is_rotating = true;
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