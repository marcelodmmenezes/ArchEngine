/*===========================================================================*
 * Arch Engine - "Physics/physicsManager.cpp"                                *
 *                                                                           *
 * Class responsible for representing the physics world.                     *
 * All physics objects, collisions and simulations are contained and         *
 * calculated in a physics world.                                            *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 06/06/2018                                                 *
 *===========================================================================*/


#include "physicsManager.hpp"


using namespace Script;
using namespace Utils;


namespace Physics {
	void btTransformToGlmMat4(const btTransform& bt, glm::mat4& gm) {
		btScalar m[16];
		bt.getOpenGLMatrix(m);

		gm = glm::mat4(
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]
		);
	}

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
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
				ServiceLocator::getFileLogger()->log<LOG_WARNING>(
					"Unidentified value in physics config: " + it.first);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			}
		}

		if (!initialize((WorldType)world_type,
			(CollisionConfigType)collision_config,
			(BroadphaseType)broadphase,
			(ConstraintSolverType)solver))
			return false;

		auto gravity = lua_context.getFloatVector("gravity");
		m_world->setGravity(btVector3(gravity[0], gravity[1], gravity[2]));

		lua_context.destroy();

		return true;
	}

	void PhysicsManager::update(float delta_time) {
		m_world->stepSimulation(delta_time, 10.0f);

		auto objs = m_world->getCollisionObjectArray();

		for (auto& it : g_entities)
			for (unsigned i = 0; i < it.bodies.size(); i++)
				btTransformToGlmMat4(objs[it.bodies[i]]->getWorldTransform(),
					it.transforms[i]);
	}

	void PhysicsManager::destroy() {
		for (int i = m_world->getNumCollisionObjects() - 1; i >= 0; i--) {
			btCollisionObject* obj = m_world->getCollisionObjectArray()[i];

			m_world->removeCollisionObject(obj);

			btRigidBody* body = btRigidBody::upcast(obj);

			if (body) {
				delete body->getMotionState();
				delete body->getCollisionShape();
			}
			else {
				btSoftBody* body = btSoftBody::upcast(obj);

				if (body)
					delete body->getCollisionShape();
			}

			delete obj;
		}

		delete m_world;
		delete m_solver;
		delete m_broadphase;
		delete m_dispatcher;
		delete m_collision_config;

		m_state = SAFE_TO_DESTROY;
	}

	void PhysicsManager::setGravity(const glm::vec3& gravity) {
		m_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}

	//---------------------------------------------------------------- TEST
	unsigned PhysicsManager::addCube(const glm::vec3& sides,
		const glm::vec3& pos, float mass, float friction) {
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(pos.x, pos.y, pos.z));

		btBoxShape* cube = new btBoxShape(
			btVector3(sides.x, sides.y, sides.z));

		btVector3 inertia(0.0f, 0.0f, 0.0f);
		if (mass != 0.0f)
			cube->calculateLocalInertia(mass, inertia);

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(
			mass, motion, cube, inertia);

		info.m_friction = friction;
		btRigidBody* body = new btRigidBody(info);

		m_world->addRigidBody(body);

		return m_world->getNumCollisionObjects() - 1;
	}

	unsigned PhysicsManager::addSphere(float radius,
		const glm::vec3& pos, float mass, float friction) {
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(pos.x, pos.y, pos.z));

		btSphereShape* sphere = new btSphereShape(radius);

		btVector3 inertia(0.0f, 0.0f, 0.0f);
		if (mass != 0.0f)
			sphere->calculateLocalInertia(mass, inertia);

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(
			mass, motion, sphere, inertia);

		info.m_friction = friction;
		btRigidBody* body = new btRigidBody(info);

		m_world->addRigidBody(body);

		return m_world->getNumCollisionObjects() - 1;
	}
	//---------------------------------------------------------------------
}