/*===========================================================================*
 * Arch Engine - "Graphics/arcball.hpp"                                      *
 *                                                                           *
 * Arcball camera.                                                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 15/06/2018                                                       *
 * Last Modified: 16/06/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <algorithm>
#include <cstdlib>


namespace Graphics {
	const double PI = 3.14159265358979323846;

	class Arcball {
	public:
		Arcball(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 5.0f),
			const glm::vec3& focus = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
		~Arcball();

		void setWidthHeight(int width, int height);
		void setRadius(float radius);

		void startRotation(const glm::vec2& pos);
		void updateRotation(const glm::vec2& pos);
		void stopRotation();

		void calc();

		glm::vec3 getPosition() const;
		glm::vec3 getFocus() const;
		glm::mat4 getViewMatrix() const;

		void reset();

		float m_delta_time;

	private:
		glm::vec3 convertCoords(const glm::vec2& coords);

		int m_width;
		int m_height;
		float m_radius;

		bool m_is_rotating;

		glm::vec3 m_start_rotation_vector;
		glm::vec3 m_current_rotation_vector;

		glm::vec3 m_position;
		glm::vec3 m_focus;
		glm::vec3 m_up;
		glm::mat4 m_view_matrix;
	};
}