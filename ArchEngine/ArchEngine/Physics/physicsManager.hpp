/*===========================================================================*
 * Arch Engine - "Physics/physicsManager.hpp"                                *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 12/05/2018                                                 *
 *===========================================================================*/


#ifndef PHYSICS_PHYSICS_MANAGER_HPP
#define PHYSICS_PHYSICS_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <btBulletCollisionCommon.h>


namespace Physics {
	class PhysicsManager {
	public:
		~PhysicsManager();

		PhysicsManager(const PhysicsManager&) = delete;
		void operator=(const PhysicsManager&) = delete;

		static PhysicsManager& getInstance();

	private:
		PhysicsManager();
	};
}


#endif	// PHYSICS_PHYSICS_MANAGER_HPP