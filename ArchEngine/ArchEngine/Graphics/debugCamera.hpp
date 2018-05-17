/*===========================================================================*
 * Arch Engine - "Graphics/debugCamera.hpp"                                  *
 *                                                                           *
 * First person camera for scene debugging.                                  *
 * Based in:                                                                 *
 * - (https://learnopengl.com/Getting-started/Camera)                        *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_DEBUG_CAMERA_HPP
#define GRAPHICS_DEBUG_CAMERA_HPP


#include "../Core/eventManager.hpp"
#include "../OS/inputManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <queue>


namespace Graphics {
	enum CameraMovement {
		CAMERA_FORWARD,
		CAMERA_BACKWARD,
		CAMERA_LEFT,
		CAMERA_RIGHT,
		CAMERA_UP,
		CAMERA_DOWN
	};

	static const float YAW = 0.0f;
	static const float PITCH = 0.0f;
	static const float SPEED = 10.0f;
	static const float SENSITIVITY = 0.15f;
	static const float ZOOM = 45.0f;

	class DebugCamera {
	public:
		DebugCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH);

		DebugCamera(float pos_x, float pos_y, float pos_z,
			float up_x, float up_y, float up_z,
			float yaw, float pitch);

		glm::vec3 getPosition() const;
		glm::vec3 getFront() const;
		glm::mat4 getViewMatrix() const;
		void processKeyboard(CameraMovement direction);
		void processMouseMovement(float x_offset, float y_offset);
		void processMouseScroll(float y_offset);

		void onInputActionEvent(Core::EventPtr e);
		void onInputStateEvent(Core::EventPtr e);
		void onInputRangeEvent(Core::EventPtr e);

		float m_delta_time;

	private:
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_world_up;

		float m_yaw;
		float m_pitch;
		float m_movement_speed;
		float m_mouse_sensitivity;
		float m_zoom;

		void updateCameraVectors();
	};
}


#endif	// GRAPHICS_DEBUG_CAMERA_HPP