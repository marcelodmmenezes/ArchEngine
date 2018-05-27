/*===========================================================================*
 * Arch Engine - "Testers/test.cpp"                                          *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 25/05/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

#if defined(ARCH_ENGINE_TEST)

#include "../Core/engine.hpp"
#include "../Graphics/assimpLoader.hpp"
#include "../Graphics/graphicsManager.hpp"
#include "../OS/inputManager.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <sstream>
#include <string>


using namespace Core;
using namespace Graphics;
using namespace OS;
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

		InputManager::getInstance().pushContext("test1");

		Engine::getInstance().run();
	}
	else {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize ArchEngine");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	}

	Engine::getInstance().exit();
}

void loadData() {
	AssimpLoader loader;
	std::vector<unsigned> loaded_meshes_ids;

	GraphicsManager::getInstance().setProjectionMatrix(
		glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f));

	DebugCamera camera(
		glm::vec3(0.0f, 15.0f, 15.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	);

	GraphicsManager::getInstance().setActiveCamera(
		GraphicsManager::getInstance().addCamera(camera));

	unsigned objshader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/objvs.glsl",
		"../../ArchEngine/Shaders/objfs.glsl"
	);

	loader.importScene(
		"../../../../GameEngineLearning/assets/sponza/mergedSponza.obj",
		aiPostProcessSteps(
			//aiProcess_GenSmoothNormals |
			//aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
		),
		loaded_meshes_ids
	);

	g_entities.push_back(
		{
			objshader,
			loaded_meshes_ids,
			glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f))
		}
	);

	loaded_meshes_ids.clear();
	loader.importScene(
		"../../../../GameEngineLearning/assets/nanosuit/nanosuit.obj",
		aiPostProcessSteps(
			//aiProcess_GenSmoothNormals |
			//aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs
		),
		loaded_meshes_ids
	);

	g_entities.push_back(
		{
			objshader,
			loaded_meshes_ids,
			glm::mat4(1.0f)
		}
	);

	DirectionalLight dlight = {
		glm::vec3(-1.0f, -1.0f, -1.0f),
		32.0f,
		glm::vec3(0.1f, 0.1f, 0.05f),
		glm::vec3(0.8f, 0.8f, 0.4f),
		glm::vec3(0.8f, 0.8f, 0.4f),
	};

	PointLight plight = {
		glm::vec3(-20.0f, 10.0f, 15.0f),
		64.0f,
		1.0f,
		0.007f,
		0.0002f,
		glm::vec3(0.02f, 0.1f, 0.08f),
		glm::vec3(0.2f, 1.0f, 0.8f),
		glm::vec3(0.2f, 1.0f, 0.8f)
	};

	SpotLight slight = {
		glm::vec3(40.0f, 55.0f, -35.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		64.0f,
		glm::cos(glm::radians(12.5f)),
		glm::cos(glm::radians(17.5f)),
		1.0f,
		0.007f,
		0.0002f,
		glm::vec3(0.0f, 0.0f, 0.1f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	};

	GraphicsManager::getInstance().addDirectionalLight(dlight);
	GraphicsManager::getInstance().addPointLight(plight);
	GraphicsManager::getInstance().addSpotLight(slight);
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


#endif	// ARCH_ENGINE_TEST