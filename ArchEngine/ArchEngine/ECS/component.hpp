/*===========================================================================*
 * Arch Engine - "ECS/component.hpp                                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 12/05/2018                                                 *
 *===========================================================================*/


#ifndef ECS_COMPONENT_HPP
#define ECS_COMPONENT_HPP


#include "../Graphics/shader.hpp"

#include <bitset>
#include <string>
#include <vector>


using namespace Graphics;


namespace ECS {
	// ArchEngine component types
	enum ComponentType {
		COMPONENT_NONE,

		// General components
		COMPONENT_TRANSFORMATION_MATRIX,

		// Graphics components
		COMPONENT_GRAPHICS_SHADER,
		COMPONENT_GRAPHICS_VAO,

		COMPONENT_GRAPHICS_TEXTURE_DIFFUSE,
		COMPONENT_GRAPHICS_TEXTURE_SPECULAR,
		COMPONENT_GRAPHICS_TEXTURE_EMISSION,
		COMPONENT_GRAPHICS_TEXTURE_HEIGHT,
		COMPONENT_GRAPHICS_TEXTURE_NORMALS,
		COMPONENT_GRAPHICS_TEXTURE_DISPLACEMENT,

		COMPONENT_GRAPHICS_DIRECTIONAL_LIGHT,
		COMPONENT_GRAPHICS_POINT_LIGHT,
		COMPONENT_GRAPHICS_SPOT_LIGHT,

		// Physics components
		COMPONENT_PHYSICS_RIGID_BODY,
		COMPONENT_PHYSICS_LINEAR_VELOCITY,
		COMPONENT_PHYSICS_ANGULAR_VELOCITY,

		NUMBER_OF_DEFAULT_COMPONENTS
	};

	// Each bit is associated with a component. The Entities have a
	// ComponentMask to represent it's components
	typedef std::bitset<NUMBER_OF_DEFAULT_COMPONENTS> ComponentMask;

	// Struct for comparing ComponentMasks
	struct ComponentMaskComparer {
		bool operator() (
			const std::bitset<NUMBER_OF_DEFAULT_COMPONENTS>& b1,
			const std::bitset<NUMBER_OF_DEFAULT_COMPONENTS>& b2) const {
			return b1.to_ulong() < b2.to_ulong();
		}
	};

	//---------------------------------------------------- Arrays of components
	struct Components {
		std::vector<Shader> shaders;
	};
}


#endif	// ECS_COMPONENT_HPP