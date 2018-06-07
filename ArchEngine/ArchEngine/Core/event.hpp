/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 07/06/2018                                                 *
 *===========================================================================*/


#ifndef CORE_EVENT_HPP
#define CORE_EVENT_HPP


#include "../Utils/delegate.hpp"

#include <memory>
#include <string>


namespace Core {
	//------------------------------------------------------------- Event types
	enum EventType {
		//--- Test events
		EVENT_TEST_1,
		EVENT_TEST_2,
		EVENT_TEST_3,

		//--- Core events
		EVENT_CORE_QUIT,
		EVENT_CORE_TIMER,

		//--- Window events
		EVENT_WINDOW_RESIZE,

		//--- Input events
		EVENT_INPUT_ACTION,
		EVENT_INPUT_STATE,
		EVENT_INPUT_RANGE,
		EVENT_INPUT_CONTEXT,

		//--- Collision events
		EVENT_COLLISION,

		//--- File watcher events
		EVENT_FILE_MODIFIED,
		EVENT_WATCH_FILE,

		//--- Control
		NUMBER_OF_DEFAULT_EVENTS
	};

	//--------------------------------------------------------- Event interface
	class IEvent {
	public:
		IEvent(EventType type);
		virtual ~IEvent() = 0;
		
		virtual EventType getType() const = 0;
		
	protected:
		EventType m_type;
	};
	
	//---------------------------------------------------------------- Typedefs
	typedef std::shared_ptr<IEvent> EventPtr;
	typedef Utils::Delegate<void(EventPtr)> EventListener;


	//----------------
	//---- Core events
	//----------------
	class CoreQuitEvent : public IEvent {
	public:
		CoreQuitEvent();
		~CoreQuitEvent();

		EventType getType() const override;
	};

	class CoreTimerEvent : public IEvent {
	public:
		CoreTimerEvent(unsigned time);
		~CoreTimerEvent();

		EventType getType() const override;
		unsigned getTime() const;

	private:
		unsigned m_time;
	};
	//-------------------------------------------------------------------------

	//------------------
	//---- Window events
	//------------------
	class WindowResizeEvent : public IEvent {
	public:
		WindowResizeEvent(int width, int height);
		~WindowResizeEvent();

		EventType getType() const override;
		void getSize(int& width, int& height);

	private:
		int m_width;
		int m_height;
	};
	//-------------------------------------------------------------------------


	//-------------------
	//---- Physics events
	//-------------------
	class CollisionEvent : public IEvent {
	public:
		CollisionEvent(long obj1, long obj2);
		~CollisionEvent();

		EventType getType() const override;
		void getObjectIds(long& obj1, long& obj2);

	private:
		long m_obj1;
		long m_obj2;
	};
	//-------------------------------------------------------------------------
}


#endif	// CORE_EVENT_HPP