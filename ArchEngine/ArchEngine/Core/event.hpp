/*===========================================================================*
 * Arch Engine - "Core/event.hpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 02/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_I_EVENT_HPP
#define CORE_I_EVENT_HPP


#include <string>


namespace Core {
	//------------------------------------------------------------- Event types
	enum EventType {
		//--- Test events
		TEST_EVENT_1,
		TEST_EVENT_2,
		TEST_EVENT_3,

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
	//--- Input events
	//----------------

	class InputActionEvent : public IEvent {
	public:
		InputActionEvent();
		~InputActionEvent();

		EventType getType() const override;

		unsigned getValue() const;
		void setValue(unsigned value);

	private:
		// Input is based in a unsigned. See inputContext.hpp for more details
		unsigned m_value;
	};

	class InputStateEvent : public IEvent {
	public:
		InputStateEvent();
		~InputStateEvent();

		EventType getType() const override;

		unsigned getValue() const;
		void setValue(unsigned value);

	private:
		unsigned m_value;
	};
}


#endif