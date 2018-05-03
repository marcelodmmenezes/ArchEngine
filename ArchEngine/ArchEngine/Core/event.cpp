/*===========================================================================*
 * Arch Engine - "Core/event.cpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 02/05/2018                                                       *
 * Last Modified: 03/05/2018                                                 *
 *===========================================================================*/


#include "event.hpp"


namespace Core {
	//------------------------------------------------------------- Core events
	CoreQuitEvent::CoreQuitEvent() : IEvent(CORE_QUIT_EVENT) {}
	CoreQuitEvent::~CoreQuitEvent() {}
	EventType CoreQuitEvent::getType() const { return m_type; }

	//------------------------------------------------------------ Input events
	InputActionEvent::InputActionEvent() : IEvent(INPUT_ACTION_EVENT) {}
	InputActionEvent::~InputActionEvent() {}
	EventType InputActionEvent::getType() const { return m_type; }
	unsigned InputActionEvent::getValue() const { return m_value; }
	void InputActionEvent::setValue(unsigned value) { m_value = value; }

	InputStateEvent::InputStateEvent() : IEvent(INPUT_STATE_EVENT) {}
	InputStateEvent::~InputStateEvent() {}
	EventType InputStateEvent::getType() const { return m_type; }
	unsigned InputStateEvent::getValue() const { return m_value; }
	void InputStateEvent::setValue(unsigned value) { m_value = value; }
}