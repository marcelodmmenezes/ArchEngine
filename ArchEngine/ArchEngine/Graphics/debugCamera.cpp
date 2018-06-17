/*===========================================================================*
 * Arch Engine - "Graphics/debugCamera.cpp"                                  *
 *                                                                           *
 * First person camera for scene debugging.                                  *
 * Based in:                                                                 *
 * - (https://learnopengl.com/Getting-started/Camera)                        *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 17/06/2018                                                 *
 *===========================================================================*/


#include "debugCamera.hpp"


using namespace Core;
using namespace OS;


namespace Graphics {
	DebugCamera::DebugCamera(glm::vec3 position, glm::vec3 front,
		glm::vec3 up, float yaw, float pitch) {
		m_position = position;
		m_front = front;
		m_world_up = up;
		m_yaw = yaw;
		m_pitch = pitch;
		m_movement_speed = SPEED;
		m_zoom = ZOOM;
		updateCameraVectors();
	}
	
	DebugCamera::DebugCamera(float pos_x, float pos_y, float pos_z, float up_x,
		float up_y, float up_z, float yaw, float pitch) {
		m_position = glm::vec3(pos_x, pos_y, pos_z);
		m_front = glm::vec3(0.0f, 0.0f, -1.0f);
		m_world_up = glm::vec3(up_x, up_y, up_z);
		m_yaw = yaw;
		m_pitch = pitch;
		m_movement_speed = SPEED;
		m_zoom = ZOOM;
		updateCameraVectors();
	}

	DebugCamera::~DebugCamera() {}

	void DebugCamera::move(CameraMovement direction) {
		float velocity = m_movement_speed * m_delta_time;

		if (direction == CAMERA_FORWARD)
			m_position += m_front * velocity;
		else if (direction == CAMERA_BACKWARD)
			m_position -= m_front * velocity;
		else if (direction == CAMERA_LEFT)
			m_position -= m_right * velocity;
		else if (direction == CAMERA_RIGHT)
			m_position += m_right * velocity;
		else if (direction == CAMERA_UP)
			m_position.y += 1.0f * velocity;
		else
			m_position.y -= 1.0f * velocity;
	}

	glm::mat4 DebugCamera::getViewMatrix() const {
		return glm::lookAt(m_position, m_position + m_front, m_up);
	}

	void DebugCamera::look(float x_offset, float y_offset) {
		m_yaw += x_offset;
		m_pitch += y_offset;

		if (m_yaw > 360.0f)
			m_yaw -= 360.0f;
		else if (m_yaw < 0.0f)
			m_yaw += 360.0f;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		else if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		updateCameraVectors();
	}

	void DebugCamera::zoom(float y_offset) {
		if (m_zoom >= 1.0f && m_zoom <= 45.0f)
			m_zoom -= y_offset;

		if (m_zoom <= 1.0f)
			m_zoom = 1.0f;
		else if (m_zoom >= 45.0f)
			m_zoom = 45.0f;
	}

	void DebugCamera::updateCameraVectors() {
		glm::vec3 front;

		front.x = cos(glm::radians(m_pitch)) * -sin(glm::radians(m_yaw));
		front.y = sin(glm::radians(m_pitch));
		front.z = cos(glm::radians(m_pitch)) * -cos(glm::radians(m_yaw));

		m_front = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_front, m_world_up));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}
}