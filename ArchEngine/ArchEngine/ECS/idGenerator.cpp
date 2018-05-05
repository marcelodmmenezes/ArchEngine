/*===========================================================================*
 * Arch Engine - "ECS/idGenerator.hpp                                        *
 *                                                                           *
 * Class responsible for generating unique identifiers to the ECS.           *
 * The class template is used to create ids for entities, components and     *
 * systems. The generateId method template uses a static aproach to generate *
 * unique counters to each type of entity, component or system.              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 05/05/2018                                                 *
 *===========================================================================*/


#include "idGenerator.hpp"


namespace ECS {
	// Static members initialization
	template<>
	IdType IdGenerator<IEntity>::s_count = ID_TYPE_INITIAL_VALUE;
	template<>
	IdType IdGenerator<IComponent>::s_count = ID_TYPE_INITIAL_VALUE;
	template<>
	IdType IdGenerator<ISystem>::s_count = ID_TYPE_INITIAL_VALUE;
}