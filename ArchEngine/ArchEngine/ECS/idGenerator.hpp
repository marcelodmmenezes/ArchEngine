/*===========================================================================*
 * Arch Engine - "ECS/idGenerator.hpp                                        *
 *                                                                           *
 * Class responsible for generating unique identifiers to the ECS.           *
 * The class template is used to create ids for entities, components and     *
 * systems. The generateId method template uses a static aproach to generate *
 * unique ids to each type of entity, component or system.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 05/05/2018                                                 *
 *===========================================================================*/


#ifndef ECS_ID_GENERATOR_HPP
#define ECS_ID_GENERATOR_HPP


#include "iEntity.hpp"
#include "iComponent.hpp"
#include "iSystem.hpp"


namespace ECS {
	typedef unsigned long IdType;
	static const IdType ID_TYPE_INITIAL_VALUE = 0ul;

	// Template for different IdGenerators for entities, components or systems.
	template<typename T>
	class IdGenerator {
	public:
		// 'Static class'
		IdGenerator() = delete;
		IdGenerator(const IdGenerator&) = delete;
		void operator=(const IdGenerator&) = delete;

		// Auto-increment id for each different type of entity, component or
		// system.
		template<typename U>
		static const IdType generateId() {
			static const IdType id = s_count++;
			return id;
		}

		static const IdType count() {
			return s_count;
		}

	private:
		static IdType s_count;
	};
}


#endif	// ECS_ID_GENERATOR