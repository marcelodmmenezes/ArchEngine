/*===========================================================================*
 * Arch Engine - "Physics/physicsManager.cpp"                                *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 05/06/2018                                                 *
 *===========================================================================*/


#include "physicsManager.hpp"


using namespace Script;
using namespace Utils;


namespace Physics {
	PhysicsManager::PhysicsManager() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"PhysicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	PhysicsManager::~PhysicsManager() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"PhysicsManager destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	PhysicsManager& PhysicsManager::getInstance() {
		static PhysicsManager instance;
		return instance;
	}

	bool PhysicsManager::initialize(
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

	bool PhysicsManager::initializeFromConfigFile(const std::string& path) {
		LuaScript lua_context;
		lua_context.initialize(path);

		// Default physics world values
		int world_type = 0, collision_config = 0, broadphase = 1, solver = 0;

		auto properties = lua_context.getTablePairs("world");

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

		if (!initialize((WorldType)world_type,
			(CollisionConfigType)collision_config,
			(BroadphaseType)broadphase,
			(ConstraintSolverType)solver))
			return false;

		lua_context.destroy();

		return true;
	}

	void PhysicsManager::update(float delta_time) {
		m_world->stepSimulation(delta_time);
	}

	void PhysicsManager::destroy() {
		auto objects = m_world->getCollisionObjectArray();

		for (int i = m_world->getNumCollisionObjects() - 1; i >= 0; i--) {
			btCollisionObject* obj = objects[i];

			btRigidBody* body = dynamic_cast<btRigidBody*>(obj);

			if (body) {
				m_world->removeCollisionObject(obj);
				btMotionState* motion = body->getMotionState();
				btCollisionShape* shape = body->getCollisionShape();
				delete shape;
				delete motion;
				delete obj;
			}
			else {
				btSoftBody* body = dynamic_cast<btSoftBody*>(obj);

				m_world->removeCollisionObject(body);
				btCollisionShape* shape = body->getCollisionShape();
				delete shape;
				delete obj;
			}
		}

		delete m_world;
		delete m_solver;
		delete m_broadphase;
		delete m_dispatcher;
		delete m_collision_config;

		m_state = SAFE_TO_DESTROY;
	}
}