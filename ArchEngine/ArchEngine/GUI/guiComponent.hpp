/*===========================================================================*
 * Arch Engine - "GUI/guiComponent.hpp"                                      *
 *                                                                           *
 * Represents a generic GUI component.                                       *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 21/06/2018                                                       *
 * Last Modified: 21/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_GUI_COMPONENT_HPP
#define GUI_GUI_COMPONENT_HPP


#include "../Config/engineMacros.hpp"
#include "guiManager.hpp"
#include "../Utils/serviceLocator.hpp"


#include <glm/glm.hpp>

#include <vector>


namespace GUI {
	class GUIComponent {
	public:
		GUIComponent();
		~GUIComponent();

	protected:
		glm::vec2 m_position;
		glm::vec2 m_size;
		GUIComponent* m_parent;
	};
}


#endif	// GUI_GUI_COMPONENT_HPP