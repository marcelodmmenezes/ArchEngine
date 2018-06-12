/*===========================================================================*
 * Arch Engine - "Testers/test.cpp"                                          *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 07/06/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

#if defined(ARCH_ENGINE_TEST)

#include "../Core/engine.hpp"
#include "../Graphics/assimpLoader.hpp"
#include "../Graphics/graphicsManager.hpp"
#include "../Graphics/terrainGenerator.hpp"
#include "../OS/inputManager.hpp"
#include "../Physics/physicsManager.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <sstream>
#include <string>


using namespace Core;
using namespace Graphics;
using namespace OS;
using namespace Physics;
using namespace Utils;


enum GameInputActions {
	TEST_ACTION,
	QUIT_ACTION = 2
};

enum GameInputStates {
	TEST_STATE,
	MOUSE_CLICK_STATE = 6
};


void loadData();
void onContextEvent(EventPtr e);
void onInputActionEvent(EventPtr e);
void onInputStateEvent(EventPtr e);
void onInputRangeEvent(EventPtr e);
void onInputMouseMoved(EventPtr e);
void onCollisionEvent(EventPtr e);
void onClosestRayTestEvent(EventPtr e);
void onAllRayTestEvent(EventPtr e);
void loopCallback();


DebugDrawer* dd;


int main(int argc, char* argv[]) {
	//srand(time(nullptr));

	Engine::startLoggingServices();

	if (Engine::getInstance().initialize("../../ArchEngine/Testers/"
		"core_engine_test_engine_config.lua")) {
		EventListener listener;

		loadData();

		listener.bind<&onContextEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_CONTEXT);

		listener.bind<&onInputActionEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_ACTION);

		listener.bind<&onInputStateEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_STATE);

		listener.bind<&onInputRangeEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_RANGE);

		listener.bind<&onInputMouseMoved>();
		EventManager::getInstance().addListener(
			listener, EVENT_MOUSE_MOVED);

		listener.bind<&onCollisionEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_COLLISION);

		listener.bind<&onClosestRayTestEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_RAY_TEST_CLOSEST);

		listener.bind<&onAllRayTestEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_RAY_TEST_ALL);

		InputManager::getInstance().pushContext("test1");

		Engine::getInstance().loopDelegate.bind<loopCallback>();

		Engine::getInstance().run();
	}
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	else {
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize ArchEngine");
	}
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

	Engine::getInstance().exit();

	delete dd;
}

void loadData() {
	GraphicsManager::getInstance().setProjectionMatrix(
		glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f));

	DebugCamera camera(
		glm::vec3(0.0f, 15.0f, 15.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	);

	GraphicsManager::getInstance().setActiveCamera(
		GraphicsManager::getInstance().addCamera(camera));

	unsigned line_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/linevs.glsl",
		"../../ArchEngine/Shaders/linefs.glsl"
	);

	dd = new DebugDrawer(line_shader);
	dd->setDebugMode(btIDebugDraw::DBG_DrawAabb);
	PhysicsManager::getInstance().setDebugDrawer(dd);

	unsigned obj_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/shadowvs.glsl",
		"../../ArchEngine/Shaders/shadowfs.glsl"
	);

	unsigned normal_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/nmshadowvs.glsl",
		"../../ArchEngine/Shaders/nmshadowfs.glsl"
	);

	unsigned dir_depth_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/depthmapvs.glsl",
		"../../ArchEngine/Shaders/depthmapgs.glsl"
	);

	unsigned point_depth_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/cubedepthmapvs.glsl",
		"../../ArchEngine/Shaders/cubedepthmapgs.glsl",
		"../../ArchEngine/Shaders/cubedepthmapfs.glsl"
	);

	unsigned normal_debug_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/normaldebugvs.glsl",
		"../../ArchEngine/Shaders/normaldebuggs.glsl",
		"../../ArchEngine/Shaders/normaldebugfs.glsl"
	);

	glm::vec3 plight_pos(0.0f, 50.0f, 0.0f);
	glm::mat4 plight_proj =
		glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
	
	PointLight plight = {
		plight_pos,
		64.0f,
		1.0f,
		0.0007,
		0.00014,
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		true,
		plight_proj,
		glm::lookAt(plight_pos, plight_pos +
		glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(plight_pos, plight_pos +
			glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(plight_pos, plight_pos +
			glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(plight_pos, plight_pos +
			glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		glm::lookAt(plight_pos, plight_pos +
			glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(plight_pos, plight_pos +
			glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		point_depth_shader,
		1024,
		1024,
		1000.0f
	};

	GraphicsManager::getInstance().addPointLight(plight);
	
	/*
	DirectionalLight dlight = {
		glm::vec3(-0.6f, -1.0f, -0.5f),
		32.0f,
		glm::vec3(0.1f, 0.1f, 0.05f),
		glm::vec3(0.8f, 0.8f, 0.4f),
		glm::vec3(0.8f, 0.8f, 0.4f),
		true,
		glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, 0.1f, 1000.0f),
		glm::lookAt(glm::vec3(60.0, 100.0, 50.0f),
		glm::vec3(-0.6f, -1.0f, -0.5f),
		glm::vec3(0.0f, 1.0f, 0.0f)),
		dir_depth_shader,
		4000,
		4000
	};

	GraphicsManager::getInstance().addDirectionalLight(dlight);
	*/

	TerrainGenerator tg;
	auto terrain = tg.genHeightMapTerrain(256, 256, 2.0f, 2.0f, 50.0f,
		"../../../../GameEngineLearning/assets/miscTextures/heightMaps/height_map3.png");
	
	std::vector<unsigned> loaded_meshes_ids;
	AssimpLoader loader;
	
	loader.importScene(
		"../../../../GameEngineLearning/assets/plane/plane_terrain.obj",
		aiPostProcessSteps(
			aiProcess_GenSmoothNormals |
			//aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
		),
		loaded_meshes_ids
	);

	loaded_meshes_ids.clear();
	loaded_meshes_ids.push_back(terrain.first);
	
	g_entities.push_back(
		{
			obj_shader,
			loaded_meshes_ids,
			std::vector<unsigned>(),
			std::vector<glm::mat4>(1, glm::mat4(1.0f))
		}
	);
	
	int physics_id = 0;
	std::vector<unsigned> bodies;
	/*
	loader.importScene(
		"../../../../GameEngineLearning/assets/sponza/mergedSponza.obj",
		aiPostProcessSteps(
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
		),
		loaded_meshes_ids
	);

	g_entities.push_back(
		{
			normalshader,
			loaded_meshes_ids,
			bodies,
			std::vector<glm::mat4>(loaded_meshes_ids.size(), glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)))
		}
	);
	*/
	loaded_meshes_ids.clear();
	loader.importScene(
		"../../../../GameEngineLearning/assets/cube/cube_wooden.obj",
		aiPostProcessSteps(
			aiProcess_GenSmoothNormals |
			//aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
		),
		loaded_meshes_ids
	);
	
	bodies.clear();
	bodies.resize(loaded_meshes_ids.size());

	for (unsigned i = 1; i < 5; i++) {
		g_entities.push_back(
			{
				obj_shader,
				loaded_meshes_ids,
				bodies,
				std::vector<glm::mat4>(loaded_meshes_ids.size(), glm::mat4(1.0f))
			}
		);

		for (auto& it : g_entities[g_entities.size() - 1].physics_objects)
			it = PhysicsManager::getInstance().addCube(
				physics_id++,
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3((rand() % 100 - 50) * 1.0f, i * 100.0f, (rand() % 100 - 50) * 1.0f),
				1.0f);
	}

	loaded_meshes_ids.clear();
	loader.importScene(
		"../../../../GameEngineLearning/assets/sphere/sphere_brick.obj",
		aiPostProcessSteps(
			aiProcess_GenSmoothNormals |
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
		),
		loaded_meshes_ids
	);

	bodies.clear();
	bodies.resize(loaded_meshes_ids.size());

	for (unsigned i = 1; i < 10; i++) {
		g_entities.push_back(
			{
				normal_shader,
				loaded_meshes_ids,
				bodies,
				std::vector<glm::mat4>(loaded_meshes_ids.size(), glm::mat4(1.0f))
			}
		);

		for (auto& it : g_entities[g_entities.size() - 1].physics_objects)
			it = PhysicsManager::getInstance().addSphere(
				physics_id++,
				1.0f,
				glm::vec3((rand() % 100 - 50) * 1.0f, i * 100.0f, (rand() % 100 - 50) * 1.0f),
				1.0f);
	}
	
	Engine::getInstance().releaseMouse();
}

void onContextEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputContextEvent>(e);

	if (evnt->getState()) {
		// Context pushed
		std::cout << "\nCONTEXT PUSHED: " << evnt->getName() <<
			"\n" << std::endl;
	}
	else {
		// Context popped
		std::cout << "\nCONTEXT POPPED: " << evnt->getName() <<
			"\n" << std::endl;
	}
}

void onInputActionEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputActionEvent>(e);

	switch (evnt->getValue()) {
	case GameInputActions::TEST_ACTION:
		std::cout << "ACTION" << std::endl;
		break;
	case GameInputActions::QUIT_ACTION:
		EventPtr evnt = std::make_shared<CoreQuitEvent>(CoreQuitEvent());
		EventManager::getInstance().sendEvent(evnt);
		break;
	}
}

bool mouse_clicked = false;

void onInputStateEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputStateEvent>(e);

	auto camera = GraphicsManager::getInstance().getActiveCamera();
	if (!camera) return;

	switch (evnt->getValue()) {
	case 0:
		camera->move(CAMERA_FORWARD);
		break;
	case 1:
		camera->move(CAMERA_BACKWARD);
		break;
	case 2:
		camera->move(CAMERA_LEFT);
		break;
	case 3:
		camera->move(CAMERA_RIGHT);
		break;
	case 4:
		camera->move(CAMERA_UP);
		break;
	case 5:
		camera->move(CAMERA_DOWN);
		break;
	case 6:
		if (evnt->isOver())
			camera->setSpeed(10.0f);
		else
			camera->setSpeed(25.0f);
		break;
	case 7:
		if (evnt->isOver())
			mouse_clicked = false;
		else
			mouse_clicked = true;
		break;
	}
}

void onInputRangeEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputRangeEvent>(e);

	auto camera = GraphicsManager::getInstance().getActiveCamera();
	if (!camera) return;

	switch (evnt->getValue().m_range) {
	case 0:
		if (mouse_clicked) camera->look((float)evnt->getValue().m_value, 0.0f);
		break;
	case 1:
		if (mouse_clicked) camera->look(0.0f, (float)evnt->getValue().m_value);
		break;
	}
}

void onInputMouseMoved(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputMouseMoved>(e);

	auto camera = GraphicsManager::getInstance().getActiveCamera();
	if (!camera) return;

	if (!mouse_clicked) {
		int x, y;
		evnt->getValues(x, y);

		float n_x = (2.0f * x) / 800 - 1.0f;
		float n_y = 1.0f - (2.0f * y) / 600;
		glm::vec3 ray_nds(n_x, n_y, -1.0f);
		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
		glm::vec4 ray_eye = glm::inverse(GraphicsManager::getInstance().getProjectionMatrix()) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
		glm::vec3 ray_world = glm::vec3(glm::inverse(camera->getViewMatrix()) * ray_eye);
		ray_world = glm::normalize(ray_world);

		PhysicsManager::getInstance().closestObjectRayTest(ray_world, glm::vec3(camera->getFront()) * 1000.0f);
	}
}

void onCollisionEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<CollisionEvent>(e);

	long id1, id2;
	evnt->getObjectIds(id1, id2);

	std::cout << "Collision: " << id1 << " " << id2 << std::endl;
}

void onClosestRayTestEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<ClosestRayTestEvent>(e);
	std::cout << "ClosestRayTest: " << evnt->getObjectId() << std::endl;
}

void onAllRayTestEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<AllRayTestEvent>(e);

	std::cout << "AllRayTest: ";

	for (auto& it : evnt->m_obj_ids)
		std::cout << it << " ";

	std::cout << std::endl;
}

void loopCallback() {
	PhysicsManager::getInstance().debugDraw();
}


#endif	// ARCH_ENGINE_TEST