/*===========================================================================*
 * Arch Engine - "GUI/guiManager.hpp"                                        *
 *                                                                           *
 * Class responsible for managing all the Graphical User Interface           *
 * associated stuff.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/06/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_GUI_MANAGER_HPP
#define GUI_GUI_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "../Core/eventManager.hpp"
#include "../Graphics/glad_3_3_core.hpp"
#include "../Graphics/shader.hpp"
#include "guiComponent.hpp"
#include "pushButton.hpp"
#include "renderableComponent.hpp"
#include "writableComponent.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <vector>


namespace GUI {
	struct Character {
		unsigned texture_id;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned advance;
	};

	struct Font {
		int size;
		std::map<char, Character> characters;
	};

	class GUIManager {
	public:
		~GUIManager();

		GUIManager(const GUIManager&) = delete;
		void operator=(const GUIManager&) = delete;

		static GUIManager& getInstance();

		bool initialize(const std::vector<std::string>& fonts);
		bool initializeFromConfigFile(const std::string& path);
		
		void update(float delta_time);

		void destroy();

		int addFont(const std::string& path, int size);

		void setProjection(const glm::mat4& proj);
		glm::mat4 getProjection() const;

		float renderText(unsigned font_id, const std::string& text,
			float x, float y, float scale, const glm::vec3& color);

		int getFontSize(unsigned font_id);
		float getCharLength(char c, unsigned font_id, float scale) const;

		//------------------------------------------------------------ CONTROLS
		unsigned addControl(GUIComponent* control);
		GUIComponent* getControl(unsigned handle);
		void removeControl(unsigned handle);
		//---------------------------------------------------------------------

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		GUIManager();

		void loadCharacters(const FT_Face& face, Font& font);
		void onWindowResizeEvent(Core::EventPtr e);

		Core::EventListener m_window_size_listener;

		glm::vec2 m_window_size;

		State m_state;

		std::vector<Font> m_fonts;
		
		Graphics::Shader m_shader;

		glm::mat4 m_projection;

		unsigned m_quad_vao;
		unsigned m_quad_vbo;

		//------------------------------------------------------------ CONTROLS
		std::vector<GUIComponent*> m_controls;
		//---------------------------------------------------------------------
	};
}


#endif	// GUI_GUI_MANAGER_HPP