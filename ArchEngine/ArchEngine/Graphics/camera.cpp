/*===========================================================================*
 * Arch Engine - "Graphics/camera.cpp"                                       *
 *                                                                           *
 * TODO: description.                                                        *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/06/2018                                                       *
 * Last Modified: 16/06/2018                                                 *
 *===========================================================================*/


#include "camera.hpp"


namespace Graphics {
	Camera::~Camera() {}

	glm::vec3 Camera::getPosition() const {
		return m_position;
	}

	glm::vec3 Camera::getFront() const {
		return m_front;
	}

	glm::mat4 Camera::getViewMatrix() const {
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	void Camera::setSpeed(float speed) {
		m_movement_speed = speed;
	}

	float Camera::getSpeed() {
		return m_movement_speed;
	}
}