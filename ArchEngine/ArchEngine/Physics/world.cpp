/*===========================================================================*
 * Arch Engine - "Physics/world.cpp"                                         *
 *                                                                           *
 * Class responsible for representing the physics world.                     *
 * All physics objects, collisions and simulations are contained and         *
 * calculated in a physics world.                                            *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 12/05/2018                                                 *
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
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		switch (world) {
		case WORLD_DISCRETE_DYNAMICS:
			break;
		case WORLD_SOFT_RIGID_DYNAMICS:
			break;
		}

		switch (collision_config) {
		case DEFAULT_COLLISION_CONFIGURATION:
			break;
		case SOFT_RIGID_COLLISION_CONFIGURATION:
			break;
		}

		switch (broadphase) {
		case AXIS_SWEEP_BROADPHASE:
			break;
		case DBVT_BROADPHASE:
			break;
		case MULTI_SAP_BROADPHASE:
			break;
		case SIMPLE_BROADPHASE:
			break;
		}

		switch (solver) {
		case SEQUENTIAL_IMPULSE_SOLVER:
			break;
		case MLCP_SOLVER:
			break;
		case MULTI_BODY_SOLVER:
			break;
		case NNGC_SOLVER:
			break;
		}

		m_state = INITIALIZED;

		return true;
	}

	bool World::initializeFromConfigFile(const std::string& path) {
		LuaScript lua_context;
		lua_context.initialize(path);

		initialize(
			(WorldType)lua_context.get<int>("world"),
			(CollisionConfigType)lua_context.get<int>("collision_config"),
			(BroadphaseType)lua_context.get<int>("broadphase"),
			(ConstraintSolverType)lua_context.get<int>("solver"));

		lua_context.destroy();

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