/*===========================================================================*
 * Arch Engine - "Graphics/bone.cpp"                                         *
 *                                                                           *
 * Represents a skeletal animation bone.                                     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 03/06/2018                                                       *
 * Last Modified: 03/06/2018                                                 *
 *===========================================================================*/


#include "bone.hpp"


namespace Graphics {
	Bone::Bone(unsigned mesh_id, const std::string& nome) {
	}

	glm::mat4 Bone::getParentTransforms() {
		Bone* b = m_parent;
		std::vector<glm::mat4> matrices;

		while (b) {
			matrices.push_back(b->m_transform);
			b = b->m_parent;
		}

		glm::mat4 concatenated;

		for (int i = matrices.size() - 1; i >= 0; i--)
			concatenated *= matrices[i];

		return concatenated;
	}
}