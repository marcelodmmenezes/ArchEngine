/*===========================================================================*
 * Arch Engine - "Core/eventManager.hpp                                      *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#ifndef CORE_EVENT_MANAGER_HPP
#define CORE_EVENT_MANAGER_HPP


#include "iEvent.hpp"
#include "../Utils/serviceLocator.hpp"


namespace Core {
	class EventManager {
	public:
		EventManager(const EventManager&) = delete;
		void operator=(const EventManager&) = delete;

		static EventManager& getInstance();

	private:
		EventManager();
	};
}


#endif