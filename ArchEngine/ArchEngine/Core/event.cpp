/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
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

	LoopFinishedEvent::LoopFinishedEvent(double delta_time, double frame_rate)
		: IEvent(EVENT_LOOP_FINISHED), m_delta_time(delta_time),
		m_frame_rate(frame_rate) {}
	LoopFinishedEvent::~LoopFinishedEvent() {}
	EventType LoopFinishedEvent::getType() const { return m_type; }
	double LoopFinishedEvent::getDeltaTime() const { return m_delta_time; }
	double LoopFinishedEvent::getFrameRate() const { return m_frame_rate; }

	//-------------------------------------------------------------- GUI events
	ButtonStateEvent::ButtonStateEvent(int button_id, bool state) :
		IEvent(EVENT_BUTTON_STATE), m_id(button_id), m_state(state) {}
	ButtonStateEvent::~ButtonStateEvent() {}
	EventType ButtonStateEvent::getType() const { return m_type; }
	int ButtonStateEvent::getButtonId() const { return m_id; }
	bool ButtonStateEvent::getState() const { return m_state; }

	//----------------------------------------------------------- Window events
	WindowResizeEvent::WindowResizeEvent(int width, int height) :
		IEvent(EVENT_WINDOW_RESIZE), m_width(width), m_height(height) {}
	WindowResizeEvent::~WindowResizeEvent() {}
	EventType WindowResizeEvent::getType() const { return m_type; }
	void WindowResizeEvent::getSize(int& width, int& height) {
		width = m_width;
		height = m_height;
	}

	//------------------------------------------------------------ Input events
	InputMouseMoved::InputMouseMoved(int x, int y, bool locked) :
		IEvent(EVENT_MOUSE_MOVED), m_x(x), m_y(y), m_locked(locked) {}
	InputMouseMoved::~InputMouseMoved() {}
	Core::EventType InputMouseMoved::getType() const { return m_type; }
	void InputMouseMoved::getValues(int& x, int&y) const { x = m_x; y = m_y; }
	bool InputMouseMoved::isLocked() const { return m_locked; }

	InputMouseButton::InputMouseButton(int x, int y,
		int button, bool press, bool locked) :
		IEvent(EVENT_MOUSE_BUTTON), m_x(x), m_y(y), m_press(press),
		m_button(button), m_locked(locked) {}
	InputMouseButton::~InputMouseButton() {}
	Core::EventType InputMouseButton::getType() const { return m_type; }
	void InputMouseButton::getValues(int& x, int&y) const { x = m_x; y = m_y; }
	int InputMouseButton::getButton() const { return m_button; }
	bool InputMouseButton::pressed() const { return m_press; }
	bool InputMouseButton::isLocked() const { return m_locked; }

	//-------------------------------------------------------- Collision events
	CollisionEvent::CollisionEvent(long obj1, long obj2) :
		IEvent(EVENT_COLLISION), m_obj1(obj1), m_obj2(obj2) {}
	CollisionEvent::~CollisionEvent() {}
	EventType CollisionEvent::getType() const { return m_type; }
	void CollisionEvent::getObjectIds(long& obj1, long& obj2) {
		obj1 = m_obj1;
		obj2 = m_obj2;
	}

	ClosestRayTestEvent::ClosestRayTestEvent(long obj) :
		IEvent(EVENT_RAY_TEST_CLOSEST), m_obj(obj) {}
	ClosestRayTestEvent::~ClosestRayTestEvent() {}
	EventType ClosestRayTestEvent::getType() const { return m_type; }
	long ClosestRayTestEvent::getObjectId() { return m_obj; }

	AllRayTestEvent::AllRayTestEvent(std::vector<long>&& objs) :
		IEvent(EVENT_RAY_TEST_ALL), m_obj_ids(std::move(objs)) {}
	AllRayTestEvent::~AllRayTestEvent() {}
	EventType AllRayTestEvent::getType() const { return m_type; }
}