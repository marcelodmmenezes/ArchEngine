/*===========================================================================*
 * Arch Engine - "GUI/guiComponent.cpp"                                      *
 *                                                                           *
 * Represents a generic GUI component.                                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
 *===========================================================================*/


#include "guiComponent.hpp"


using namespace Core;
using namespace OS;


namespace GUI {
	GUIComponent::GUIComponent() {
		m_mouse_moved_listener.bind<GUIComponent,
			&GUIComponent::onMouseMovedEvent>(this);
		m_mouse_button_listener.bind<GUIComponent,
			&GUIComponent::onMouseButtonEvent>(this);
	}

	GUIComponent::~GUIComponent() {
		EventManager::getInstance().removeListener(
			m_mouse_moved_listener, EVENT_MOUSE_MOVED);
		EventManager::getInstance().removeListener(
			m_mouse_button_listener, EVENT_MOUSE_BUTTON);
	}

	void GUIComponent::trackMouse() {
		EventManager::getInstance().addListener(
			m_mouse_moved_listener, EVENT_MOUSE_MOVED);
		EventManager::getInstance().addListener(
			m_mouse_button_listener, EVENT_MOUSE_BUTTON);
	}

	void GUIComponent::untrackMouse() {
		EventManager::getInstance().removeListener(
			m_mouse_moved_listener, EVENT_MOUSE_MOVED);
		EventManager::getInstance().removeListener(
			m_mouse_button_listener, EVENT_MOUSE_BUTTON);
	}

	void GUIComponent::onMouseMovedEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<InputMouseMoved>(e);

		evnt->getValues(m_mouse_x, m_mouse_y);

		if (!evnt->isLocked()) {
			// Checking if mouse is inside control
			if (m_mouse_x >= m_mouse_space.x &&
				m_mouse_x <= m_mouse_space.z &&
				m_mouse_y >= m_mouse_space.y &&
				m_mouse_y <= m_mouse_space.w)
				mouseHover(m_mouse_x, m_mouse_y);
			else
				mouseOut();
		}
	}

	void GUIComponent::onMouseButtonEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<InputMouseButton>(e);

		evnt->getValues(m_mouse_x, m_mouse_y);

		if (!evnt->isLocked() &&
			m_mouse_x >= m_mouse_space.x &&
			m_mouse_x <= m_mouse_space.z &&
			m_mouse_y >= m_mouse_space.y &&
			m_mouse_y <= m_mouse_space.w) {
			if (evnt->pressed())
				mouseDown(m_mouse_x, m_mouse_y, evnt->getButton());
			else
				mouseUp(m_mouse_x, m_mouse_y, evnt->getButton());
		}
	}
}