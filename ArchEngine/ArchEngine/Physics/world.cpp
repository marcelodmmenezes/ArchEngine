/*===========================================================================*
 * Arch Engine - "Physics/world.cpp"                                         *
 *                                                                           *
 * Class responsible for representing the physics world.                     *
 * All physics objects, collisions and simulations are contained and         *
 * calculated in a physics world.                                            *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 04/06/2018                                                 *
 *===========================================================================*/


#include "world.hpp"


using namespace Script;
using namespace Utils;


namespace Physics {
	World::World() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Physics::World constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	World::~World() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Physics::World destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool World::initialize(
		WorldType world,
		CollisionConfigType collision_config,
		BroadphaseType broadphase,
		ConstraintSolverType solver) {
		// TODO: fill in the missing cases

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		switch (collision_config) {
		case DEFAULT_COLLISION_CONFIGURATION:
			m_collision_config = new btDefaultCollisionConfiguration();
			break;
		case SOFT_RIGID_COLLISION_CONFIGURATION:
			break;
		}

		m_dispatcher = new btCollisionDispatcher(m_collision_config);

		switch (broadphase) {
		case AXIS_SWEEP_BROADPHASE:
			break;
		case DBVT_BROADPHASE:
			m_broadphase = new btDbvtBroadphase();
			break;
		case MULTI_SAP_BROADPHASE:
			break;
		case SIMPLE_BROADPHASE:
			break;
		}

		switch (solver) {
		case SEQUENTIAL_IMPULSE_SOLVER:
			m_solver = new btSequentialImpulseConstraintSolver();
			break;
		case MLCP_SOLVER:
			break;
		case MULTI_BODY_SOLVER:
			break;
		case NNGC_SOLVER:
			break;
		}

		switch (world) {
		case WORLD_DISCRETE_DYNAMICS:
			m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase,
				m_solver, m_collision_config);
			break;
		case WORLD_SOFT_RIGID_DYNAMICS:
			break;
		}

		m_state = INITIALIZED;

		return true;
	}

	void World::update(float delta_time) {
		m_world->stepSimulation(delta_time);
	}

	void World::destroy() {
		// TODO

		m_state = SAFE_TO_DESTROY;
	}
}