/*===========================================================================*
 * Arch Engine - "Core/event.cpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 02/05/2018                                                       *
 * Last Modified: 05/05/2018                                                 *
 *===========================================================================*/


#include "event.hpp"


namespace Core {
	//------------------------------------------------------------- Core events
	CoreQuitEvent::CoreQuitEvent() : IEvent(CORE_QUIT_EVENT) {}
	CoreQuitEvent::~CoreQuitEvent() {}
	EventType CoreQuitEvent::getType() const { return m_type; }

	//------------------------------------------------------------ Input events
	InputActionEvent::InputActionEvent() : IEvent(INPUT_ACTION_EVENT) {}
	InputActionEvent::InputActionEvent(OS::InputAction value) :
		IEvent(INPUT_ACTION_EVENT), m_value(value) {}
	InputActionEvent::~InputActionEvent() {}
	EventType InputActionEvent::getType() const { return m_type; }
	OS::InputAction InputActionEvent::getValue() const { return m_value; }
	void InputActionEvent::setValue(OS::InputAction value) { m_value = value; }

	InputStateEvent::InputStateEvent() : IEvent(INPUT_STATE_EVENT) {}
	InputStateEvent::InputStateEvent(OS::InputState value) :
		IEvent(INPUT_STATE_EVENT), m_value(value) {}
	InputStateEvent::~InputStateEvent() {}
	EventType InputStateEvent::getType() const { return m_type; }
	OS::InputState InputStateEvent::getValue() const { return m_value; }
	void InputStateEvent::setValue(OS::InputState value) { m_value = value; }

	InputRangeEvent::InputRangeEvent() : IEvent(INPUT_RANGE_EVENT) {}
	InputRangeEvent::InputRangeEvent(OS::RangeInfo& value) :
		IEvent(INPUT_RANGE_EVENT), m_value(std::move(value)) {}
	InputRangeEvent::~InputRangeEvent() {}
	EventType InputRangeEvent::getType() const { return m_type; }
	OS::RangeInfo InputRangeEvent::getValue() const { return m_value; }
	void InputRangeEvent::setValue(OS::RangeInfo& value) { 
		m_value = std::move(value);
	}
}