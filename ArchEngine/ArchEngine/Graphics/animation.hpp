/*===========================================================================*
 * Arch Engine - "Graphics/animation.hpp"                                    *
 *                                                                           *
 * Class which contains bones, and info for animating a given model.         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 02/06/2018                                                       *
 * Last Modified: 02/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_ANIMATED_MESH_HPP
#define GRAPHICS_ANIMATED_MESH_HPP


#include "../Config/engineMacros.hpp"
#include "mesh.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>

#include <map>
#include <string>
#include <vector>


namespace Graphics {
	// Maximum of 4 vertex influence
	const unsigned WEIGHTS_PER_VERTEX = 4u;

	struct Bone {
		unsigned ids[WEIGHTS_PER_VERTEX];
		float weights[WEIGHTS_PER_VERTEX];
		glm::mat4 offset;
	};

	struct Animation {
		// Corresponding GraphicsManager mesh id
		unsigned m_mesh_id;

		// Number of bones
		unsigned m_num_bones;

		// Root node inverse transformation matrix
		glm::mat4 m_global_inverse_transform;

		// Mesh bones
		std::vector<Bone> m_bones;

		// Maps bone name to it's id
		std::map<std::string, unsigned> m_bone_mapping;
	};
}


#endif	// GRAPHICS_ANIMATED_MESH_HPP