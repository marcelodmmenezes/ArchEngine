/*===========================================================================*
 * Arch Engine - "Graphics/skeleton.hpp"                                     *
 *                                                                           *
 * Represents a skeletal animation bone collection.                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 03/06/2018                                                       *
 * Last Modified: 03/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_SKELETON_HPP
#define GRAPHICS_SKELETON_HPP


#include "bone.hpp"


namespace Graphics {
	class Skeleton {
	public:
		std::vector<Bone> m_bones;
		glm::mat4 m_global_inverse_transform;

	private:
	};
}


#endif	// GRAPHICS_SKELETON_HPP