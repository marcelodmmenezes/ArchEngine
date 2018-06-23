/*===========================================================================*
 * Arch Engine - "GUI/guiComponent.cpp"                                      *
 *                                                                           *
 * Represents a generic GUI component.                                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 21/06/2018                                                 *
 *===========================================================================*/


#include "guiComponent.hpp"


using namespace Core;
using namespace OS;


namespace GUI {
	GUIComponent::GUIComponent() {
		m_mouse_moved_listener.bind<GUIComponent,
			&GUIComponent::onMouseMovedEvent>(this);
	}

	GUIComponent::~GUIComponent() {}

	void GUIComponent::trackMouse() {
		EventManager::getInstance().addListener(
			m_mouse_moved_listener, EVENT_MOUSE_MOVED);
	}

	void GUIComponent::untrackMouse() {
		EventManager::getInstance().removeListener(
			m_mouse_moved_listener, EVENT_MOUSE_MOVED);
	}

	void GUIComponent::onMouseMovedEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<InputMouseMoved>(e);

		evnt->getValues(m_mouse_x, m_mouse_y);

		if (!evnt->isLocked()) {
			// Checking if mouse is inside control
			if (m_mouse_x >= m_limits.x && m_mouse_x <= m_limits.z &&
				m_mouse_y >= m_limits.y && m_mouse_y <= m_limits.w) {
				mouseHover();
			}
			else
				mouseOut();
		}
	}
}