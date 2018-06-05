/*===========================================================================*
 * Arch Engine - "Physics/physicsManager.hpp"                                *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 04/06/2018                                                 *
 *===========================================================================*/


#ifndef PHYSICS_PHYSICS_MANAGER_HPP
#define PHYSICS_PHYSICS_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "world.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <btBulletCollisionCommon.h>

#include <string>
#include <vector>


namespace Physics {
	class PhysicsManager {
	public:
		~PhysicsManager();

		PhysicsManager(const PhysicsManager&) = delete;
		void operator=(const PhysicsManager&) = delete;

		static PhysicsManager& getInstance();
		
		bool initializeFromConfigFile(const std::string& path);
		void destroy();

		bool addWorld(int world_type, int collision_config,
			int broadphase, int solver);

	private:
		PhysicsManager();

		unsigned m_active_world;

		std::vector<World> m_worlds;
	};
}


#endif	// PHYSICS_PHYSICS_MANAGER_HPP