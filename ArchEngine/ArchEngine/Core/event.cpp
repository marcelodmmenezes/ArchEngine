/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/


#include "event.hpp"


namespace Core {
	IEvent::IEvent(EventType type) : m_type(type) {}
	// Virtual pure destructor needs implementation
	IEvent::~IEvent() {}
	EventType IEvent::getType() const { return m_type; }

	//------------------------------------------------------------- Core events
	CoreQuitEvent::CoreQuitEvent() : IEvent(EVENT_CORE_QUIT) {}
	CoreQuitEvent::~CoreQuitEvent() {}

	LoopFinishedEvent::LoopFinishedEvent(double delta_time, double frame_rate)
		: IEvent(EVENT_LOOP_FINISHED), m_delta_time(delta_time),
		m_frame_rate(frame_rate) {}
	LoopFinishedEvent::~LoopFinishedEvent() {}
	double LoopFinishedEvent::getDeltaTime() const { return m_delta_time; }
	double LoopFinishedEvent::getFrameRate() const { return m_frame_rate; }

	//-------------------------------------------------------------- GUI events
	ButtonStateEvent::ButtonStateEvent(int button_id, bool state) :
		IEvent(EVENT_BUTTON_STATE), m_id(button_id), m_state(state) {}
	ButtonStateEvent::~ButtonStateEvent() {}
	int ButtonStateEvent::getButtonId() const { return m_id; }
	bool ButtonStateEvent::getState() const { return m_state; }

	//----------------------------------------------------------- Window events
	WindowResizeEvent::WindowResizeEvent(int width, int height) :
		IEvent(EVENT_WINDOW_RESIZE), m_width(width), m_height(height) {}
	WindowResizeEvent::~WindowResizeEvent() {}
	void WindowResizeEvent::getSize(int& width, int& height) {
		width = m_width;
		height = m_height;
	}

	//------------------------------------------------------------ Input events
	InputMouseMoved::InputMouseMoved(int x, int y, bool locked) :
		IEvent(EVENT_MOUSE_MOVED), m_x(x), m_y(y), m_locked(locked) {}
	InputMouseMoved::~InputMouseMoved() {}
	void InputMouseMoved::getValues(int& x, int&y) const { x = m_x; y = m_y; }
	bool InputMouseMoved::isLocked() const { return m_locked; }

	InputMouseButton::InputMouseButton(int x, int y,
		int button, bool press, bool locked) :
		IEvent(EVENT_MOUSE_BUTTON), m_x(x), m_y(y), m_press(press),
		m_button(button), m_locked(locked) {}
	InputMouseButton::~InputMouseButton() {}
	void InputMouseButton::getValues(int& x, int&y) const { x = m_x; y = m_y; }
	int InputMouseButton::getButton() const { return m_button; }
	bool InputMouseButton::pressed() const { return m_press; }
	bool InputMouseButton::isLocked() const { return m_locked; }

	//--------------------------------------------------------- Graphics events
	PrePostProcessEvent::PrePostProcessEvent() :
		IEvent(EVENT_BEFORE_POST_PROCESS) {}
	PrePostProcessEvent::~PrePostProcessEvent() {}

	//---------------------------------------------------------- Physics events
	CollisionEvent::CollisionEvent(long obj1, long obj2) :
		IEvent(EVENT_COLLISION), m_obj1(obj1), m_obj2(obj2) {}
	CollisionEvent::~CollisionEvent() {}
	void CollisionEvent::getObjectIds(long& obj1, long& obj2) {
		obj1 = m_obj1;
		obj2 = m_obj2;
	}

	ClosestRayTestEvent::ClosestRayTestEvent(long obj) :
		IEvent(EVENT_RAY_TEST_CLOSEST), m_obj(obj) {}
	ClosestRayTestEvent::~ClosestRayTestEvent() {}
	long ClosestRayTestEvent::getObjectId() { return m_obj; }

	AllRayTestEvent::AllRayTestEvent(std::vector<long>&& objs) :
		IEvent(EVENT_RAY_TEST_ALL), m_obj_ids(std::move(objs)) {}
	AllRayTestEvent::~AllRayTestEvent() {}
}