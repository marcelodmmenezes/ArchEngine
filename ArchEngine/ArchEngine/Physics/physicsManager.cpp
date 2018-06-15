/*===========================================================================*
 * Arch Engine - "Physics/physicsManager.cpp"                                *
 *                                                                           *
 * Class responsible for representing the physics world.                     *
 * All physics objects, collisions and simulations are contained and         *
 * calculated in a physics world.                                            *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 14/06/2018                                                 *
 *===========================================================================*/


#include "physicsManager.hpp"


using namespace Core;
using namespace Graphics;
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

	bool collisionCallback(btManifoldPoint& cp,
		const btCollisionObjectWrapper *obj1, int id1, int index1,
		const btCollisionObjectWrapper *obj2, int id2, int index2) {
		auto pobj1 =
			static_cast<PhysicsObject*>(
				obj1->getCollisionObject()->getUserPointer());

		auto pobj2 =
			static_cast<PhysicsObject*>(
				obj2->getCollisionObject()->getUserPointer());

		if (!pobj1 || !pobj2)
			return false;

		EventPtr evnt = std::make_shared<CollisionEvent>(
			CollisionEvent(pobj1->id, pobj2->id));
		EventManager::getInstance().sendEvent(evnt);

		return false;
	}

	PhysicsManager::PhysicsManager() : m_state(CONSTRUCTED),
		m_picking_constraint(nullptr), m_picked_obj(nullptr) {
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

		gContactAddedCallback = Physics::collisionCallback;

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
			for (unsigned i = 0; i < it.physics_objects.size(); i++)
				btTransformToGlmMat4(
					objs[it.physics_objects[i]]->getWorldTransform(),
					it.transforms[i]);
	}

	void PhysicsManager::destroy() {
		for (auto& it : m_user_objects)
			delete it;

		// Assert that there arent constraints before destroying the world
		removePickingConstraint();

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

	void PhysicsManager::closestObjectRayTest(const glm::vec3& pos,
		const glm::vec3& front) {
		btCollisionWorld::ClosestRayResultCallback
			rayCallback(btVector3(pos.x, pos.y, pos.z),
				btVector3(front.x, front.y, front.z));

		m_world->rayTest(btVector3(pos.x, pos.y, pos.z),
			btVector3(front.x, front.y, front.z), rayCallback);

		if (rayCallback.hasHit()) {
			auto obj = static_cast<PhysicsObject*>(
				rayCallback.m_collisionObject->getUserPointer());

			if (!obj)
				return;

			EventPtr evnt = std::make_shared<ClosestRayTestEvent>(
				ClosestRayTestEvent(obj->id));
			EventManager::getInstance().sendEvent(evnt);
		}
	}

	void PhysicsManager::allObjectsRayTest(const glm::vec3& pos,
		const glm::vec3& front) {
		btCollisionWorld::AllHitsRayResultCallback
			rayCallback(btVector3(pos.x, pos.y, pos.z),
				btVector3(front.x, front.y, front.z));

		m_world->rayTest(btVector3(pos.x, pos.y, pos.z),
			btVector3(front.x, front.y, front.z), rayCallback);

		if (rayCallback.hasHit()) {
			std::vector<long> objs;
			objs.resize(rayCallback.m_collisionObjects.size());

			for (int i = 0; i < rayCallback.m_collisionObjects.size(); i++) {
				auto obj = static_cast<PhysicsObject*>(
					rayCallback.m_collisionObjects[i]->getUserPointer());

				if (!obj)
					continue;

				objs[i] = obj->id;
			}

			EventPtr evnt = std::make_shared<AllRayTestEvent>(
				AllRayTestEvent(std::move(objs)));
			EventManager::getInstance().sendEvent(evnt);
		}
	}

	void PhysicsManager::createPickingConstraint(int x, int y,
		float ray_size, bool limit_angular_motion) {
		auto camera = GraphicsManager::getInstance().getActiveCamera();

		int width, height;
		GraphicsManager::getInstance().getScreenSize(width, height);

		float n_x = (2.0f * x) / width - 1.0f;
		float n_y = 1.0f - (2.0f * y) / height;

		glm::vec4 ray_eye = glm::inverse(
			GraphicsManager::getInstance().getProjectionMatrix()) *
			glm::vec4(n_x, n_y, -1.0f, 1.0f);

		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

		glm::vec3 ray_world = glm::normalize(
			glm::vec3(glm::inverse(camera->getViewMatrix()) * ray_eye));

		auto from = camera->getPosition();
		auto to = from + ray_size * ray_world;

		btCollisionWorld::ClosestRayResultCallback
			rayCallback(btVector3(from.x, from.y, from.z),
				btVector3(to.x, to.y, to.z));

		m_world->rayTest(btVector3(from.x, from.y, from.z),
			btVector3(to.x, to.y, to.z), rayCallback);

		PhysicsObject* physics_obj = nullptr;

		if (rayCallback.hasHit()) {
			physics_obj = static_cast<PhysicsObject*>(
				rayCallback.m_collisionObject->getUserPointer());

			if (!physics_obj)
				return;

			m_picked_obj = static_cast<btRigidBody*>(
				m_world->getCollisionObjectArray()
				[physics_obj->m_world_id]);

			if (!m_picked_obj)
				return;

			m_picked_obj->setActivationState(DISABLE_DEACTIVATION);

			btVector3 local_pivot =
				m_picked_obj->getCenterOfMassTransform().inverse() *
				rayCallback.m_hitPointWorld;

			btTransform pivot;
			pivot.setIdentity();
			pivot.setOrigin(local_pivot);

			btGeneric6DofConstraint* dof6 =
				new btGeneric6DofConstraint(*m_picked_obj, pivot, true);

			if (limit_angular_motion) {
				dof6->setAngularLowerLimit(btVector3(0, 0, 0));
				dof6->setAngularUpperLimit(btVector3(0, 0, 0));
			}

			m_world->addConstraint(dof6, true);

			m_picking_constraint = dof6;

			float cfm = 0.5f;
			dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 0);
			dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 1);
			dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 2);
			dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 3);
			dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 4);
			dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 5);

			float erp = 0.5f;
			dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 0);
			dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 1);
			dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 2);
			dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 3);
			dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 4);
			dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 5);

			auto cam_pos = camera->getPosition();

			m_old_picking_dist = (rayCallback.m_hitPointWorld -
				btVector3(cam_pos.x, cam_pos.y, cam_pos.z)).length();
		}
	}

	void PhysicsManager::pickingMotion(int x, int y, float ray_size) {
		if (m_picked_obj) {
			btGeneric6DofConstraint* pick_constraint =
				static_cast<btGeneric6DofConstraint*>(m_picking_constraint);

			if (!pick_constraint)
				return;

			auto camera = GraphicsManager::getInstance().getActiveCamera();

			int width, height;
			GraphicsManager::getInstance().getScreenSize(width, height);

			float n_x = (2.0f * x) / width - 1.0f;
			float n_y = 1.0f - (2.0f * y) / height;

			glm::vec4 ray_eye = glm::inverse(
				GraphicsManager::getInstance().getProjectionMatrix()) *
				glm::vec4(n_x, n_y, -1.0f, 1.0f);

			ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

			glm::vec3 ray_world = glm::normalize(
				glm::vec3(glm::inverse(camera->getViewMatrix()) * ray_eye));

			auto from = camera->getPosition();
			auto to = from + ray_size * ray_world;

			glm::vec3 glm_dir = to - from;

			btVector3 dir(glm_dir.x, glm_dir.y, glm_dir.z);
			dir.normalize();

			dir *= m_old_picking_dist;
			btVector3 new_pivot =
				btVector3(from.x, from.y, from.z) + dir;
			pick_constraint->getFrameOffsetA().setOrigin(new_pivot);
		}
	}

	void PhysicsManager::removePickingConstraint() {
		if (!m_picking_constraint || !m_world)
			return;

		m_world->removeConstraint(m_picking_constraint);
		
		delete m_picking_constraint;

		m_picked_obj->forceActivationState(ACTIVE_TAG);
		m_picked_obj->setDeactivationTime(0.0f);

		m_picking_constraint = nullptr;
		m_picked_obj = nullptr;
	}

	void PhysicsManager::setDebugDrawer(DebugDrawer* dd) {
		m_world->setDebugDrawer(dd);
	}

	void PhysicsManager::debugDraw() {
		m_world->debugDrawWorld();
		static_cast<DebugDrawer*>(m_world->getDebugDrawer())->update();
	}

	unsigned PhysicsManager::addCube(long id, const glm::vec3& sides,
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

		// TODO: make dynamic later
		body->setCollisionFlags(body->getCollisionFlags() |
			btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

		unsigned world_id = m_world->getNumCollisionObjects();

		m_user_objects.push_back(
			new PhysicsObject{
				PHYSICS_OBJECT_CUBE,
				id,
				m_user_objects.size(),
				world_id
			}
		);

		body->setUserPointer(m_user_objects[m_user_objects.size() - 1]);

		m_world->addRigidBody(body);

		return world_id;
	}

	unsigned PhysicsManager::addSphere(long id, float radius,
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

		// TODO: make dynamic later
		body->setCollisionFlags(body->getCollisionFlags() |
			btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

		unsigned world_id = m_world->getNumCollisionObjects();

		m_user_objects.push_back(
			new PhysicsObject{
				PHYSICS_OBJECT_SPHERE,
				id,
				m_user_objects.size(),
				world_id
			}
		);

		body->setUserPointer(m_user_objects[m_user_objects.size() - 1]);

		m_world->addRigidBody(body);

		return world_id;
	}

	unsigned PhysicsManager::addStaticBody(
		const std::vector<Graphics::BasicVertex>& vertices,
		const std::vector<unsigned>& indices,
		const glm::mat4& transform) {
		btTransform t;
		t.setIdentity();
		t.setOrigin(btVector3(0.f, 0.f, 0.f));

		btTriangleMesh* mesh = new btTriangleMesh();

		for (unsigned i = 0; i < indices.size(); i += 3) {
			glm::vec3 v1 = glm::vec3(transform *
				glm::vec4(vertices[indices[i]].position, 1.0f));
			glm::vec3 v2 = glm::vec3(transform *
				glm::vec4(vertices[indices[i + 1]].position, 1.0f));
			glm::vec3 v3 = glm::vec3(transform *
				glm::vec4(vertices[indices[i + 2]].position, 1.0f));

			mesh->addTriangle(
				btVector3(v1.x, v1.y, v1.z),
				btVector3(v2.x, v2.y, v2.z),
				btVector3(v3.x, v3.y, v3.z)
			);
		}

		btCollisionShape* shape =
			new btBvhTriangleMeshShape(mesh, true);

		btMotionState* motion = new btDefaultMotionState(t);
		btRigidBody::btRigidBodyConstructionInfo info(0.f, motion, shape);
		btRigidBody* body = new btRigidBody(info);

		this->m_world->addRigidBody(body);

		return m_world->getNumCollisionObjects() - 1;
	}
}