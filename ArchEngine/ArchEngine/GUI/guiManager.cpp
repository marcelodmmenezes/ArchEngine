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
			"GUIManager destructor");
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

		generateQuads();

		FT_Done_Face(m_font.face); // TODO: correct resources
		FT_Done_FreeType(m_ft);

		return success;
	}

	bool GUIManager::initializeFromConfigFile(const std::string& path) {
		bool success;

		LuaScript lua_context;
		lua_context.initialize(path);

		auto fonts = lua_context.getStringVector("fonts");

		m_shader.initialize(
			lua_context.get<std::string>("vsshader"),
			lua_context.get<std::string>("fsshader")
		);

		// TODO: dynamic load projetion matrix
		m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f); 

		success = initialize(fonts);

		lua_context.destroy();

		return success;
	}

	void GUIManager::destroy() {
		for (auto& it : m_font.characters)
			if (glIsTexture(it.second.texture_id))
				glDeleteTextures(1, &it.second.texture_id);

		if (glIsBuffer(m_quad_vbo))
			glDeleteBuffers(1, &m_quad_vbo);

		if (glIsVertexArray(m_quad_vao))
			glDeleteVertexArrays(1, &m_quad_vao);

		m_shader.destroy();

		m_state = SAFE_TO_DESTROY;
	}

	void GUIManager::renderText(const std::string& text, float x,
		float y, float scale, const glm::vec3& color) {
		m_shader.bind();

		m_shader.setMat4("u_projection_matrix", m_projection);
		m_shader.setInt("u_texture", 0);
		m_shader.setVec3("u_color", color);

		m_shader.update();

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_quad_vao);

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character ch = m_font.characters[*c];

			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};

			glBindTexture(GL_TEXTURE_2D, ch.texture_id);
			glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.advance >> 6) * scale;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
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

	void GUIManager::generateQuads() {
		glGenVertexArrays(1, &m_quad_vao);
		glBindVertexArray(m_quad_vao);

		glGenBuffers(1, &m_quad_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
			nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			4 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}