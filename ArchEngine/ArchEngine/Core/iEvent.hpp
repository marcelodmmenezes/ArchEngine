/*===========================================================================*
 * Arch Engine - "Core/iEvent.hpp                                            *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_I_EVENT_HPP
#define CORE_I_EVENT_HPP


#include <string>


namespace Core {
	enum EventType {
		TEST_EVENT_1,
		TEST_EVENT_2,
		TEST_EVENT_3
	};

	class IEvent {
	public:
		IEvent(EventType type) : m_type(type) {}
		virtual ~IEvent() = 0 {}

		virtual EventType getType() = 0;

	protected:
		EventType m_type;
	};
}


#endif