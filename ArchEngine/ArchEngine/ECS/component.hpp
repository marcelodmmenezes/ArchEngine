/*===========================================================================*
 * Arch Engine - "ECS/component.hpp                                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 06/05/2018                                                 *
 *===========================================================================*/


#ifndef ECS_COMPONENT_HPP
#define ECS_COMPONENT_HPP


#include <bitset>
#include <string>
#include <vector>


namespace ECS {
	// ArchEngine component types
	enum ComponentType {
		COMPONENT_NONE,

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

	//----------------------------------------------- Component implementations

	/* Components should be plain old data structs,
	 * but when using third party libraries this may not always be true.
	 */

	//---------------------------------------------------- Arrays of components
	struct Components {
	};
}


#endif	// ECS_COMPONENT_HPP