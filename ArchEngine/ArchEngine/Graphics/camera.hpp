/*===========================================================================*
 * Arch Engine - "Graphics/camera.hpp"                                       *
 *                                                                           *
 * TODO: description.                                                        *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/06/2018                                                       *
 * Last Modified: 16/06/2018                                                 *
 *===========================================================================*/


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Graphics {
	enum CameraMovement {
		CAMERA_FORWARD,
		CAMERA_BACKWARD,
		CAMERA_LEFT,
		CAMERA_RIGHT,
		CAMERA_UP,
		CAMERA_DOWN
	};

	const float YAW = 0.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.0f;
	const float ZOOM = 45.0f;

	class Camera {
	public:
		virtual ~Camera() = 0;

		glm::vec3 getPosition() const;
		glm::vec3 getFront() const;
		glm::mat4 getViewMatrix() const;

		void setSpeed(float speed);
		float getSpeed();

		float m_delta_time;

	protected:
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_world_up;

		float m_yaw;
		float m_pitch;
		float m_movement_speed;
		float m_zoom;
	};
}