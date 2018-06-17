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


#ifndef GUI_GUI_MANAGER_HPP
#define GUI_GUI_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "../Graphics/glad_3_3_core.hpp"
#include "../Graphics/shader.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>


namespace GUI {
	struct Character {
		unsigned texture_id;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned advance;
	};

	struct Font {
		FT_Face face;
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
		
		void destroy();

		void renderText(const std::string& text, float x,
			float y, float scale, const glm::vec3& color);

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		GUIManager();

		bool loadFont(const std::string& font);
		void loadCharacters();
		void generateQuads();

		State m_state;

		FT_Library m_ft;

		Font m_font; // TODO: multiple fonts
		
		Graphics::Shader m_shader;

		glm::mat4 m_projection;

		unsigned m_quad_vao;
		unsigned m_quad_vbo;
	};
}


#endif	// GUI_GUI_MANAGER_HPP