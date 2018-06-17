/*===========================================================================*
 * Arch Engine - "Graphics/guiManager.hpp"                                   *
 *                                                                           *
 * Class responsible for managing all the Graphical User Interface           *
 * associated stuff.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/07/2018                                                       *
 * Last Modified: 17/06/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <string>


namespace GUI {
	class GUIManager {
	public:
		~GUIManager();

		GUIManager(const GUIManager&) = delete;
		void operator=(const GUIManager&) = delete;

		static GUIManager& getInstance();

		bool initializeFromConfigFile(const std::string& path);
		
	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		GUIManager();

		State m_state;
	};
}