/*===========================================================================*
 * Arch Engine - "Graphics/debugCamera.hpp"                                  *
 *                                                                           *
 * First person camera for scene debugging.                                  *
 * Based in:                                                                 *
 * - (https://learnopengl.com/Getting-started/Camera)                        *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 16/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_DEBUG_CAMERA_HPP
#define GRAPHICS_DEBUG_CAMERA_HPP


#include "../Core/eventManager.hpp"
#include "camera.hpp"
#include "../OS/inputManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Graphics {
	class DebugCamera : public Camera {
	public:
		DebugCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH);

		DebugCamera(float pos_x, float pos_y, float pos_z,
			float up_x, float up_y, float up_z,
			float yaw, float pitch);

		~DebugCamera();

		void move(CameraMovement direction);
		void look(float x_offset, float y_offset);
		void zoom(float y_offset);

	private:
		void updateCameraVectors();
	};
}


#endif	// GRAPHICS_DEBUG_CAMERA_HPP