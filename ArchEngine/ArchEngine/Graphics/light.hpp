/*===========================================================================*
 * Arch Engine - "Graphics/light.hpp"                                        *
 *                                                                           *
 * Structs for various types of light representation.                        *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#include <glm/glm.hpp>


namespace Graphics {
	struct DirectionalLight {
		glm::vec3 direction;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool emit_shadows;
	};

	struct PointLight {
		glm::vec3 position;

		// Attenuation properties
		float constant;
		float linear;
		float quadratic;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool emit_shadows;
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;

		// SpotLight radiuses
		float cut_off;
		float outer_cut_off;

		// Attenuation properties
		float constant;
		float linear;
		float quadratic;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool emit_shadows;
	};
}