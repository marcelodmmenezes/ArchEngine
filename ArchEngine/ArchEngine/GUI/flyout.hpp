/*===========================================================================*
 * Arch Engine - "GUI/flyout.hpp"                                            *
 *                                                                           *
 * Flyout.                                                                   *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/06/2018                                                       *
 * Last Modified: 25/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_FLYOUT_HPP
#define GUI_FLYOUT_HPP


#include "guiComponent.hpp"
#include "renderableComponent.hpp"

#include <glm/glm.hpp>


namespace GUI {
	class Flyout : public GUIComponent {
	public:
		Flyout();
		Flyout();
		~Flyout();

		void open();
		void close();

	private:
		int m_final_x;
		float interpolation_factor;
	};
}


#endif	// GUI_FLYOUT_HPP