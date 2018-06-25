/*===========================================================================*
* Arch Engine - "Testers/test.cpp"                                          *
*                                                                           *
* This is a tester file. It's purpose is to test some functionalities, thus *
* should be removed from releases.                                          *
* Expect some hard coded, bizarre stuff. You've been warned ;)              *
*                                                                           *
* Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
* Created: 25/06/2018                                                       *
* Last Modified: 25/06/2018                                                 *
*===========================================================================*/


#include "../Config/engineMacros.hpp"


#include "../Core/engine.hpp"
#include "../Graphics/assimpLoader.hpp"
#include "../Graphics/graphicsManager.hpp"
#include "../Graphics/terrainGenerator.hpp"
#include "../Graphics/thirdPersonCamera.hpp"
#include "../GUI/guiManager.hpp"
#include "../GUI/renderableComponent.hpp"
#include "../GUI/pushButton.hpp"
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
	QUIT_ACTION = 2
};

enum GameInputStates {
	TEST_STATE,
	MOUSE_CLICK_STATE = 6
};


void loadData();
void onInputActionEvent(EventPtr e);
void onInputStateEvent(EventPtr e);
void onInputRangeEvent(EventPtr e);
void onButtonStateEvent(EventPtr e);
void onLoopFinishedEvent(EventPtr e);


DebugCamera debug_camera(
	glm::vec3(0.0f, 15.0f, 15.0f),
	glm::vec3(0.0f, 0.0f, -1.0f)
);


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

		listener.bind<&onInputRangeEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_INPUT_RANGE);

		listener.bind<&onButtonStateEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_BUTTON_STATE);

		listener.bind<&onLoopFinishedEvent>();
		EventManager::getInstance().addListener(
			listener, EVENT_LOOP_FINISHED);

		InputManager::getInstance().pushContext("writing_context");
		InputManager::getInstance().pushContext("test1");
		InputManager::getInstance().prioritize("test1", true);

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

	unsigned dbcam = GraphicsManager::getInstance().addCamera(&debug_camera);

	unsigned line_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/linevs.glsl",
		"../../ArchEngine/Shaders/linefs.glsl"
	);

	unsigned dd_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/debugdrawvs.glsl",
		"../../ArchEngine/Shaders/debugdrawfs.glsl"
	);

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
		"../../ArchEngine/Shaders/depthmapfs.glsl"
	);

	unsigned point_depth_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/cubedepthmapvs.glsl",
		"../../ArchEngine/Shaders/cubedepthmapgs.glsl",
		"../../ArchEngine/Shaders/cubedepthmapfs.glsl"
	);

	unsigned color_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/colorvs.glsl",
		"../../ArchEngine/Shaders/colorfs.glsl"
	);

	unsigned quad_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/quadvs.glsl",
		"../../ArchEngine/Shaders/quadfs.glsl"
	);

	unsigned button_shader = GraphicsManager::getInstance().addShader(
		"../../ArchEngine/Shaders/buttonvs.glsl",
		"../../ArchEngine/Shaders/buttonfs.glsl"
	);

	Engine::getInstance().releaseMouse();
}

int act_cm = 0;
int player = 1;
int context_priority = 0;

void onInputActionEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputActionEvent>(e);

	switch (evnt->getValue()) {
	case GameInputActions::CHANGE_PLAYER:
		if (context_priority == 0) {
			context_priority = 1;
			InputManager::getInstance().prioritize("writing_context", true);
			Engine::getInstance().releaseMouse();
		}
		else {
			context_priority = 0;
			InputManager::getInstance().prioritize("test1", true);
			Engine::getInstance().captureMouse();
		}

		if (player == 1)
			player = 5;
		else
			player = 1;

		break;
	case GameInputActions::CHANGE_CAMERA_ACTION:
		GraphicsManager::getInstance().setActiveCamera(act_cm);

		if (act_cm == 0)
			act_cm = 1;
		else
			act_cm = 0;
		break;
	case GameInputActions::QUIT_ACTION:
		EventPtr evnt = std::make_shared<CoreQuitEvent>(CoreQuitEvent());
		EventManager::getInstance().sendEvent(evnt);
		break;
	}

	if (context_priority == 1) {
		if (evnt->getValue() == '1')
			dynamic_cast<PushButton*>(
				GUIManager::getInstance().getControl("xablau"))
			->justifyLeft();
		else if (evnt->getValue() == '2')
			dynamic_cast<PushButton*>(
				GUIManager::getInstance().getControl("xablau"))
			->centralize();
		else if (evnt->getValue() == '3')
			dynamic_cast<PushButton*>(
				GUIManager::getInstance().getControl("xablau"))
			->justifyRight();
	}
}

bool mouse_left_released = false;
bool mouse_left_clicked = false;
bool mouse_right_clicked = false;

void onInputStateEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputStateEvent>(e);

	auto camera = GraphicsManager::getInstance().getActiveCamera();

	mouse_left_released = false;

	switch (evnt->getValue()) {
	case 0:
		if (auto cm = dynamic_cast<DebugCamera*>(camera))
			cm->move(CAMERA_FORWARD);
		break;
	case 1:
		if (auto cm = dynamic_cast<DebugCamera*>(camera))
			cm->move(CAMERA_BACKWARD);
		break;
	case 2:
		if (auto cm = dynamic_cast<DebugCamera*>(camera))
			cm->move(CAMERA_LEFT);
		break;
	case 3:
		if (auto cm = dynamic_cast<DebugCamera*>(camera))
			cm->move(CAMERA_RIGHT);
		break;
	case 4:
		if (auto cm = dynamic_cast<DebugCamera*>(camera))
			cm->move(CAMERA_UP);
		break;
	case 5:
		if (auto cm = dynamic_cast<DebugCamera*>(camera))
			cm->move(CAMERA_DOWN);
		break;
	case 6:
		if (auto cm = dynamic_cast<DebugCamera*>(camera)) {
			if (evnt->isOver())
				cm->setSpeed(10.0f);
			else
				cm->setSpeed(25.0f);
		}
		break;
	case 7:
		if (evnt->isOver()) {
			mouse_left_released = true;
			mouse_left_clicked = false;
		}
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

void onInputRangeEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<InputRangeEvent>(e);

	auto camera = dynamic_cast<DebugCamera*>(
		GraphicsManager::getInstance().getActiveCamera());

	if (camera) {
		Engine::getInstance().releaseMouse();

		switch (evnt->getValue().m_range) {
		case 0:
			if (mouse_right_clicked)
				camera->look((float)evnt->getValue().m_value, 0.0f);
			break;
		case 1:
			if (mouse_right_clicked)
				camera->look(0.0f, (float)evnt->getValue().m_value);
			break;
		}
	}
}

void onButtonStateEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<ButtonStateEvent>(e);

	switch (evnt->getButtonId()) {
	case 0:
		break;
	}
}

void onLoopFinishedEvent(EventPtr e) {
	auto evnt = std::static_pointer_cast<LoopFinishedEvent>(e);

	std::stringstream ss;
	ss << "SBF: " <<
		std::setprecision(6) << evnt->getDeltaTime() << "\nFPS: " <<
		std::setprecision(3) << " " << evnt->getFrameRate();

	dynamic_cast<WritableComponent*>
		(GUIManager::getInstance().getControl("fpscounter"))->write(ss.str());
}