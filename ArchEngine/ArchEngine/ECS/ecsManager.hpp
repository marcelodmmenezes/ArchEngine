/*===========================================================================*
 * Arch Engine - "ECS/ecsManager.hpp                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 06/05/2018                                                 *
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
	Components g_components;

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
		void removeEntity(unsigned entity_handle);

	private:
		ECSManager();

		// Stores a list of entities per mask.
		// The unsigned represents the index of the entity in m_entities list.
		std::multimap<ComponentMask, unsigned, ComponentMaskComparer>
			m_entities_per_mask;

		std::vector<Entity> m_entities;
	};

	//----------------------------------- addComponent template specializations
	template<typename T>
	unsigned ECSManager::addComponent(T& comp) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
		Utils::ServiceLocator.getFileLogger()->log<Utils::LOG_WARNING>(
			"Component type does not exist");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING

#ifndef	ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(false); // Should never get here
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS
	}

	template<>
	unsigned ECSManager::addComponent<ComponentTest1>(ComponentTest1& comp) {
		g_components.m_test1.push_back(std::move(comp));
		return g_components.m_test1.size() - 1;
	}

	template<>
	unsigned ECSManager::addComponent<ComponentTest2>(ComponentTest2& comp) {
		g_components.m_test2.push_back(std::move(comp));
		return g_components.m_test2.size() - 1;
	}

	template<>
	unsigned ECSManager::addComponent<ComponentTest3>(ComponentTest3& comp) {
		g_components.m_test3.push_back(std::move(comp));
		return g_components.m_test3.size() - 1;
	}
}


#endif	// ECS_ECS_MANAGER_HPP