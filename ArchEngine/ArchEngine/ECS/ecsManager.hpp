/*===========================================================================*
 * Arch Engine - "ECS/ecsManager.hpp                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 11/05/2018                                                 *
 *===========================================================================*/


#ifndef ECS_ECS_MANAGER_HPP
#define ECS_ECS_MANAGER_HPP


#include "entity.hpp"
#include "component.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cassert>
#include <map>
#include <vector>


namespace ECS {
	class IProcess {
	public:
		// Evaluates the kind of entity this process needs
		ComponentMask m_mask;
		unsigned m_duration;
	};

	class ECSManager {
	public:
		~ECSManager();

		ECSManager(const ECSManager&) = delete;
		void operator=(const ECSManager&) = delete;

		static ECSManager& getInstance();

		bool initialize(const std::string& config_path);
		void update();
		void destroy();

		// CAUTION: Uses move semantics
		// Returns the index to the component array position where
		// the new component was added.
		template<typename T>
		unsigned addComponent(T& component);

		// CAUTION: Uses move semantics
		// Returns an index to the m_entities position where
		// the new entity was added.
		unsigned addEntity(ComponentMask mask, int component_handles[]);

	private:
		typedef std::vector<Entity> Entities;
		typedef std::vector<IProcess> Processes;

		ECSManager();

		Entities m_entities;
		Components m_components;
		Processes m_processes;

		// Stores a list of entities per mask.
		// The unsigned represents the index of the entity in m_entities list.
		std::multimap<ComponentMask, unsigned, ComponentMaskComparer>
			m_entities_per_mask;
	};

	//----------------------------------- addComponent template specializations
	template<typename T>
	unsigned ECSManager::addComponent(T& comp) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
		Utils::ServiceLocator::getFileLogger()->log<Utils::LOG_WARNING>(
			"Component type does not exist");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING

#ifndef	ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(false); // Should never get here
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS
	}
}


#endif	// ECS_ECS_MANAGER_HPP