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
	TEST_STATE
};


void loadData();
void onContextEvent(EventPtr e);
void onInputActionEvent(EventPtr e);
void onInputStateEvent(EventPtr e);
void onInputRangeEvent(EventPtr e);
void onCollisionEvent(EventPtr e);
void onClosestRayTestEvent(EventPtr e);
void onAllRayTestEvent(EventPtr e);


int main(int argc, char* argv[]) {
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

		Engine::getInstance().run();
	}
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	else {
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize ArchEngine");
	}
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

	Engine::getInstance().exit();
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

	unsigned objshader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/shadowvs.glsl",
		"../../ArchEngine/Shaders/shadowfs.glsl"
	);

	unsigned normalshader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/nmshadowvs.glsl",
		"../../ArchEngine/Shaders/nmshadowfs.glsl"
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

	glm::vec3 plight_pos(0.0f, 10.0f, 0.0f);
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

	TerrainGenerator tg;
	unsigned terrain = tg.genHeightMapTerrain(5, 15,
		"../../../../GameEngineLearning/assets/miscTextures/heightMaps/height_map.png");

	std::vector<unsigned> loaded_meshes_ids;
	AssimpLoader loader;
	loader.importScene(
		"../../../../GameEngineLearning/assets/plane/plane_brick.obj",
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
	loaded_meshes_ids.push_back(terrain);

	g_entities.push_back(
		{
			objshader,
			loaded_meshes_ids,
			std::vector<unsigned>(),
			std::vector<glm::mat4>(1, glm::mat4(1.0f))
		}
	);
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
	}
}

void onInputRangeEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputRangeEvent>(e);

	auto camera = GraphicsManager::getInstance().getActiveCamera();
	if (!camera) return;

	switch (evnt->getValue().m_range) {
	case 0:
		camera->look((float)evnt->getValue().m_value, 0.0f);
		break;
	case 1:
		camera->look(0.0f, (float)evnt->getValue().m_value);
		break;
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


#endif	// ARCH_ENGINE_TEST