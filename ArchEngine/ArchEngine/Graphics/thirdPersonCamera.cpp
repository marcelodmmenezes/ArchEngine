/*===========================================================================*
 * Arch Engine - "Graphics/thirdPersonCamera.cpp"                            *
 *                                                                           *
 * Third person camera.                                                      *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/06/2018                                                       *
 * Last Modified: 17/06/2018                                                 *
 *===========================================================================*/


#include "thirdPersonCamera.hpp"


namespace Graphics {
	static const float G_PI = 3.14159265359f;

	ThirdPersonCamera::ThirdPersonCamera(float distance_from_target,
		float yaw, float pitch) {
		m_yaw = yaw;
		m_pitch = pitch;
		m_distance_from_target = distance_from_target;
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	ThirdPersonCamera::~ThirdPersonCamera() {}

	void ThirdPersonCamera::update(const glm::vec3& target) {
		m_front = target;

		m_position.x =
			target.x + m_distance_from_target * cos(m_pitch) * sin(m_yaw);
		m_position.y =
			target.y + m_distance_from_target * sin(m_pitch);
		m_position.z =
			target.z + m_distance_from_target * cos(m_pitch) * cos(m_yaw);
	}
	
	void ThirdPersonCamera::calcYaw(float x) {
		m_yaw += m_delta_time * x;
	}

	void ThirdPersonCamera::calcPitch(float y) {
		m_pitch -= m_delta_time * y;

		if (m_pitch > G_PI / 2.0f - 0.1f)
			m_pitch = G_PI / 2.0f - 0.1f;
		else if (m_pitch < -G_PI / 2.0f + 0.1f)
			m_pitch = -G_PI / 2.0f + 0.1f;
	}

	glm::mat4 ThirdPersonCamera::getViewMatrix() const {
		return glm::lookAt(m_position, m_front, m_up);
	}
}