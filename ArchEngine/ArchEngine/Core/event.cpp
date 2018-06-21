/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 06/05/2018                                                       *
 * Last Modified: 07/06/2018                                                 *
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

	LoopFinishedEvent::LoopFinishedEvent(float delta_time) :
		IEvent(EVENT_LOOP_FINISHED), m_delta_time(delta_time) {}
	LoopFinishedEvent::~LoopFinishedEvent() {}
	EventType LoopFinishedEvent::getType() const { return m_type; }
	unsigned LoopFinishedEvent::getDeltaTime() const { return m_delta_time; }

	//----------------------------------------------------------- Window events
	WindowResizeEvent::WindowResizeEvent(int width, int height) :
		IEvent(EVENT_WINDOW_RESIZE), m_width(width), m_height(height) {}
	WindowResizeEvent::~WindowResizeEvent() {}
	EventType WindowResizeEvent::getType() const { return m_type; }
	void WindowResizeEvent::getSize(int& width, int& height) {
		width = m_width;
		height = m_height;
	}

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