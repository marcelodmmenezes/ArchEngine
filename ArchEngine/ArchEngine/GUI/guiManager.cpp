/*===========================================================================*
 * Arch Engine - "GUI/guiManager.cpp"                                        *
 *                                                                           *
 * Class responsible for managing all the Graphical User Interface           *
 * associated stuff.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 17/06/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
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

		m_state = INITIALIZED;

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

		if (proj.size() == 4) {
			m_projection = glm::ortho(proj[0], proj[1], proj[2], proj[3]);
			m_window_size = glm::vec2(proj[1], proj[3]);
		}
		else {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				path + " projection matrix not informed correctly."
				"Assigning default");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			m_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
			m_window_size = glm::vec2(800.0f, 600.0f);
		}

		success = initialize(fonts);

		if (success) {
			std::string controls = lua_context.get<std::string>("controls");
			success = createControlsFromFile(controls);
		}

		lua_context.destroy();

		return success;
	}

	bool GUIManager::createControlsFromFile(const std::string& path) {
		bool success = true;

		LuaScript lua_context;
		
		if (!lua_context.initialize(path))
			return false;

		int i = 1;
		int aux;
		bool found;
		std::string name, type;

		try {
			do {
				found = false;

				name = "component" + std::to_string(i);

				if (lua_context.getToStack(name, aux)) {
					type = lua_context.get<std::string>(name + ".tp");

					if (type == "renderableComponent")
						createRenderableComponent(lua_context, name);
					else if (type == "writableComponent")
						createWritableComponent(lua_context, name);
					else if (type == "pushButton")
						createPushButton(lua_context, name);

					found = true;
				}

				i++;
			} while (found);
		}
		catch (...) {
			success = false;
		}

		lua_context.destroy();

		return success;
	}

	void GUIManager::createRenderableComponent(LuaScript& lua_context,
		const std::string& name) {
		auto id =
			lua_context.get<std::string>(name + ".id");
		auto shader_id =
			lua_context.get<int>(name + ".shader_id");
		auto limits =
			lua_context.getFloatVector(name + ".limits");
		auto texture =
			lua_context.get<std::string>(name + ".texture");
		auto color =
			lua_context.getFloatVector(name + ".color");
		auto hover_color =
			lua_context.getFloatVector(name + ".hover_color");
		auto border_color =
			lua_context.getFloatVector(name + ".border_color");
		auto border_width =
			lua_context.get<int>(name + ".border_width");
		auto projection =
			lua_context.getFloatVector(name + ".projection");
		auto border_constraint =
			lua_context.getIntVector(name + ".border_constraint");
		auto track_mouse =
			lua_context.get<bool>(name + ".track_mouse");

		auto rc = new RenderableComponent(
			shader_id,
			m_window_size,
			glm::vec4(limits[0], limits[1], limits[2], limits[3]),
			texture
		);

		if (color.size() == 4)
			rc->setColor(glm::vec4(color[0],
				color[1], color[2], color[3]));

		if (hover_color.size() == 4)
			rc->setHoverColor(glm::vec4(hover_color[0],
				hover_color[1], hover_color[2], hover_color[3]));

		if (border_color.size() == 4)
			rc->setBorderColor(glm::vec4(border_color[0],
				border_color[1], border_color[2], border_color[3]));

		if (border_width > 0)
			rc->setBorderWidth(border_width);

		if (projection.size() == 4)
			rc->setProjection(glm::ortho(projection[0],
				projection[1], projection[2], projection[3]));
		else
			rc->setProjection(glm::ortho(0.0f,
				(float)m_window_size.x, 0.0f, (float)m_window_size.y));

		int lim = std::min(4, (int)border_constraint.size());
		int mask = 0;
		for (int i = 0; i < lim; i++)
			if (border_constraint[i])
				mask += 1 << i;
		rc->setBorderConstraint(mask);

		if (track_mouse)
			rc->trackMouse();

		m_controls.push_back(rc);
		m_control_name_to_handle[id] = m_controls.size() - 1;
	}

	void GUIManager::createWritableComponent(LuaScript& lua_context,
		const std::string& name) {
		auto id =
			lua_context.get<std::string>(name + ".id");
		auto text =
			lua_context.get<std::string>(name + ".text");
		auto position =
			lua_context.getFloatVector(name + ".position");
		auto font_id =
			lua_context.get<int>(name + ".font_id");
		auto font_scale =
			lua_context.get<float>(name + ".font_scale");
		auto text_spacing =
			lua_context.get<int>(name + ".text_spacing");
		auto color =
			lua_context.getFloatVector(name + ".color");
		auto hover_color =
			lua_context.getFloatVector(name + ".hover_color");
		auto maximum_size =
			lua_context.getFloatVector(name + ".maximum_size");
		auto anchored =
			lua_context.get<bool>(name + ".anchored");
		auto track_mouse =
			lua_context.get<bool>(name + ".track_mouse");

		auto wc = new WritableComponent(
			m_window_size,
			glm::vec2(position[0], position[1]),
			font_id,
			font_scale,
			text_spacing,
			glm::vec3(color[0], color[1], color[2]),
			glm::vec2(maximum_size[0], maximum_size[1])
		);

		wc->write(text);

		if (hover_color.size() == 4)
			wc->setHoverColor(glm::vec3(hover_color[0],
				hover_color[1], hover_color[2]));

		wc->anchor(anchored);

		if (track_mouse)
			wc->trackMouse();

		m_controls.push_back(wc);
		m_control_name_to_handle[id] = m_controls.size() - 1;
	}

	void GUIManager::createPushButton(LuaScript& lua_context,
		const std::string& name) {
		auto id =
			lua_context.get<std::string>(name + ".id");
		auto button_id =
			lua_context.get<int>(name + ".button_id");
		auto shader_id =
			lua_context.get<int>(name + ".shader_id");
		auto limits =
			lua_context.getFloatVector(name + ".limits");
		auto font_id =
			lua_context.get<int>(name + ".font_id");
		auto font_scale =
			lua_context.get<float>(name + ".font_scale");
		auto text_color =
			lua_context.getFloatVector(name + ".text_color");
		auto texture =
			lua_context.get<std::string>(name + ".texture");
		auto text =
			lua_context.get<std::string>(name + ".text");
		auto render_color =
			lua_context.getFloatVector(name + ".render_color");
		auto render_hover_color =
			lua_context.getFloatVector(
				name + ".render_hover_color");
		auto render_border_color =
			lua_context.getFloatVector(
				name + ".render_border_color");
		auto render_border_width =
			lua_context.get<int>(name + ".render_border_width");
		auto projection =
			lua_context.getFloatVector(name + ".projection");
		auto track_mouse =
			lua_context.get<bool>(name + ".track_mouse");

		auto pb = new PushButton(
			button_id,
			shader_id,
			m_window_size,
			glm::vec4(limits[0], limits[1], limits[2], limits[3]),
			font_id,
			font_scale,
			glm::vec3(text_color[0], text_color[1], text_color[2]),
			texture
		);

		pb->setText(text);

		if (render_color.size() == 4)
			pb->setRenderColor(glm::vec4(render_color[0],
				render_color[1], render_color[2], render_color[3]));

		if (render_hover_color.size() == 4)
			pb->setRenderHoverColor(glm::vec4(render_hover_color[0],
				render_hover_color[1], render_hover_color[2],
				render_hover_color[3]));

		if (render_border_color.size() == 4)
			pb->setRenderBorderColor(glm::vec4(render_border_color[0],
				render_border_color[1], render_border_color[2],
				render_border_color[3]));

		if (render_border_width > 0)
			pb->setRenderBorderWidth(render_border_width);

		if (projection.size() == 4)
			pb->setRenderProjection(glm::ortho(projection[0],
				projection[1], projection[2], projection[3]));
		else
			pb->setRenderProjection(glm::ortho(0.0f,
			(float)m_window_size.x, 0.0f, (float)m_window_size.y));

		if (track_mouse)
			pb->trackMouse();

		m_controls.push_back(pb);
		m_control_name_to_handle[id] = m_controls.size() - 1;
	}

	void GUIManager::update(float delta_time) {
		for (auto& it : m_controls) {
			glClear(GL_DEPTH_BUFFER_BIT);
			it->update(delta_time);
		}
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

		for (auto& it : m_controls)
			delete it;

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

	//---------------------------------------------------------------- CONTROLS
	unsigned GUIManager::addControl(GUIComponent* control) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		m_controls.push_back(control);
		return (unsigned)m_controls.size() - 1;
	}

	GUIComponent* GUIManager::getControl(unsigned handle) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		if (handle >= m_controls.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to get GUIComponent outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return m_controls[handle];
	}

	GUIComponent* GUIManager::getControl(const std::string& name) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		auto it = m_control_name_to_handle.find(name);

		if (it == m_control_name_to_handle.end()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Could not find GUIComoponent " + name);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return m_controls[it->second];
	}

	void GUIManager::removeControl(unsigned handle) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
		assert(m_state == INITIALIZED);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

		if (handle >= m_controls.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove GUIComponent outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		delete m_controls[handle];
		m_controls.erase(m_controls.begin() + handle);
	}
	//-------------------------------------------------------------------------

	void GUIManager::onWindowResizeEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<WindowResizeEvent>(e);

		int w, h;
		evnt->getSize(w, h);

		m_projection = glm::ortho(0.0f, (float)w, 0.0f, (float)h);

		for (auto& it : m_controls) {
			if (auto ctrl = dynamic_cast<RenderableComponent*>(it)) {	
				glm::vec4 lim = ctrl->getLimits();

				int mask = ctrl->getBorderConstraint();

				if (!(mask & BC_LEFT))
					lim.x += (w - m_window_size.x);

				if (!(mask & BC_BOTTOM))
					lim.y += (h - m_window_size.y);

				if (mask & BC_RIGHT)
					lim.z += (w - m_window_size.x);

				if (mask & BC_TOP)
					lim.w += (h - m_window_size.y);

				ctrl->setLimits(lim);
				ctrl->setProjection(m_projection);
			}
			else if (auto ctrl = dynamic_cast<WritableComponent*>(it)) {
				glm::vec2 pos = ctrl->getPosition();
				glm::vec2 m_size = ctrl->getMaximumSize();

				if (!ctrl->isAnchored())
					ctrl->setPosition(glm::vec2(pos.x,
						pos.y + (h - m_window_size.y)));

				ctrl->setMaximumSize(glm::vec2(m_size.x, h - m_size.y));
			}
			else if (auto ctrl = dynamic_cast<PushButton*>(it))
				ctrl->setRenderProjection(m_projection);
		}

		m_window_size = glm::vec2(w, h);
	}
}