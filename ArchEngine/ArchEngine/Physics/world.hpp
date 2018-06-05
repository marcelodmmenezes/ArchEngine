/*===========================================================================*
 * Arch Engine - "Physics/world.hpp"                                         *
 *                                                                           *
 * Class responsible for representing the physics world.                     *
 * All physics objects, collisions and simulations are contained and         *
 * calculated in a physics world.                                            *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 04/06/2018                                                 *
 *===========================================================================*/


#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP


#include "../Config/engineMacros.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <btBulletDynamicsCommon.h>

#include <string>


namespace Physics {
	enum WorldType {
		WORLD_DISCRETE_DYNAMICS,
		WORLD_SOFT_RIGID_DYNAMICS
	};

	enum CollisionConfigType {
		DEFAULT_COLLISION_CONFIGURATION,
		SOFT_RIGID_COLLISION_CONFIGURATION
	};

	enum BroadphaseType {
		AXIS_SWEEP_BROADPHASE,
		DBVT_BROADPHASE,
		MULTI_SAP_BROADPHASE,
		SIMPLE_BROADPHASE,
	};

	enum ConstraintSolverType {
		SEQUENTIAL_IMPULSE_SOLVER,
		MLCP_SOLVER,
		MULTI_BODY_SOLVER,
		NNGC_SOLVER
	};


	class World {
	public:
		World();
		~World();

		bool initialize(
			WorldType world,
			CollisionConfigType collision_config,
			BroadphaseType broadphase,
			ConstraintSolverType solver);

		void update(float delta_time);

		void destroy();

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		State m_state;

		btDynamicsWorld* m_world;
		btDispatcher* m_dispatcher;
		btCollisionConfiguration* m_collision_config;
		btBroadphaseInterface* m_broadphase;
		btConstraintSolver* m_solver;
	};
}


#endif	// PHYSICS_WORLD_HPP