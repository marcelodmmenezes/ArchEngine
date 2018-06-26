/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/


#ifndef CORE_EVENT_HPP
#define CORE_EVENT_HPP


#include "../Utils/delegate.hpp"

#include <memory>
#include <string>
#include <vector>


namespace Core {
	//------------------------------------------------------------- Event types
	enum EventType {
		//--- Core events
		EVENT_CORE_QUIT,
		EVENT_LOOP_FINISHED,

		//--- GUI events
		EVENT_BUTTON_STATE,

		//--- Window events
		EVENT_WINDOW_RESIZE,

		//--- Input events
		EVENT_INPUT_ACTION,
		EVENT_INPUT_STATE,
		EVENT_INPUT_RANGE,
		EVENT_MOUSE_MOVED,
		EVENT_MOUSE_BUTTON,
		EVENT_INPUT_CONTEXT,

		//--- Graphics events
		EVENT_BEFORE_POST_PROCESS,

		//--- Physics events
		EVENT_COLLISION,
		EVENT_RAY_TEST_CLOSEST,
		EVENT_RAY_TEST_ALL,

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
		
		EventType getType() const;
		
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
	};

	class LoopFinishedEvent : public IEvent {
	public:
		LoopFinishedEvent(double delta_time, double frame_rate);
		~LoopFinishedEvent();

		double getDeltaTime() const;
		double getFrameRate() const;

	private:
		double m_delta_time;
		double m_frame_rate;
	};
	//-------------------------------------------------------------------------


	//---------------
	//---- GUI events
	//---------------
	class ButtonStateEvent : public IEvent {
	public:
		ButtonStateEvent(int button_id, bool state);
		~ButtonStateEvent();

		int getButtonId() const;
		bool getState() const;

	private:
		int m_id;
		bool m_state; // true - pressed, false - released
	};
	//-------------------------------------------------------------------------

	//------------------
	//---- Window events
	//------------------
	class WindowResizeEvent : public IEvent {
	public:
		WindowResizeEvent(int width, int height);
		~WindowResizeEvent();

		void getSize(int& width, int& height);

	private:
		int m_width;
		int m_height;
	};
	//-------------------------------------------------------------------------

	//-----------------
	//---- Input events
	//-----------------
	class InputMouseMoved : public Core::IEvent {
	public:
		InputMouseMoved(int x, int y, bool locked);
		~InputMouseMoved();

		void getValues(int& x, int&y) const;
		bool isLocked() const;

	private:
		int m_x;
		int m_y;
		bool m_locked;
	};

	class InputMouseButton : public Core::IEvent {
	public:
		InputMouseButton(int x, int y, int button, bool press, bool locked);
		~InputMouseButton();

		void getValues(int& x, int&y) const;
		int getButton() const;
		bool pressed() const;
		bool isLocked() const;

	private:
		int m_x;
		int m_y;
		int m_button; // 1 - left, 2 - middle, 3 - right
		bool m_press; // true - press, false - release
		bool m_locked;
	};
	//-------------------------------------------------------------------------

	//--------------------
	//---- Graphics events
	//--------------------
	class PrePostProcessEvent : public IEvent {
	public:
		PrePostProcessEvent();
		~PrePostProcessEvent();
	};
	//-------------------------------------------------------------------------

	//-------------------
	//---- Physics events
	//-------------------
	class CollisionEvent : public IEvent {
	public:
		CollisionEvent(long obj1, long obj2);
		~CollisionEvent();

		void getObjectIds(long& obj1, long& obj2);

	private:
		long m_obj1;
		long m_obj2;
	};

	class ClosestRayTestEvent : public IEvent {
	public:
		ClosestRayTestEvent(long obj);
		~ClosestRayTestEvent();

		long getObjectId();

	private:
		long m_obj;
	};

	class AllRayTestEvent : public IEvent {
	public:
		AllRayTestEvent(std::vector<long>&& objs);
		~AllRayTestEvent();

		std::vector<long> m_obj_ids;
	};
	//-------------------------------------------------------------------------
}


#endif	// CORE_EVENT_HPP