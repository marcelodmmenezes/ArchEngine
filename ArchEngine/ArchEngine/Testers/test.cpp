/*===========================================================================*
 * Arch Engine - "Testers/test.cpp"                                          *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 17/05/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

#if defined(ARCH_ENGINE_TEST)

#include "../Core/engine.hpp"
#include "../Graphics/shader.hpp"
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
	std::vector<Vertex> vertices;
	Vertex vertex;

	vertex.position = glm::vec3(-1.0f, -1.0f, 1.0f);
	vertices.push_back(vertex);
	vertex.position = glm::vec3(1.0f, -1.0f, 1.0f);
	vertices.push_back(vertex);
	vertex.position = glm::vec3(1.0f, 1.0f, 1.0f);
	vertices.push_back(vertex);
	vertex.position = glm::vec3(-1.0f, 1.0f, 1.0f);
	vertices.push_back(vertex);
	vertex.position = glm::vec3(-1.0f, -1.0f, -1.0f);
	vertices.push_back(vertex);
	vertex.position = glm::vec3(1.0f, -1.0f, -1.0f);
	vertices.push_back(vertex);
	vertex.position = glm::vec3(1.0f, 1.0f, -1.0f);
	vertices.push_back(vertex);
	vertex.position = glm::vec3(-1.0f, 1.0f, -1.0f);
	vertices.push_back(vertex);

	std::vector<unsigned> indices = {
		0, 1, 2,
		2, 3, 0,
		3, 2, 6,
		6, 7, 3,
		7, 6, 5,
		5, 4, 7,
		4, 5, 1,
		1, 0, 4,
		4, 0, 3,
		3, 7, 4,
		1, 5, 6,
		6, 2, 1,
	};

	GraphicsManager::getInstance().m_mesh.create(
		VERTEX_POSITION, vertices, indices, 0);

	GraphicsManager::getInstance().m_shader.initialize(
		"../../ArchEngine/Testers/simplevs.glsl",
		"../../ArchEngine/Testers/simplefs.glsl");
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

	auto& camera = GraphicsManager::getInstance().m_camera;

	switch (evnt->getValue()) {
	case 0:
		camera.move(CAMERA_FORWARD);
		break;
	case 1:
		camera.move(CAMERA_BACKWARD);
		break;
	case 2:
		camera.move(CAMERA_LEFT);
		break;
	case 3:
		camera.move(CAMERA_RIGHT);
		break;
	case 4:
		camera.move(CAMERA_UP);
		break;
	case 5:
		camera.move(CAMERA_DOWN);
		break;
	case 6:
		if (evnt->isOver())
			camera.setSpeed(10.0f);
		else
			camera.setSpeed(25.0f);
		break;
	}
}

void onInputRangeEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputRangeEvent>(e);

	auto& camera = GraphicsManager::getInstance().m_camera;

	switch (evnt->getValue().m_range) {
	case 0:
		camera.look(evnt->getValue().m_value, 0.0f);
		break;
	case 1:
		camera.look(0.0f, evnt->getValue().m_value);
		break;
	}
}


#endif	// ARCH_ENGINE_TEST