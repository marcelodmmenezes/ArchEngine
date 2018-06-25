/*===========================================================================*
 * Arch Engine - "GUI/guiComponent.hpp"                                      *
 *                                                                           *
 * Represents a generic GUI component.                                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_GUI_COMPONENT_HPP
#define GUI_GUI_COMPONENT_HPP


#include "../Config/engineMacros.hpp"
#include "../Core/eventManager.hpp"
#include "../OS/inputManager.hpp"
#include "../Utils/serviceLocator.hpp"


#include <glm/glm.hpp>

#include <vector>


namespace GUI {
	class GUIComponent {
	public:
		GUIComponent();
		virtual ~GUIComponent() = 0;

		virtual void update(float delta_time) = 0;

		//------------------------------------------------------ Mouse tracking
		virtual void trackMouse();
		virtual void untrackMouse();
		//---------------------------------------------------------------------

	protected:
		//------------------------------------------------------ Mouse tracking
		virtual void mouseHover(int x, int y) = 0;
		virtual void mouseOut() = 0;

		virtual void mouseDown(int x, int y, int button) = 0;
		virtual void mouseUp(int x, int y, int button) = 0;

		void onMouseMovedEvent(Core::EventPtr e);
		void onMouseButtonEvent(Core::EventPtr e);

		Core::EventListener m_mouse_moved_listener;
		Core::EventListener m_mouse_button_listener;

		int m_mouse_x;
		int m_mouse_y;

		glm::vec4 m_mouse_space;
		//---------------------------------------------------------------------

		glm::vec2 m_position;
	};
}


#endif	// GUI_GUI_COMPONENT_HPP