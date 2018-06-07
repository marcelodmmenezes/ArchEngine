/*===========================================================================*
 * Arch Engine - "Physics/physicsManager.hpp"                                *
 *                                                                           *
 * Class responsible for representing the physics world.                     *
 * All physics objects, collisions and simulations are contained and         *
 * calculated in a physics world.                                            *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 06/06/2018                                                 *
 *===========================================================================*/


#ifndef PHYSICS_PHYSICS_MANAGER_HPP
#define PHYSICS_PHYSICS_MANAGER_HPP


#include "../API/entity.hpp"
#include "../Config/engineMacros.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftBody.h>

#include <map>
#include <string>
#include <vector>


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


	class PhysicsManager {
	public:
		~PhysicsManager();

		PhysicsManager(const PhysicsManager&) = delete;
		void operator=(const PhysicsManager&) = delete;

		static PhysicsManager& getInstance();

		bool initialize(
			WorldType world,
			CollisionConfigType collision_config,
			BroadphaseType broadphase,
			ConstraintSolverType solver);

		bool initializeFromConfigFile(const std::string& path);
		void update(float delta_time);
		void destroy();

		void setGravity(const glm::vec3& gravity);

		//---------------------------------------------------------------- TEST
		unsigned addCube(const glm::vec3& sides,
			const glm::vec3& pos, float mass, float friction = 0.5f);

		unsigned addSphere(float radius,
			const glm::vec3& pos, float mass, float friction = 0.5f);
		//---------------------------------------------------------------------

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		PhysicsManager();
		
		State m_state;

		btDynamicsWorld* m_world;
		btDispatcher* m_dispatcher;
		btCollisionConfiguration* m_collision_config;
		btBroadphaseInterface* m_broadphase;
		btConstraintSolver* m_solver;
	};
}


#endif	// PHYSICS_PHYSICS_MANAGER_HPP