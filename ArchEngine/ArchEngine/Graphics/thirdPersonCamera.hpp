/*===========================================================================*
 * Arch Engine - "Graphics/thirdPersonCamera.hpp"                            *
 *                                                                           *
 * Third person camera.                                                      *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/06/2018                                                       *
 * Last Modified: 17/06/2018                                                 *
 *===========================================================================*/


#include "camera.hpp"


#include <glm/glm.hpp>


namespace Graphics {
	class ThirdPersonCamera : public Camera {
	public:
		ThirdPersonCamera(float distance_from_target,
			float yaw = YAW, float pitch = PITCH);
		~ThirdPersonCamera();

		void update(const glm::vec3& target);
		void calcYaw(float x);
		void calcPitch(float y);

		glm::mat4 getViewMatrix() const override;

		void increaseDistanceFromTarget(float delta);
		float getDistanceFromTarget() const;

		void setInterpolationFactor(float factor);
		float getInterpolationFactor() const;

	private:
		float m_distance_from_target;
		float m_interpolation_factor;
		glm::vec3 m_old_target_pos;
	};
}
