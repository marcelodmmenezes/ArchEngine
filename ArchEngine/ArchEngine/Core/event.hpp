/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 05/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_I_EVENT_HPP
#define CORE_I_EVENT_HPP


#include "../OS/inputManager.hpp"

#include <string>


namespace Core {
	//------------------------------------------------------------- Event types
	enum EventType {
		//--- Test events
		TEST_EVENT_1,
		TEST_EVENT_2,
		TEST_EVENT_3,

		//--- Core events
		CORE_QUIT_EVENT,

		//--- Input events
		INPUT_ACTION_EVENT,
		INPUT_STATE_EVENT,
		INPUT_RANGE_EVENT
	};

	//--------------------------------------------------------- Event interface
	class IEvent {
	public:
		IEvent(EventType type) : m_type(type) {}
		virtual ~IEvent() = 0 {}
		
		virtual EventType getType() const = 0;
		
	protected:
		EventType m_type;
	};

	//-------------------------------------------------- Event concrete classes
	
	//----------------
	//---- Core events
	//----------------

	class CoreQuitEvent : public IEvent {
	public:
		CoreQuitEvent();
		~CoreQuitEvent();

		EventType getType() const override;

	private:
	};

	//----------------
	//--- Input events
	//----------------

	class InputActionEvent : public IEvent {
	public:
		InputActionEvent();
		InputActionEvent(OS::InputAction value);
		~InputActionEvent();

		EventType getType() const override;

		OS::InputAction getValue() const;
		void setValue(OS::InputAction value);

	private:
		// Input is based in a unsigned. See inputContext.hpp for more details
		OS::InputAction m_value;
	};

	class InputStateEvent : public IEvent {
	public:
		InputStateEvent();
		InputStateEvent(OS::InputState value);
		~InputStateEvent();

		EventType getType() const override;

		OS::InputState getValue() const;
		void setValue(OS::InputState value);

	private:
		OS::InputState m_value;
	};

	class InputRangeEvent : public IEvent {
	public:
		InputRangeEvent();
		// CAUTION: OS::RangeInfo is moved
		InputRangeEvent(OS::RangeInfo value);
		~InputRangeEvent();

		EventType getType() const override;

		OS::RangeInfo getValue() const;
		void setValue(OS::RangeInfo value);

	private:
		OS::RangeInfo m_value;
	};
}


#endif