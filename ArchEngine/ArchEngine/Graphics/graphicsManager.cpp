/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 26/06/2018                                                 *
 *===========================================================================*/


#include "graphicsManager.hpp"


using namespace Core;
using namespace Script;
using namespace Utils;


//------------------------------------------------------------ Lua Graphics API
int addMesh(lua_State* lua) {
	/*
	int argc = lua_gettop(lua);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Lua pushing context with " + std::to_string(argc) + " arguments");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
	assert(argc == 1);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

	std::string path(lua_tostring(lua, lua_gettop(lua)));
	lua_pop(lua, 1);

	unsigned handle = Graphics::GraphicsManager::getInstance().addMesh(path);

	lua_pushnumber(lua, handle);

	// No values returned to Lua
	return 1;
	*/
	return 0;
}

int removeMesh(lua_State* lua) {
	int argc = lua_gettop(lua);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
	ServiceLocator::getFileLogger()->log<LOG_INFO>(
		"Lua popping context with " + std::to_string(argc) + " arguments");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
	assert(argc == 1);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

	unsigned handle = (unsigned)(lua_tointeger(lua, lua_gettop(lua)));
	lua_pop(lua, 1);

	Graphics::GraphicsManager::getInstance().removeMesh(handle);

	// No values returned to Lua
	return 0;
}
//-------------------------------------------------------------------------


namespace Graphics {
	GraphicsManager::GraphicsManager() :
		m_state(CONSTRUCTED),
		m_line_vao(0),
		m_quad_vao(0),
		m_draw_skybox(false),
		m_fog(false),
		m_blur_level(0),
		m_contrast_factor(0.0f) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GraphicsManager::~GraphicsManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GraphicsManager& GraphicsManager::getInstance() {
		static GraphicsManager instance;
		return instance;
	}

	bool GraphicsManager::initialize(bool depth_test, bool face_culling,
		bool blending, int view_port[4], const glm::vec4& color,
		float fov, int active_camera) {
		if (depth_test)
			glEnable(GL_DEPTH_TEST);
		if (face_culling)
			glEnable(GL_CULL_FACE);
		if (blending) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		//------------------------------------------------------ Event handlers
		m_window_size_listener.bind
			<GraphicsManager, &GraphicsManager::onWindowResizeEvent>(this);
		EventManager::getInstance().addListener(
			m_window_size_listener, EVENT_WINDOW_RESIZE);
		//---------------------------------------------------------------------

		m_screen_width = view_port[2];
		m_screen_height = view_port[3];

		m_fov = fov;

		m_projection = glm::perspective(glm::radians(m_fov),
			(float)m_screen_width / (float)m_screen_height, 0.1f, 1000.0f);

		//---------------------------------------- Post Processing Framebuffers
		// Gaussian blur
		m_horizontal_gb_framebuffer.initialize(FB_COLOR_BUFFER,
			m_screen_width / 2, m_screen_height / 2);
		m_horizontal_gb_framebuffer.setProportion(2);
		m_horizontal_gb_framebuffer2.initialize(FB_COLOR_BUFFER,
			m_screen_width / 8, m_screen_height / 8);
		m_horizontal_gb_framebuffer2.setProportion(8);
		m_vertical_gb_framebuffer.initialize(FB_COLOR_BUFFER,
			m_screen_width / 2, m_screen_height / 2);
		m_vertical_gb_framebuffer.setProportion(2);
		m_vertical_gb_framebuffer2.initialize(FB_COLOR_BUFFER,
			m_screen_width / 8, m_screen_height / 8);
		m_vertical_gb_framebuffer2.setProportion(8);
		//--------------

		// Bloom
		m_bright_framebuffer.initialize(FB_COLOR_BUFFER,
			m_screen_width / 2, m_screen_height / 2);
		m_bright_framebuffer.setProportion(2);
		m_combine_framebuffer.initialize(FB_COLOR_BUFFER,
			m_screen_width, m_screen_height);
		//------

		m_pp_framebuffer.initialize(FB_COLOR_BUFFER,
			m_screen_width, m_screen_height);
		//---------------------------------------------------------------------

		glViewport(view_port[0], view_port[1], view_port[2], view_port[3]);
		glClearColor(color.r, color.g, color.b, color.a);

		m_active_camera = active_camera;

		return true;
	}

	bool GraphicsManager::initializeFromConfigFile(const std::string& path) {
		LuaScript lua_context;
		lua_context.initialize(path);

		// TODO: allocation hints for all containers
		int n_shaders = lua_context.get<int>("n_shaders");

		m_shaders.reserve(n_shaders);

		bool depth_test = lua_context.get<bool>("depth_test");
		bool face_culling = lua_context.get<bool>("face_culling");
		bool blending = lua_context.get<bool>("blending");
		auto color = lua_context.getFloatVector("clear_color");
		auto view_port = lua_context.getIntVector("view_port");
		float fov = lua_context.get<float>("fov");
		int active_camera = lua_context.get<int>("active_camera");

		//---------------------------------------------- Post Processing Shader
		auto vs = lua_context.get<std::string>("m_horizontal_gb_shadervs");
		auto fs = lua_context.get<std::string>("m_horizontal_gb_shaderfs");
		m_horizontal_gb_shader = addShader(vs, fs);

		vs = lua_context.get<std::string>("m_vertical_gb_shadervs");
		fs = lua_context.get<std::string>("m_vertical_gb_shaderfs");
		m_vertical_gb_shader = addShader(vs, fs);

		vs = lua_context.get<std::string>("m_bright_shadervs");
		fs = lua_context.get<std::string>("m_bright_shaderfs");
		m_bright_shader = addShader(vs, fs);

		vs = lua_context.get<std::string>("m_combine_shadervs");
		fs = lua_context.get<std::string>("m_combine_shaderfs");
		m_combine_shader = addShader(vs, fs);

		vs = lua_context.get<std::string>("m_pp_shadervs");
		fs = lua_context.get<std::string>("m_pp_shaderfs");
		m_pp_shader = addShader(vs, fs);
		//---------------------------------------------------------------------

		lua_context.destroy();

		return initialize(depth_test, face_culling, blending,
			&view_port[0], glm::vec4(color[0], color[1], color[2], color[3]),
			fov, active_camera);
	}

	void GraphicsManager::update(float delta_time) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		checkOpenGLErrors("Entering GraphicsManager::update");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

		// Passing delta time to cameras
		for (auto& it : m_cameras)
			it->m_delta_time = delta_time;

		renderDepthMaps();

		m_pp_framebuffer.bind();
		glViewport(0, 0, m_pp_framebuffer.getWidth(),
			m_pp_framebuffer.getHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderScene();
		renderSkybox();

		EventPtr e =
			std::make_shared<PrePostProcessEvent>(PrePostProcessEvent());
		EventManager::getInstance().sendEvent(e);

		postProcess();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		checkOpenGLErrors("Exiting GraphicsManager::update");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	}

	void GraphicsManager::renderDepthMaps() {
		glCullFace(GL_FRONT);

		for (auto& it : m_directional_lights) {
			if (it.emit_shadows) {
				it.depth_map.bind();

				glViewport(0, 0, it.dmw, it.dmh);
				glClear(GL_DEPTH_BUFFER_BIT);

				m_shaders[it.depth_shader].bind();

				m_shaders[it.depth_shader].setMat4("u_light_space_matrix",
					it.projection * it.view);

				for (auto& entity : g_entities) {
					if (entity.emit_shadows) {
						for (unsigned i = 0; i < entity.meshes.size(); i++) {
							m_shaders[it.depth_shader].setMat4(
								"u_model_matrix",
								entity.transforms[i]);

							m_shaders[it.depth_shader].update();

							m_meshes[entity.meshes[i]].first.draw(
								GL_TRIANGLES);
						}
					}
				}
			}
		}

		for (auto& it : m_point_lights) {
			if (it.emit_shadows) {
				it.depth_map.bind();

				glViewport(0, 0, it.dmw, it.dmh);
				glClear(GL_DEPTH_BUFFER_BIT);

				m_shaders[it.depth_shader].bind();

				for (int i = 0; i < 6; i++)
					m_shaders[it.depth_shader].setMat4("u_shadow_matrices[" +
						std::to_string(i) + "]", it.projection * it.view[i]);

				m_shaders[it.depth_shader].setVec3(
					"u_light_pos", it.position);

				m_shaders[it.depth_shader].setFloat(
					"u_far_plane", it.far_plane);

				for (auto& entity : g_entities) {
					if (entity.emit_shadows) {
						for (unsigned i = 0; i < entity.meshes.size(); i++) {
							m_shaders[it.depth_shader].setMat4(
								"u_model_matrix",
								entity.transforms[i]);

							m_shaders[it.depth_shader].update();

							m_meshes[entity.meshes[i]].first.draw(
								GL_TRIANGLES);
						}
					}
				}
			}
		}

		glCullFace(GL_BACK);
	}

	void GraphicsManager::renderScene() {
		for (auto& it : g_entities) {
			m_shaders[it.shader].bind();

			m_shaders[it.shader].setMat4("u_projection_matrix",
				m_projection);

			m_shaders[it.shader].setMat4("u_view_matrix",
				m_cameras[m_active_camera]->getViewMatrix());

			m_shaders[it.shader].setVec3("u_view_pos",
				m_cameras[m_active_camera]->getPosition());

			bindLights(m_shaders[it.shader]);
			bindFog(m_shaders[it.shader]);

			for (unsigned i = 0; i < it.meshes.size(); i++) {
				bind2DTextures(m_shaders[it.shader],
					m_meshes[it.meshes[i]].first.m_material_id);

				m_shaders[it.shader].setMat4("u_model_matrix",
					it.transforms[i]);

				m_shaders[it.shader].setMat3("u_trn_inv_up_model",
					glm::transpose(glm::inverse(glm::mat3(it.transforms[i]))));

				m_shaders[it.shader].update();

				m_meshes[it.meshes[i]].first.draw(GL_TRIANGLES);
			}
		}
	}

	void GraphicsManager::renderSkybox() {
		if (m_draw_skybox) {
			int depth_func;
			glGetIntegerv(GL_DEPTH_FUNC, &depth_func);

			glDisable(GL_CULL_FACE);
			glDepthFunc(GL_LEQUAL);
			unsigned shader_id = m_skybox.getShaderId();

			m_shaders[shader_id].bind();
			glm::mat4 view = m_cameras[m_active_camera]->getViewMatrix();

			view[3][0] = 0.0f;
			view[3][1] = 0.0f;
			view[3][2] = 0.0f;

			m_shaders[shader_id].setMat4("u_projection_matrix", m_projection);
			m_shaders[shader_id].setMat4("u_view_matrix", view);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, MaterialManager::getInstance().
				getCubeTexture(m_skybox.getTextureId()));

			m_shaders[shader_id].setInt("u_cube_map", 0);
			m_shaders[shader_id].setBool("u_fog", m_fog);

			if (m_fog) {
				m_shaders[shader_id].setVec3("u_fog_color",
					m_skybox.getFogColor());
				m_shaders[shader_id].setFloat("u_lower_limit",
					m_sb_lower_limit);
				m_shaders[shader_id].setFloat("u_upper_limit",
					m_sb_upper_limit);
			}

			m_shaders[shader_id].update();

			m_skybox.draw(GL_TRIANGLES);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			glDepthFunc(depth_func);
			glEnable(GL_CULL_FACE);
		}
	}

	void GraphicsManager::postProcess() {
		bool depth_test = false;

		if (glIsEnabled(GL_DEPTH_TEST))
			depth_test = true;

		glDisable(GL_DEPTH_TEST);

		unsigned texture_id = m_pp_framebuffer.getTextureId();

		if (m_bloom_level > 0.0f) {
			m_bright_framebuffer.bind();

			glViewport(0, 0, m_bright_framebuffer.getWidth(),
				m_bright_framebuffer.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			m_shaders[m_bright_shader].bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			m_shaders[m_bright_shader].setInt("u_texture", 0);
			m_shaders[m_bright_shader].update();

			drawQuad(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f));

			texture_id = m_bright_framebuffer.getTextureId();
		}

		if (m_blur_level > 0) {
			// Horizontal blur
			m_horizontal_gb_framebuffer.bind();

			glViewport(0, 0, m_horizontal_gb_framebuffer.getWidth(),
				m_horizontal_gb_framebuffer.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			m_shaders[m_horizontal_gb_shader].bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			m_shaders[m_horizontal_gb_shader].setInt("u_texture", 0);
			m_shaders[m_horizontal_gb_shader].setFloat("u_target_width",
				m_horizontal_gb_framebuffer.getWidth());
			m_shaders[m_horizontal_gb_shader].update();

			drawQuad(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f));
			//----------------

			m_vertical_gb_framebuffer.bind();

			glViewport(0, 0, m_horizontal_gb_framebuffer.getWidth(),
				m_horizontal_gb_framebuffer.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			// Vertical blur
			m_shaders[m_vertical_gb_shader].bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,
				m_horizontal_gb_framebuffer.getTextureId());
			m_shaders[m_vertical_gb_shader].setInt("u_texture", 0);
			m_shaders[m_vertical_gb_shader].setFloat("u_target_height",
				m_vertical_gb_framebuffer.getHeight());
			m_shaders[m_vertical_gb_shader].update();
			drawQuad(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f));
			//----------------

			texture_id = m_vertical_gb_framebuffer.getTextureId();
		}
		
		if (m_blur_level > 1) {
			// Horizontal blur
			m_horizontal_gb_framebuffer2.bind();

			glViewport(0, 0, m_horizontal_gb_framebuffer2.getWidth(),
				m_horizontal_gb_framebuffer2.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			m_shaders[m_horizontal_gb_shader].bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			m_shaders[m_horizontal_gb_shader].setInt("u_texture", 0);
			m_shaders[m_horizontal_gb_shader].setFloat("u_target_width",
				m_horizontal_gb_framebuffer2.getWidth());
			m_shaders[m_horizontal_gb_shader].update();

			drawQuad(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f));
			//----------------

			m_vertical_gb_framebuffer2.bind();

			glViewport(0, 0, m_horizontal_gb_framebuffer2.getWidth(),
				m_horizontal_gb_framebuffer2.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			// Vertical blur
			m_shaders[m_vertical_gb_shader].bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,
				m_horizontal_gb_framebuffer2.getTextureId());
			m_shaders[m_vertical_gb_shader].setInt("u_texture", 0);
			m_shaders[m_vertical_gb_shader].setFloat("u_target_height",
				m_vertical_gb_framebuffer2.getHeight());
			m_shaders[m_vertical_gb_shader].update();
			drawQuad(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f));
			//----------------

			texture_id = m_vertical_gb_framebuffer2.getTextureId();
		}
		
		if (m_bloom_level > 0.0f) {
			m_combine_framebuffer.bind();

			glViewport(0, 0, m_combine_framebuffer.getWidth(),
				m_combine_framebuffer.getHeight());
			glClear(GL_COLOR_BUFFER_BIT);

			m_shaders[m_combine_shader].bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_pp_framebuffer.getTextureId());
			m_shaders[m_combine_shader].setInt("u_color_texture", 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			m_shaders[m_combine_shader].setInt("u_highlight_texture", 1);

			m_shaders[m_combine_shader].setFloat("u_glow_factor", m_bloom_level);

			m_shaders[m_combine_shader].update();

			drawQuad(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f));

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);

			texture_id = m_combine_framebuffer.getTextureId();
		}

		Framebuffer::defaultFramebuffer();

		glViewport(0, 0, m_screen_width, m_screen_height);
		glClear(GL_COLOR_BUFFER_BIT);

		// Per pixel effects
		m_shaders[m_pp_shader].bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		m_shaders[m_pp_shader].setInt("u_texture", 0);
		m_shaders[m_pp_shader].setFloat("u_contrast", m_contrast_factor);
		m_shaders[m_pp_shader].update();
		drawQuad(glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		//----------------

		if (depth_test)
			glEnable(GL_DEPTH_TEST);
	}

	void GraphicsManager::bindLights(Shader& shader) {
		if (shader.hasDirLights()) {
			shader.setInt("u_nr_dir_lights", (int)m_directional_lights.size());

			for (unsigned i = 0; i < m_directional_lights.size(); i++) {
				shader.setVec3("u_dir_lights[" + std::to_string(i) +
					"].direction", m_directional_lights[i].direction);

				shader.setFloat("u_dir_lights[" + std::to_string(i) +
					"].shininess", m_directional_lights[i].shininess);

				shader.setVec3("u_dir_lights[" + std::to_string(i) +
					"].ambient", m_directional_lights[i].ambient);
				shader.setVec3("u_dir_lights[" + std::to_string(i) +
					"].diffuse", m_directional_lights[i].diffuse);
				shader.setVec3("u_dir_lights[" + std::to_string(i) +
					"].specular", m_directional_lights[i].specular);

				if (m_directional_lights[i].emit_shadows) {
					shader.setMat4("u_dir_light_space_matrix[" +
						std::to_string(i) + "]",
						m_directional_lights[i].projection *
						m_directional_lights[i].view);

					glActiveTexture(GL_TEXTURE8 + i);
					glBindTexture(GL_TEXTURE_2D,
						m_directional_lights[i].depth_map.getTextureId());
					shader.setInt("u_dir_shadow_map[" + std::to_string(i) +
						"]", 8 + i);
				}
			}
		}

		if (shader.hasPointLights()) {
			shader.setInt("u_nr_point_lights", (int)m_point_lights.size());

			for (unsigned i = 0; i < m_point_lights.size(); i++) {
				shader.setVec3("u_point_lights[" + std::to_string(i) +
					"].position", m_point_lights[i].position);

				shader.setFloat("u_point_lights[" + std::to_string(i) +
					"].shininess", m_point_lights[i].shininess);

				shader.setFloat("u_point_lights[" + std::to_string(i) +
					"].constant", m_point_lights[i].constant);
				shader.setFloat("u_point_lights[" + std::to_string(i) +
					"].linear", m_point_lights[i].linear);
				shader.setFloat("u_point_lights[" + std::to_string(i) +
					"].quadratic", m_point_lights[i].quadratic);

				shader.setVec3("u_point_lights[" + std::to_string(i) +
					"].ambient", m_point_lights[i].ambient);
				shader.setVec3("u_point_lights[" + std::to_string(i) +
					"].diffuse", m_point_lights[i].diffuse);
				shader.setVec3("u_point_lights[" + std::to_string(i) +
					"].specular", m_point_lights[i].specular);

				if (m_point_lights[i].emit_shadows) {
					shader.setFloat("u_far_plane",
						m_point_lights[i].far_plane);

					glActiveTexture(GL_TEXTURE16 + i);
					glBindTexture(GL_TEXTURE_CUBE_MAP,
						m_point_lights[i].depth_map.getTextureId());
					shader.setInt("u_point_shadow_map[" + std::to_string(i) +
						"]", 16 + i);
				}
			}
		}

		if (shader.hasSpotLights()) {
			shader.setInt("u_nr_spot_lights", (int)m_spot_lights.size());

			for (unsigned i = 0; i < m_spot_lights.size(); i++) {
				shader.setVec3("u_spot_lights[" + std::to_string(i) +
					"].position", m_spot_lights[i].position);
				shader.setVec3("u_spot_lights[" + std::to_string(i) +
					"].direction", m_spot_lights[i].direction);

				shader.setFloat("u_spot_lights[" + std::to_string(i) +
					"].shininess", m_spot_lights[i].shininess);

				shader.setFloat("u_spot_lights[" + std::to_string(i) +
					"].inner_cut_off", m_spot_lights[i].inner_cut_off);
				shader.setFloat("u_spot_lights[" + std::to_string(i) +
					"].outer_cut_off", m_spot_lights[i].outer_cut_off);

				shader.setFloat("u_spot_lights[" + std::to_string(i) +
					"].constant", m_spot_lights[i].constant);
				shader.setFloat("u_spot_lights[" + std::to_string(i) +
					"].linear", m_spot_lights[i].linear);
				shader.setFloat("u_spot_lights[" + std::to_string(i) +
					"].quadratic", m_spot_lights[i].quadratic);

				shader.setVec3("u_spot_lights[" + std::to_string(i) +
					"].ambient", m_spot_lights[i].ambient);
				shader.setVec3("u_spot_lights[" + std::to_string(i) +
					"].diffuse", m_spot_lights[i].diffuse);
				shader.setVec3("u_spot_lights[" + std::to_string(i) +
					"].specular", m_spot_lights[i].specular);
			}
		}
	}
	
	void GraphicsManager::bindFog(Shader& shader) {
		if (shader.hasFog()) {
			shader.setBool("u_fog", m_fog);
			shader.setFloat("u_fog_density", m_fog_density);
			shader.setFloat("u_gradient", m_fog_gradient);
			shader.setVec3("u_sky_color", m_sky_color);
		}
	}

	void GraphicsManager::bind2DTextures(Shader& shader, unsigned material_id) {
		for (unsigned i = 0; i < NUMBER_OF_TEXTURE_TYPES; i++) {
			if (i == 3 || i == 6 || i == 7) // TODO
				continue;

			if ((i == 4 || i == 5) && !shader.hasNormalMap())
				continue;

			unsigned texture = m_materials[material_id].textures[i];

			glActiveTexture(GL_TEXTURE0 + i);

			if (texture < INT_MAX) {
				glBindTexture(GL_TEXTURE_2D,
					MaterialManager::getInstance().get2DTexture(texture));
				shader.setInt(m_texture_names[i], i);
			}
			else
				glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void GraphicsManager::destroy() {
		// TODO

		//------------------------------------------------------------- Helpers
		if (glIsBuffer(m_line_vbo))
			glDeleteBuffers(1, &m_line_vbo);

		if (glIsVertexArray(m_line_vao))
			glDeleteVertexArrays(1, &m_line_vao);

		if (glIsBuffer(m_quad_vbo))
			glDeleteBuffers(1, &m_quad_vbo);

		if (glIsVertexArray(m_quad_vao))
			glDeleteVertexArrays(1, &m_quad_vao);
		//---------------------------------------------------------------------

		for (auto& it : m_meshes)
			it.first.destroy();

		for (auto& it : m_shaders)
			it.destroy();

		for (auto& it : m_directional_lights)
			it.depth_map.destroy();

		for (auto& it : m_point_lights)
			it.depth_map.destroy();

		for (auto& it : m_spot_lights)
			it.depth_map.destroy();

		m_horizontal_gb_framebuffer.destroy();
		m_horizontal_gb_framebuffer2.destroy();
		m_vertical_gb_framebuffer.destroy();
		m_vertical_gb_framebuffer2.destroy();

		m_bright_framebuffer.destroy();
		m_combine_framebuffer.destroy();

		m_pp_framebuffer.destroy();

		m_skybox.destroy();
	}

	void GraphicsManager::reserveMeshes(int size) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager allocating " + std::to_string(size) +
			" more meshes");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

		m_meshes.reserve(m_meshes.size() + size);
	}

	void GraphicsManager::reserveMaterials(int size) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager allocating " + std::to_string(size) +
			" more materials");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

		m_materials.reserve(m_materials.size() + size);
	}

	void GraphicsManager::reserveAnimations(int size) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager allocating " + std::to_string(size) +
			" more animations");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

		m_animations.reserve(m_animations.size() + size);
	}

	unsigned GraphicsManager::numberOfMeshes() {
		return (unsigned)m_meshes.size() - 1;
	}

	//----------------------------------------------------------- Add functions
	unsigned GraphicsManager::addCamera(Camera* camera) {
		m_cameras.push_back(camera);
		return (unsigned)m_cameras.size() - 1;
	}

	unsigned GraphicsManager::addShader(const std::string& vs_path,
		const std::string& fs_path) {
		// Shader must be initialized after push_back
		// to keep the instance reference
		m_shaders.push_back(Shader());
		m_shaders[m_shaders.size() - 1].initialize(vs_path, fs_path);
		return (unsigned)m_shaders.size() - 1;
	}

	unsigned GraphicsManager::addShader(const std::string& vs_path,
		const std::string& gs_path, const std::string& fs_path) {
		// Shader must be initialized after push_back
		// to keep the instance reference
		m_shaders.push_back(Shader());
		m_shaders[m_shaders.size() - 1].initialize(
			vs_path, gs_path, fs_path);
		return (unsigned)m_shaders.size() - 1;
	}

	unsigned GraphicsManager::addMesh(const Mesh& mesh) {
		auto it = m_mesh_name_to_handle.find(mesh.m_name);

		// If the mesh isn't in the vector
		if (it == this->m_mesh_name_to_handle.end()) {
			unsigned handle;

			// If there aren't spaces available from previous removes
			if (m_meshes_unused_spaces.empty()) {
				m_meshes.push_back(std::move(std::make_pair(mesh, 1)));
				m_mesh_name_to_handle[mesh.m_name] = (unsigned)m_meshes.size() - 1;
				handle = (unsigned)m_meshes.size() - 1;
			}
			else {
				handle = m_meshes_unused_spaces.top();
				m_meshes_unused_spaces.pop();

				m_meshes[handle] = std::make_pair(mesh, 1);
				m_mesh_name_to_handle[mesh.m_name] = handle;
			}

			return handle;
		}

		// Increases mesh reference
		m_meshes[it->second].second++;
		return it->second;
	}

	unsigned GraphicsManager::addAnimation(const Animation& animation) {
		m_animations.push_back(animation);
		return (unsigned)m_animations.size() - 1;
	}

	unsigned GraphicsManager::addMaterial(const Material& material) {
		m_materials.push_back(material);
		return (unsigned)m_materials.size() - 1;
	}

	unsigned GraphicsManager::addDirectionalLight(
		const DirectionalLight& light) {
		m_directional_lights.push_back(light);

		m_directional_lights[m_directional_lights.size() - 1].
			depth_map.initialize(FB_DEPTH_MAP, light.dmw, light.dmh);

		return (unsigned)m_directional_lights.size() - 1;
	}

	unsigned GraphicsManager::addPointLight(
		const PointLight& light) {
		m_point_lights.push_back(light);

		m_point_lights[m_point_lights.size() - 1].
			depth_map.initialize(FB_DEPTH_CUBE_MAP, light.dmw, light.dmh);

		return (unsigned)m_point_lights.size() - 1;
	}

	unsigned GraphicsManager::addSpotLight(
		const SpotLight& light) {
		m_spot_lights.push_back(light);
		return (unsigned)m_spot_lights.size() - 1;
	}
	//-------------------------------------------------------------------------

	//-------------------------------------------------------- Remove functions
	void GraphicsManager::removeCamera(unsigned handle) {
		if (handle >= m_cameras.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Mesh outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		m_cameras.erase(m_cameras.begin() + handle);
	}

	void GraphicsManager::removeShader(unsigned handle) {
		if (handle >= m_shaders.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Mesh outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		m_shaders[handle].destroy();
		m_shaders.erase(m_shaders.begin() + handle);
	}

	void GraphicsManager::removeMesh(unsigned handle) {
		if (handle >= m_meshes.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Mesh outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		if (m_meshes[handle].second > 0)
			m_meshes[handle].second--;
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
		else
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Mesh with 0 references");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING

		// If no one references the mesh anymore
		if (m_meshes[handle].second == 0) {
			m_meshes[handle].first.destroy();

			// Remove reference from map
			for (auto it = m_mesh_name_to_handle.begin();
				it != m_mesh_name_to_handle.end(); ++it) {
				if (it->second == handle) {
					m_mesh_name_to_handle.erase(it);
					break;
				}
			}

			m_meshes_unused_spaces.push(handle);
		}
	}

	void GraphicsManager::removeAnimation(unsigned handle) {
		if (handle >= m_animations.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Animation outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		m_animations.erase(m_animations.begin() + handle);
	}

	void GraphicsManager::removeMaterial(unsigned handle) {
		if (handle >= m_materials.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Material outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		m_materials.erase(m_materials.begin() + handle);
	}

	void GraphicsManager::removeDirectionalLight(unsigned handle) {
		if (handle >= m_directional_lights.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Mesh outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		m_directional_lights.erase(m_directional_lights.begin() + handle);
	}

	void GraphicsManager::removePointLight(unsigned handle) {
		if (handle >= m_point_lights.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Mesh outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		m_point_lights.erase(m_point_lights.begin() + handle);
	}

	void GraphicsManager::removeSpotLight(unsigned handle) {
		if (handle >= m_spot_lights.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove Mesh outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		m_spot_lights.erase(m_spot_lights.begin() + handle);
	}

	void GraphicsManager::onWindowResizeEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<WindowResizeEvent>(e);

		int w, h;
		evnt->getSize(w, h);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Window size: " + std::to_string(w) + " " + std::to_string(h));
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

		m_screen_width = w;
		m_screen_height = h;

		m_projection = glm::perspective(glm::radians(m_fov),
			(float)w / (float)h, 0.1f, 1000.0f);

		glViewport(0, 0, w, h);
	}
	//-------------------------------------------------------------------------

	//----------------------------------------------------------------- Helpers
	void GraphicsManager::setProjectionMatrix(const glm::mat4& matrix) {
		m_projection = matrix;
	}

	glm::mat4 GraphicsManager::getProjectionMatrix() {
		return m_projection;
	}

	void GraphicsManager::setActiveCamera(int id) {
		m_active_camera = id;
	}

	Camera* GraphicsManager::getActiveCamera() {
		if (m_active_camera < 0 || m_active_camera >= (int)m_cameras.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Trying to get inexisting active camera");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return m_cameras[m_active_camera];
	}

	Shader* GraphicsManager::getShader(int id) {
		if (id >= m_shaders.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to get shader outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return &m_shaders[id];
	}

	DirectionalLight* GraphicsManager::getDirectionalLight(int id) {
		if (id >= m_directional_lights.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to get dir light outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return &m_directional_lights[id];
	}

	PointLight* GraphicsManager::getPointLight(int id) {
		if (id >= m_point_lights.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to get point light outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return &m_point_lights[id];
	}

	SpotLight* GraphicsManager::getSpotLight(int id) {
		if (id >= m_spot_lights.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to get spot light outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return &m_spot_lights[id];
	}

	void GraphicsManager::getScreenSize(int& w, int&h) {
		w = m_screen_width;
		h = m_screen_height;
	}
	//-------------------------------------------------------------------------

	//--------------------------------------------------------- Drawing helpers
	void GraphicsManager::drawLine(const glm::vec3& from,
		const glm::vec3& to, const glm::vec3& color) {
		if (m_line_vao == 0)
			glGenVertexArrays(1, &m_line_vao);

		Shader* shader_ptr =
			GraphicsManager::getInstance().getShader(m_line_shader);

		shader_ptr->bind();
		shader_ptr->setMat4("u_projection_matrix",
			GraphicsManager::getInstance().getProjectionMatrix());
		shader_ptr->setMat4("u_view_matrix",
			GraphicsManager::getInstance().getActiveCamera()->getViewMatrix());
		shader_ptr->setVec3("u_color", color);
		shader_ptr->update();

		glBindVertexArray(m_line_vao);

		float vertices[] = {
			from.x, from.y, from.z,
			to.x, to.y, to.z
		};

		glGenBuffers(1, &m_line_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_LINES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glDeleteBuffers(1, &m_line_vbo);
	}

	void GraphicsManager::drawQuad(const glm::vec4& limits) {
		if (m_quad_vao == 0) {
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

		float vertices[6][4] = {
			{ limits.x,            limits.y + limits.w,   0.0f, 0.0f },
		{ limits.x,            limits.y,              0.0f, 1.0f },
		{ limits.x + limits.z, limits.y,              1.0f, 1.0f },
		{ limits.x,            limits.y + limits.w,   0.0f, 0.0f },
		{ limits.x + limits.z, limits.y,              1.0f, 1.0f },
		{ limits.x + limits.z, limits.y + limits.w,   1.0f, 0.0f }
		};

		glBindVertexArray(m_quad_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_quad_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}
	//-------------------------------------------------------------------------

	//------------------------------------------------------------------ Skybox
	void GraphicsManager::setSkybox(const std::string& texture_folder,
		const std::string& extension, const std::string& vs_path,
		const std::string& fs_path) {
		m_skybox.destroy();

		std::string aux = texture_folder;

		if (texture_folder.back() != '/')
			aux += "/";

		std::string texture[6];
		texture[0] = aux + "right." + extension;
		texture[1] = aux + "left." + extension;
		texture[2] = aux + "top." + extension;
		texture[3] = aux + "bottom." + extension;
		texture[4] = aux + "front." + extension;
		texture[5] = aux + "back." + extension;

		m_skybox.initialize(
			GraphicsManager::getInstance().addShader(vs_path, fs_path),
			MaterialManager::getInstance().addCubeTexture(texture)
		);

		m_draw_skybox = true;
	}

	void GraphicsManager::allowSkyboxDraw(bool value) {
		m_draw_skybox = value;
	}

	Skybox* GraphicsManager::getSkybox() {
		return &m_skybox;
	}

	void GraphicsManager::removeSkybox() {
		m_skybox.destroy();
		m_draw_skybox = false;
	}
	//-------------------------------------------------------------------------

	//--------------------------------------------------------------------- Fog
	void GraphicsManager::setFog(float density, float gradient,
		const glm::vec3 sky_color, float sb_lower_limit,
		float sb_upper_limit, const glm::vec3& fog_color) {
		m_fog = true;
		m_fog_density = density;
		m_fog_gradient = gradient;
		m_sky_color = sky_color;
		m_sb_lower_limit = sb_lower_limit;
		m_sb_upper_limit = sb_upper_limit;
		m_skybox.setFogColor(fog_color);
	}

	void GraphicsManager::removeFog() {
		m_fog = false;
	}
	//-------------------------------------------------------------------------

	//--------------------------------------------------------- Post Processing
	void GraphicsManager::setGaussianBlurLevel(int level) {
		m_blur_level = std::min(std::max(level, 0), 2);
	}

	int GraphicsManager::getGaussianBlurLevel() const {
		return m_blur_level;
	}

	void GraphicsManager::setBloomLevel(float level) {
		m_bloom_level = level;
	}

	float GraphicsManager::getBloomLevel() const {
		return m_bloom_level;
	}

	void GraphicsManager::setContrastFactor(float factor) {
		m_contrast_factor = factor;
	}

	float GraphicsManager::getContrastFactor() const {
		return m_contrast_factor;
	}
	//-------------------------------------------------------------------------
}