/*===========================================================================*
 * Arch Engine - "ECS/iEntity.hpp                                            *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 30/04/2018                                                 *
 *===========================================================================*/


#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP


#include "component.hpp"


namespace ECS {
	struct Entity {
		// Used for fast evaluation of what components the entity has.
		ComponentMask m_mask;
		// Index of it's components in the components arrays.
		unsigned m_component_handles[NUMBER_OF_DEFAULT_COMPONENTS];
	};
}


#endif	// ECS_ENTITY_HPP