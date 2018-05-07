/*===========================================================================*
 * Arch Engine - "ECS/ecsManager.cpp                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 06/05/2018                                                 *
 *===========================================================================*/


#include "ecsManager.hpp"


namespace ECS {
	ECSManager::ECSManager() {}

	ECSManager::~ECSManager() {}

	ECSManager& ECSManager::getInstance() {
		static ECSManager instance;
		return instance;
	}

	bool ECSManager::initialize(const std::string& config_path) {
		return true; // TODO
	}

	void ECSManager::update() {

	}

	void ECSManager::destroy() {

	}

	unsigned ECSManager::addEntity(ComponentMask mask,
		int component_handles[]) {
		Entity entity;

		entity.m_mask = std::move(mask);
		for (unsigned i = 0; i < NUMBER_OF_DEFAULT_COMPONENTS; i++)
			entity.m_component_handles[i] = component_handles[i];

		m_entities.push_back(entity);
		m_entities_per_mask.insert(
			std::make_pair(mask, m_entities.size() - 1));

		return m_entities.size() - 1;
	}

	void ECSManager::removeEntity(unsigned entity_handle) {
		// Order doesn't matter, so we can change with last entity
		// and remove it, to avoid unnecessary copies.

	}
}