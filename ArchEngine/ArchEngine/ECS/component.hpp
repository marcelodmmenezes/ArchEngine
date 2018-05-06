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

		COMPONENT_TEST_1,
		COMPONENT_TEST_2,
		COMPONENT_TEST_3,

		NUMBER_OF_DEFAULT_COMPONENTS
	};

	// Each bit is associated with a component. The Entities have a
	// ComponentMask to represent it's components
	typedef std::bitset<NUMBER_OF_DEFAULT_COMPONENTS> ComponentMask;

	//----------------------------------------------- Component implementations

	/* Components should be plain old data structs,
	 * but when using third party libraries this may not always be true.
	 */

	struct ComponentTest1 {
		int m_x;
		int m_y;
	};

	struct ComponentTest2 {
		float m_value;
	};

	struct ComponentTest3 {
		std::string m_name;
		bool m_valid;
		int m_u;
		int m_v;
	};

	//---------------------------------------------------- Arrays of components
	struct Components {
		std::vector<ComponentTest1> m_test1;
		std::vector<ComponentTest2> m_test2;
		std::vector<ComponentTest3> m_test3;
	};
}


#endif	// ECS_COMPONENT_HPP