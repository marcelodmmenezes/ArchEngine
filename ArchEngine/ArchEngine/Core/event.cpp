/*===========================================================================*
 * Arch Engine - "Core/event.cpp                                             *
 *                                                                           *
 * Simple event system for communication between engine elements.            *
 * It consists of a basic observer pattern implementation.                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 05/05/2018                                                       *
 * Last Modified: 05/05/2018                                                 *
 *===========================================================================*/


#include "event.hpp"


namespace Core {
	// Virtual pure destructor needs implementation
	IEvent::~IEvent() {}
}