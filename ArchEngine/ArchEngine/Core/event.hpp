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


#include "../Utils/delegate.hpp"

#include <memory>
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
	
	//---------------------------------------------------------------- Typedefs
	typedef std::shared_ptr<IEvent> EventPtr;
	typedef Utils::Delegate<void(EventPtr)> EventListener;
}


#endif