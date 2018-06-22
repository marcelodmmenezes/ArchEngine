/*===========================================================================*
 * Arch Engine - "GUI/guiManager.cpp"                                        *
 *                                                                           *
 * Class responsible for managing all the Graphical User Interface           *
 * associated stuff.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/06/2018                                                       *
 * Last Modified: 22/06/2018                                                 *
 *===========================================================================*/


#include "guiManager.hpp"


using namespace Core;
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

		//------------------------------------------------------ Event handlers
		m_window_size_listener.bind
			<GUIManager, &GUIManager::onWindowResizeEvent>(this);
		EventManager::getInstance().addListener(
			m_window_size_listener, EVENT_WINDOW_RESIZE);
		//---------------------------------------------------------------------

		bool success = true;

		for (unsigned i = 0; i < fonts.size(); i += 2) {
			try {
				if (addFont(fonts[i], std::stoi(fonts[i + 1])) == -1) {
					success = false;
					break;
				}
			}
			catch (...) {
				if (addFont(fonts[i], 12) == -1) {
					success = false;
					break;
				}

				i--;
			}
		}

		return true;
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

		auto proj = lua_context.getFloatVector("projection");

		if (proj.size() == 4)
			m_projection = glm::ortho(proj[0], proj[1], proj[2], proj[3]);
		else {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				path + " projection matrix not informed correctly."
				"Assigning default");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
		}

		success = initialize(fonts);

		lua_context.destroy();

		return success;
	}

	void GUIManager::destroy() {
		for (auto& it : m_fonts)
			for (auto& c : it.characters)
				if (glIsTexture(c.second.texture_id))
					glDeleteTextures(1, &c.second.texture_id);

		if (glIsBuffer(m_quad_vbo))
			glDeleteBuffers(1, &m_quad_vbo);

		if (glIsVertexArray(m_quad_vao))
			glDeleteVertexArrays(1, &m_quad_vao);

		m_shader.destroy();

		m_state = SAFE_TO_DESTROY;
	}

	int GUIManager::addFont(const std::string& path, int size) {
		FT_Library ft;
		FT_Face face;
		Font font;

		if (FT_Init_FreeType(&ft)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not initialize FreeType");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return -1;
		}

		if (FT_New_Face(ft, path.c_str(), 0, &face)) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not load " + path + " font");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return -1;
		}

		font.size = size;
		FT_Set_Pixel_Sizes(face, 0, size);

		loadCharacters(face, font);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		m_fonts.push_back(font);
		return m_fonts.size() - 1;
	}

	void GUIManager::setProjection(const glm::mat4& proj) {
		m_projection = proj;
	}

	glm::mat4 GUIManager::getProjection() const {
		return m_projection;
	}

	float GUIManager::renderText(unsigned font_id, const std::string& text,
		float x, float y, float scale, const glm::vec3& color) {
		if (font_id >= m_fonts.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Font id higher than font vector size");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return 0.0f;
		}

		m_shader.bind();

		m_shader.setMat4("u_projection_matrix", m_projection);
		m_shader.setInt("u_texture", 0);
		m_shader.setVec3("u_color", color);

		m_shader.update();

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_quad_vao);

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character ch = m_fonts[font_id].characters[*c];

			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
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

		return x;
	}

	int GUIManager::getFontSize(unsigned font_id) {
		if (font_id > m_fonts.size() - 1) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Font id higher than font vector size");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return 0;
		}

		return m_fonts[font_id].size;
	}

	float GUIManager::getCharLength(char c,
		unsigned font_id, float scale) const {
		if (font_id > m_fonts.size() - 1) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Font id higher than font vector size");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return 0.0f;
		}

		Character ch = m_fonts[font_id].characters.at(c);
		return (ch.advance >> 6) * scale;
	}

	void GUIManager::loadCharacters(const FT_Face& face, Font& font) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned c = 0; c < 128; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
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
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
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
				glm::ivec2(face->glyph->bitmap.width,
					face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left,
					face->glyph->bitmap_top),
				(unsigned)face->glyph->advance.x
			};

			font.characters.insert(
				std::pair<GLchar, Character>(c, character));
		}
	}

	void GUIManager::onWindowResizeEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<WindowResizeEvent>(e);

		int w, h;
		evnt->getSize(w, h);

		m_projection = glm::ortho(0.0f, (float)w, 0.0f, (float)h);
	}
}