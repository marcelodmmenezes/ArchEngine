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


#include "guiManager.hpp"


using namespace Script;
using namespace Utils;


namespace GUI {
	GUIManager::GUIManager() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GUIManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GUIManager::~GUIManager() {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state != INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GUIManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GUIManager& GUIManager::getInstance() {
		static GUIManager instance;
		return instance;
	}

	bool GUIManager::initialize(const std::vector<std::string>& fonts) {
		bool success;

		if (FT_Init_FreeType(&m_ft)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not initialize FreeType");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		for (auto& it : fonts) {
			success = loadFont(it);
			if (!success)
				break;
		}

		FT_Done_Face(m_font.face);
		FT_Done_FreeType(m_ft);

		return success;
	}

	bool GUIManager::initializeFromConfigFile(const std::string& path) {
		bool success;

		LuaScript lua_context;
		lua_context.initialize(path);

		auto fonts = lua_context.getStringVector("fonts");

		success = initialize(fonts);

		lua_context.destroy();

		return success;
	}

	bool GUIManager::loadFont(const std::string& font) {
		if (FT_New_Face(m_ft, font.c_str(), 0, &m_font.face)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not load " + font + " font");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return false;
		}

		m_font.size = 48;
		FT_Set_Pixel_Sizes(m_font.face, 0, m_font.size); // TODO: dynamic size

		loadCharacters();

		return true;
	}

	void GUIManager::loadCharacters() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned c = 0; c < 128; c++) {
			if (FT_Load_Char(m_font.face, c, FT_LOAD_RENDER)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
				ServiceLocator::getFileLogger()->log<LOG_WARNING>(
					"Could not load " + (char)c);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
				continue;
			}

			unsigned texture_id;
			
			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				m_font.face->glyph->bitmap.width,
				m_font.face->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE,
				m_font.face->glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character character = {
				texture_id,
				glm::ivec2(m_font.face->glyph->bitmap.width,
					m_font.face->glyph->bitmap.rows),
				glm::ivec2(m_font.face->glyph->bitmap_left,
					m_font.face->glyph->bitmap_top),
				m_font.face->glyph->advance.x
			};

			m_font.characters.insert(
				std::pair<GLchar, Character>(c, character));
		}
	}
}