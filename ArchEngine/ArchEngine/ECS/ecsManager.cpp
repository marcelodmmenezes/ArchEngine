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
		return true;
		// TODO
	}

	void ECSManager::update() {
		// TODO
	}

	void ECSManager::destroy() {
		// TODO
	}

	unsigned ECSManager::addEntity(ComponentMask mask,
		int component_handles[]) {
		return 0;
		// TODO
	}
}