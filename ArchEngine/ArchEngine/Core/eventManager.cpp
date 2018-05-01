/*===========================================================================*
 * Arch Engine - "Core/eventManager.cpp                                      *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 01/05/2018                                                       *
 * Last Modified: 01/05/2018                                                 *
 *===========================================================================*/


#include "eventManager.hpp"


namespace Core {
	EventManager::EventManager() {}

	EventManager& EventManager::getInstance() {
		static EventManager instance;
		return instance;
	}
}