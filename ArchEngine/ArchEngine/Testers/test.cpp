/*===========================================================================*
 * Arch Engine - "Testers/test.cpp"                                          *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 31/05/2018                                                 *
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

	unsigned colorshader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/colorvs.glsl",
		"../../ArchEngine/Shaders/colorfs.glsl"
	);

	unsigned objshader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/shadowvs.glsl",
		"../../ArchEngine/Shaders/shadowfs.glsl"
	);

	unsigned normalshader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/nmshadowvs.glsl",
		"../../ArchEngine/Shaders/nmshadowfs.glsl"
	);

	unsigned dir_depth_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/depthmapvs.glsl",
		"../../ArchEngine/Shaders/depthmapfs.glsl"
	);

	unsigned point_depth_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/cubedepthmapvs.glsl",
		"../../ArchEngine/Shaders/cubedepthmapgs.glsl",
		"../../ArchEngine/Shaders/cubedepthmapfs.glsl"
	);
	
	loader.importScene(
		"../../../../GameEngineLearning/assets/plane/plane_brick.obj",
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

	for (int i = -2; i <= 2; i++) {
		for (int k = -2; k <= 2; k++) {
			g_entities.push_back(
				{
					normalshader,
					loaded_meshes_ids,
					glm::scale(glm::translate(glm::mat4(1.0f),
						glm::vec3(i * 20.0f, 0.0f, k * 20.0f)),
						glm::vec3(10.0f, 1.0f, 10.0f))
				}
			);
		}
	}
	/*
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
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f))
		}
	);
	*/

	loaded_meshes_ids.clear();
	loader.importScene(
		"../../../../GameEngineLearning/assets/bob/bob_lamp_update_export.md5mesh",
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

	g_entities.push_back(
		{
			objshader,
			loaded_meshes_ids,
			glm::scale(glm::rotate(glm::translate(
				glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f)),
				glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
				glm::vec3(2.0f, 2.0f, 2.0f))
		}
	);

	loader.importScene(
		"../../../../GameEngineLearning/assets/sphere/sphere_brick.obj",
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
	
	glm::vec3 plight_pos(0.0f, 10.0f, 0.0f);
	glm::mat4 plight_proj =
		glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);

	PointLight plight = {
		plight_pos,
		64.0f,
		1.0f,
		0.007,
		0.0002f,
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

	//GraphicsManager::getInstance().addDirectionalLight(dlight);
	GraphicsManager::getInstance().addPointLight(plight);
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