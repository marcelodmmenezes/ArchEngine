/*===========================================================================*
 * Arch Engine - "Physics/physicsManager.cpp"                                *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 04/06/2018                                                 *
 *===========================================================================*/


#include "physicsManager.hpp"


using namespace Script;
using namespace Utils;


namespace Physics {
	PhysicsManager::PhysicsManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"PhysicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	PhysicsManager::~PhysicsManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"PhysicsManager destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	PhysicsManager& PhysicsManager::getInstance() {
		static PhysicsManager instance;
		return instance;
	}

	bool PhysicsManager::initializeFromConfigFile(const std::string& path) {
		bool success = true;

		LuaScript lua_context;
		lua_context.initialize(path);

		auto worlds = lua_context.getTableKeys("worlds");

		// Default physics world values
		int world_type = 0, collision_config = 0, broadphase = 1, solver = 0;

		for (auto& it : worlds) {
			auto properties = lua_context.getTablePairs("worlds." + it);
			
			for (auto& it : properties) {
				if (it.first == "world_type")
					world_type = std::stoi(it.second);
				else if (it.first == "collision_config")
					collision_config = std::stoi(it.second);
				else if (it.first == "broadphase")
					broadphase = std::stoi(it.second);
				else if (it.first == "solver")
					solver = std::stoi(it.second);
				else {
					// If it.first is not in the above ifs,
					// the input file is wrong.
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
					ServiceLocator::getFileLogger()->log<LOG_ERROR>(
						"Could not create " + path + " physics world");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
					// Should never get here
					assert(false);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS
				}
			}

			if (!addWorld(world_type, collision_config, broadphase, solver))
				success = false;
		}

		lua_context.destroy();

		return success;
	}

	void PhysicsManager::destroy() {
		for (auto& it : m_worlds)
			it.destroy();
	}

	bool PhysicsManager::addWorld(int world_type,
		int collision_config, int broadphase, int solver) {
		m_worlds.push_back(std::move(World()));
		return m_worlds[m_worlds.size() - 1].initialize(
			(WorldType)world_type,
			(CollisionConfigType)collision_config,
			(BroadphaseType)broadphase,
			(ConstraintSolverType)solver
		);
	}
}