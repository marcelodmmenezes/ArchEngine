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
	class IEvent {
	public:
		virtual std::string test() = 0;
	private:
	};
}


#endif