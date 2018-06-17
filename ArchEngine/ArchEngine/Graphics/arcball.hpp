/*===========================================================================*
 * Arch Engine - "Graphics/arcball.hpp"                                      *
 *                                                                           *
 * Arcball camera.                                                           *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 15/06/2018                                                       *
 * Last Modified: 16/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_ARCBALL_HPP
#define GRAPHICS_ARCBALL_HPP


#include "../Config/engineMacros.hpp"
#include "camera.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <algorithm>
#include <cstdlib>


namespace Graphics {
	const double PI = 3.14159265358979323846;

	class Arcball : public Camera {
	public:
		Arcball(int width, int height, float radius,
			const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 5.0f),
			const glm::vec3& front = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
		~Arcball();

		void setWidthHeight(int width, int height);
		void setRadius(float radius);

		void updateRotation(const glm::vec2& pos);
		void stopRotation();
		void calc();
		void reset();

	private:
		void startRotation(const glm::vec2& pos);
		glm::vec3 convertCoords(const glm::vec2& coords);

		int m_width;
		int m_height;
		float m_radius;

		bool m_is_rotating;

		glm::vec3 m_start_rotation_vector;
		glm::vec3 m_current_rotation_vector;
	};
}


#endif	// GRAPHICS_ARCBALL_HPP