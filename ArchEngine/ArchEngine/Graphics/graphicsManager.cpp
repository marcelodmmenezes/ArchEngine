/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 26/05/2018                                                 *
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
	GraphicsManager::GraphicsManager() : m_state(CONSTRUCTED) {
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
		int view_port[4], const glm::vec4& color, int active_camera) {
		if (depth_test)
			glEnable(GL_DEPTH_TEST);
		if (face_culling)
			glEnable(GL_CULL_FACE);

		//------------------------------------------------------ Event handlers
		m_window_size_listener.bind
			<GraphicsManager, &GraphicsManager::onWindowResizeEvent>(this);
		EventManager::getInstance().addListener(
			m_window_size_listener, EVENT_WINDOW_RESIZE);
		//---------------------------------------------------------------------

		glViewport(view_port[0], view_port[1], view_port[2], view_port[3]);
		glClearColor(color.r, color.g, color.b, color.a);

		m_active_camera = active_camera;

		return true;
	}

	bool GraphicsManager::initializeFromConfigFile(const std::string& path) {
		LuaScript lua_context;
		lua_context.initialize(path);

		bool depth_test = lua_context.get<bool>("depth_test");
		bool face_culling = lua_context.get<bool>("face_culling");
		auto color = lua_context.getFloatVector("clear_color");
		auto view_port = lua_context.getIntVector("view_port");
		int active_camera = lua_context.get<int>("active_camera");

		lua_context.destroy();

		return initialize(depth_test, face_culling, &view_port[0],
			glm::vec4(color[0], color[1], color[2], color[3]), active_camera);
	}

	void GraphicsManager::update(float delta_time) {
		// TODO

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		checkOpenGLErrors("Entering GraphicsManager::update");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Passing delta time to cameras
		for (auto& it : m_cameras)
			it.m_delta_time = delta_time;

		// Drawing scene
		for (auto& it : m_shaders) {
			it.bind();

			it.setMat4("u_projection_matrix", m_projection);
			it.setMat4("u_view_matrix",
				m_cameras[m_active_camera].getViewMatrix());
			it.setVec3("u_view_pos",
				m_cameras[m_active_camera].getPosition());

			bindLights(it);
			drawMeshes(it, true);
		}

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
		checkOpenGLErrors("Exiting GraphicsManager::update");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
	}

	void GraphicsManager::bindLights(Shader& shader) {
		shader.setInt("u_nr_dir_lights", m_directional_lights.size());

		for (unsigned i = 0; i < m_directional_lights.size(); i++) {
			shader.setVec3("u_dir_lights[" + std::to_string(i) +
				"].direction", m_directional_lights[i].direction);

			shader.setFloat("u_dir_lights[" + std::to_string(i) +
				"].shineness", m_directional_lights[i].shineness);

			shader.setVec3("u_dir_lights[" + std::to_string(i) +
				"].ambient", m_directional_lights[i].ambient);
			shader.setVec3("u_dir_lights[" + std::to_string(i) +
				"].diffuse", m_directional_lights[i].diffuse);
			shader.setVec3("u_dir_lights[" + std::to_string(i) +
				"].specular", m_directional_lights[i].specular);
		}

		shader.setInt("u_nr_point_lights", m_point_lights.size());

		for (unsigned i = 0; i < m_point_lights.size(); i++) {
			shader.setVec3("u_point_lights[" + std::to_string(i) +
				"].position", m_point_lights[i].position);

			shader.setFloat("u_point_lights[" + std::to_string(i) +
				"].shineness", m_point_lights[i].shineness);

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
		}

		shader.setInt("u_nr_spot_lights", m_spot_lights.size());

		for (unsigned i = 0; i < m_spot_lights.size(); i++) {
			shader.setVec3("u_spot_lights[" + std::to_string(i) +
				"].position", m_spot_lights[i].position);
			shader.setVec3("u_spot_lights[" + std::to_string(i) +
				"].direction", m_spot_lights[i].direction);

			shader.setFloat("u_spot_lights[" + std::to_string(i) +
				"].shineness", m_spot_lights[i].shineness);

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

	void GraphicsManager::drawMeshes(Shader& shader, bool draw_textures) {
		glm::mat4 model_matrix;

		for (unsigned i = 0; i < m_meshes.size(); i++) {
			// If current mesh has references
			if (m_meshes[i].second > 0) {
				if (draw_textures)
					bind2DTextures(shader, i);

				// TODO -> get real matrix
				model_matrix = glm::scale(glm::mat4(1.0f),
					glm::vec3(0.1f, 0.1f, 0.1f));

				shader.setMat4("u_model_matrix", model_matrix);
				shader.update();
				m_meshes[i].first.draw();
			}
		}
	}

	void GraphicsManager::bind2DTextures(Shader& shader, unsigned mesh_id) {
		for (unsigned i = 0; i < NUMBER_OF_TEXTURE_TYPES; i++) {
			if (i != 0 && i != 3)
				continue;

			unsigned texture =
				m_materials[m_meshes[mesh_id].first.m_material_id].textures[i];

			if (texture < UINT_MAX) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D,
					MaterialManager::getInstance().getTexture(texture));
				shader.setInt(m_texture_names[i], i);
			}
		}
	}

	void GraphicsManager::unbind2DTextures(Shader& shader) {
		for (unsigned i = 0; i < NUMBER_OF_TEXTURE_TYPES; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void GraphicsManager::destroy() {
		// TODO

		for (auto& it : m_directional_lights)
			it.depth_map.destroy();

		for (auto& it : m_point_lights)
			it.depth_map.destroy();

		for (auto& it : m_spot_lights)
			it.depth_map.destroy();
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

	//----------------------------------------------------------- Add functions
	unsigned GraphicsManager::addCamera(const DebugCamera& camera) {
		m_cameras.push_back(camera);
		return m_cameras.size() - 1;
	}

	unsigned GraphicsManager::addShader(const std::string& vs_path,
		const std::string& fs_path) {
		// Shader must be initialized after push_back
		// to keep the instance reference
		Shader shader;
		m_shaders.push_back(shader);
		m_shaders[m_shaders.size() - 1].initialize(vs_path, fs_path);
		return m_shaders.size() - 1;
	}

	unsigned GraphicsManager::addShader(const std::string& vs_path,
		const std::string& gs_path, const std::string& fs_path) {
		// Shader must be initialized after push_back
		// to keep the instance reference
		Shader shader;
		m_shaders.push_back(shader);
		m_shaders[m_shaders.size() - 1].initialize(vs_path, gs_path, fs_path);
		return m_shaders.size() - 1;
	}

	unsigned GraphicsManager::addMesh(const Mesh& mesh) {
		auto it = m_mesh_name_to_handle.find(mesh.m_name);

		// If the mesh isn't in the vector
		if (it == this->m_mesh_name_to_handle.end()) {
			unsigned handle;

			// If there aren't spaces available from previous removes
			if (m_meshes_unused_spaces.empty()) {
				m_meshes.push_back(std::move(std::make_pair(mesh, 1)));
				m_mesh_name_to_handle[mesh.m_name] = m_meshes.size() - 1;
				handle = m_meshes.size() - 1;
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

	unsigned GraphicsManager::addMaterial(const Material& material) {
		m_materials.push_back(material);
		return m_materials.size() - 1;
	}

	unsigned GraphicsManager::addDirectionalLight(
		const DirectionalLight& light) {
		m_directional_lights.push_back(light);
		return m_directional_lights.size() - 1;
	}

	unsigned GraphicsManager::addPointLight(
		const PointLight& light) {
		m_point_lights.push_back(light);
		return m_point_lights.size() - 1;
	}

	unsigned GraphicsManager::addSpotLight(
		const SpotLight& light) {
		m_spot_lights.push_back(light);
		return m_spot_lights.size() - 1;
	}

	//----------------------------------------------------------------- Setters
	void GraphicsManager::setProjectionMatrix(const glm::mat4& matrix) {
		m_projection = matrix;
	}

	void GraphicsManager::setActiveCamera(int id) {
		m_active_camera = id;
	}

	//----------------------------------------------------------------- Getters
	DebugCamera* GraphicsManager::getActiveCamera() {
		if (m_active_camera < 0 || m_active_camera >= (int)m_cameras.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Trying to get inexisting active camera");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return nullptr;
		}

		return &m_cameras[m_active_camera];
	}

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

		glViewport(0, 0, w, h);
	}
}