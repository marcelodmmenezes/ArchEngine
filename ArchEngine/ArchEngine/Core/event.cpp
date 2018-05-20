/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 20/05/2018                                                 *
 *===========================================================================*/


#include "event.hpp"


namespace Core {
	IEvent::IEvent(EventType type) : m_type(type) {}
	// Virtual pure destructor needs implementation
	IEvent::~IEvent() {}

	//------------------------------------------------------------- Core events
	CoreQuitEvent::CoreQuitEvent() : IEvent(EVENT_CORE_QUIT) {}
	CoreQuitEvent::~CoreQuitEvent() {}
	EventType CoreQuitEvent::getType() const { return m_type; }

	CoreTimerEvent::CoreTimerEvent(unsigned time) :
		IEvent(EVENT_CORE_TIMER), m_time(time) {}
	CoreTimerEvent::~CoreTimerEvent() {}
	EventType CoreTimerEvent::getType() const { return m_type; }
	unsigned CoreTimerEvent::getTime() const { return m_time; }

	//----------------------------------------------------------- Window events
	WindowResizeEvent::WindowResizeEvent(int width, int height) :
		IEvent(EVENT_WINDOW_RESIZE), m_width(width), m_height(height) {}
	WindowResizeEvent::~WindowResizeEvent() {}
	EventType WindowResizeEvent::getType() const { return m_type; }
	void WindowResizeEvent::getSize(int& width, int& height) {
		width = m_width;
		height = m_height;
	}
}