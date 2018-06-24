/*===========================================================================*
 * Arch Engine - "GUI/guiComponent.hpp"                                      *
 *                                                                           *
 * Represents a generic GUI component.                                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 23/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_GUI_COMPONENT_HPP
#define GUI_GUI_COMPONENT_HPP


#include "../Config/engineMacros.hpp"
#include "../Core/eventManager.hpp"
#include "guiManager.hpp"
#include "../OS/inputManager.hpp"
#include "../Utils/serviceLocator.hpp"


#include <glm/glm.hpp>

#include <vector>


namespace GUI {
	class GUIComponent {
	public:
		GUIComponent();
		virtual ~GUIComponent() = 0;

		//------------------------------------------------------ Mouse tracking
		void trackMouse();
		void untrackMouse();
		//---------------------------------------------------------------------

	protected:
		//------------------------------------------------------ Mouse tracking
		virtual void mouseHover() = 0;
		virtual void mouseOut() = 0;

		void onMouseMovedEvent(Core::EventPtr e);
		Core::EventListener m_mouse_moved_listener;

		int m_mouse_x;
		int m_mouse_y;

		glm::vec4 m_mouse_space;
		//---------------------------------------------------------------------

		glm::vec2 m_position;
	};
}


#endif	// GUI_GUI_COMPONENT_HPP