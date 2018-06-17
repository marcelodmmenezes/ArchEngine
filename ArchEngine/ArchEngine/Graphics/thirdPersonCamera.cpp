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
		m_interpolation_factor = 0.95f;
		m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	ThirdPersonCamera::~ThirdPersonCamera() {}

	void ThirdPersonCamera::update(const glm::vec3& target) {
		glm::vec3 target_pos = glm::mix(
			target,
			m_old_target_pos,
			m_interpolation_factor
		);

		m_old_target_pos = target_pos;
		m_front = target_pos;

		m_position = glm::vec3(
			target_pos.x + m_distance_from_target * cos(m_pitch) * sin(m_yaw),
			target_pos.y + m_distance_from_target * sin(m_pitch),
			target_pos.z + m_distance_from_target * cos(m_pitch) * cos(m_yaw)
		);
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

	void ThirdPersonCamera::increaseDistanceFromTarget(float dist) {
		m_distance_from_target -= dist;
	}

	float ThirdPersonCamera::getDistanceFromTarget() const {
		return m_distance_from_target;
	}

	void ThirdPersonCamera::setInterpolationFactor(float factor) {
		m_interpolation_factor = factor;
	}

	float ThirdPersonCamera::getInterpolationFactor() const {
		return m_interpolation_factor;
	}
}