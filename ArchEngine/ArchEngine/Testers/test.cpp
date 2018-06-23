/*===========================================================================*
 * Arch Engine - "Testers/test.cpp"                                          *
 *                                                                           *
 * This is a tester file. It's purpose is to test some functionalities, thus *
 * should be removed from releases.                                          *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 30/04/2018                                                       *
 * Last Modified: 23/06/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"

#if defined (ARCH_ENGINE_TEST)

#include "../Core/engine.hpp"
#include "../Graphics/assimpLoader.hpp"
#include "../Graphics/graphicsManager.hpp"
#include "../Graphics/terrainGenerator.hpp"
#include "../Graphics/thirdPersonCamera.hpp"
#include "../GUI/guiManager.hpp"
#include "../GUI/writableComponent.hpp"
#include "../GUI/renderableComponent.hpp"
#include "../OS/inputManager.hpp"
#include "../Physics/physicsManager.hpp"
#include "../Utils/serviceLocator.hpp"

#include <cstdlib>
#include <sstream>
#include <string>


using namespace Core;
using namespace Graphics;
using namespace GUI;
using namespace OS;
using namespace Physics;
using namespace Utils;


enum GameInputActions {
	CHANGE_PLAYER = 0,
	CHANGE_CAMERA_ACTION = 1,
	QUIT_ACTION = 27
};

enum GameInputStates {
	TEST_STATE,
	MOUSE_CLICK_STATE = 6
};


void loadData();
void onInputActionEvent(EventPtr e);
void onInputStateEvent(EventPtr e);
void onInputMouseMoved(EventPtr e);
void onWindowResizeEvent(EventPtr e);
void onLoopFinishedEvent(EventPtr e);


ThirdPersonCamera tpcamera(15.0f);

DebugCamera debug_camera(
	glm::vec3(0.0f, 15.0f, 15.0f),
	glm::vec3(0.0f, 0.0f, -1.0f)
);

DebugDrawer* dd;
WritableComponent* wc;
RenderableComponent* rc;
RenderableComponent* rc2;


int main(int argc, char* argv[]) {
	srand(time(nullptr));

	Engine::startLoggingServices();

	if (Engine::getInstance().initialize("../../ArchEngine/Testers/"
		"core_engine_test_engine_config.lua")) {
		EventListener listener;

		loadData();

		listener.bind<&onInputActionEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_ACTION);

		listener.bind<&onInputStateEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_STATE);

		listener.bind<&onInputMouseMoved>();
		EventManager::getInstance().addListener(
			listener, EVENT_MOUSE_MOVED);

		listener.bind<&onWindowResizeEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_WINDOW_RESIZE);

		listener.bind<&onLoopFinishedEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_LOOP_FINISHED);

		InputManager::getInstance().pushContext("writing_context");

		Engine::getInstance().run();
	}
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	else {
		ServiceLocator::getFileLogger()->log<LOG_ERROR>(
			"Failed to initialize ArchEngine");
	}
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

	Engine::getInstance().exit();

	delete rc2;
	delete rc;
	delete wc;
}

void loadData() {
	unsigned line_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/linevs.glsl",
		"../../ArchEngine/Shaders/linefs.glsl"
	);			

	unsigned quad_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/quadvs.glsl",
		"../../ArchEngine/Shaders/quadfs.glsl"
	);

	wc = new WritableComponent(nullptr, glm::vec2(800, 600),
		glm::vec2(25.0f, 575.0f), 1, 1.0f, 15.0f,
		glm::vec3(0.8f, 0.8f, 0.0f), glm::vec2(155.0f, 32.0f));
	wc->setHoverColor(glm::vec3(0.94f, 0.94f, 0.94f));
	wc->trackMouse();

	rc = new RenderableComponent(quad_shader, glm::vec2(800, 600),
		glm::vec4(0.0f, 0.0f, 205.0f, 600.0f),
		"../../../../GameEngineLearning/assets/miscTextures/wood_specular2.png");
	rc->setColor(glm::vec4(0.22f, 0.22f, 0.24f, 1.0f));
	rc->setBorderColor(glm::vec4(0.3f, 0.6f, 0.3f, 1.0f));
	rc->setBorderWidth(2);
	rc->trackMouse();

	rc2 = new RenderableComponent(quad_shader, glm::vec2(800, 600),
		glm::vec4(20.0f, 538.0f, 165.0f, 42.0f), "");
	rc2->setColor(glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
	rc2->setBorderColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
	rc2->setBorderWidth(2);
	rc2->trackMouse();

	Engine::getInstance().releaseMouse();
}

void onInputActionEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputActionEvent>(e);
	wc->write(evnt->getValue());

	if (evnt->getValue() == QUIT_ACTION) {
		EventPtr evnt = std::make_shared<CoreQuitEvent>(CoreQuitEvent());
		EventManager::getInstance().sendEvent(evnt);
	}
}

bool mouse_left_clicked = false;
bool mouse_right_clicked = false;

void onInputStateEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputStateEvent>(e);

	wc->write(evnt->getValue());

	switch (evnt->getValue()) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		break;
	case 7:
		if (evnt->isOver())
			mouse_left_clicked = false;
		else
			mouse_left_clicked = true;
		break;
	case 8:
		if (evnt->isOver())
			mouse_right_clicked = false;
		else
			mouse_right_clicked = true;
		break;
	}
}

void onInputMouseMoved(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputMouseMoved>(e);
}

int g_screen_width = 800;
int g_screen_height = 600;

void onWindowResizeEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<WindowResizeEvent>(e);
	evnt->getSize(g_screen_width, g_screen_height);

	wc->setPosition(glm::vec2(25.0f, g_screen_height - 25.0f));
	wc->setMaximumSize(wc->getMaximumSize());

	rc->setLimits(glm::vec4(0.0f, 0.0f, 205.0f, g_screen_height));
	rc2->setLimits(glm::vec4(20.0f, 20.0f, 165.0f, g_screen_height - 40.0f));
}

void onLoopFinishedEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<LoopFinishedEvent>(e);
	
	rc->render(glm::ortho(0.0f, (float)g_screen_width, 0.0f, (float)g_screen_height));
	glClear(GL_DEPTH_BUFFER_BIT);
	rc2->render(glm::ortho(0.0f, (float)g_screen_width, 0.0f, (float)g_screen_height));

	glDisable(GL_DEPTH_TEST);

	std::stringstream ss;
	ss << "SBF: " <<
		std::setprecision(6) << evnt->getDeltaTime() << "\nFPS: " <<
		std::setprecision(3) << " " << evnt->getFrameRate();
	wc->write(ss.str());

	wc->update();

	glEnable(GL_DEPTH_TEST);
}


#endif	// ARCH_ENGINE_TEST