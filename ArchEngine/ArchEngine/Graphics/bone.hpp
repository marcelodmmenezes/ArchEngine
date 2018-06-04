/*===========================================================================*
 * Arch Engine - "Graphics/bone.hpp"                                         *
 *                                                                           *
 * Represents a skeletal animation bone.                                     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 03/06/2018                                                       *
 * Last Modified: 03/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_BONE_HPP
#define GRAPHICS_BONE_HPP


#include <glm/glm.hpp>

#include <string>
#include <vector>


namespace Graphics {
	class Bone {
	public:
		Bone(unsigned mesh_id, const std::string& nome);
		
		glm::mat4 getParentTransforms();

		std::string m_name;
		glm::mat4 m_transform;

	private:
		// Id of related GraphicsManager's mesh.
		unsigned m_mesh_id;

		Bone* m_parent;
		glm::mat4 m_parent_transforms;
	};
}


#endif	// GRAPHICS_BONE_HPP