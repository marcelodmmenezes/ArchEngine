/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 22/05/2018                                                 *
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

	bool GraphicsManager::initialize(const glm::vec4& color,
		int width, int height) {
		// TODO

		m_window_size_listener.bind
			<GraphicsManager, &GraphicsManager::onWindowResizeEvent>(this);
		EventManager::getInstance().addListener(
			m_window_size_listener, EVENT_WINDOW_RESIZE);

		//---------------------------------------------------------------- TEST
		onWindowResizeEvent(EventPtr(new WindowResizeEvent(800, 600)));
		//---------------------------------------------------------------------

		glClearColor(color.r, color.g, color.b, color.a);

		return true;
	}

	bool GraphicsManager::initializeFromConfigFile(const std::string& path) {
		// TODO
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);

		//---------------------------------------------------------------- TEST
		return initialize(glm::vec4(0.05f, 0.08f, 0.07f, 1.0f), 800, 600);
		//---------------------------------------------------------------------
	}

	void GraphicsManager::update(float delta_time) {
		// TODO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//---------------------------------------------------------------- TEST

		m_cameras[0].m_delta_time = delta_time;

		auto &it = m_shaders[0];

		//for (auto& it : m_shaders) {
			it.bind();
			it.setMat4("u_projection", m_projection);
			it.setMat4("u_view", m_cameras[0].getViewMatrix());
			it.setVec3("u_view_pos", m_cameras[0].getPosition());

			for (int i = 0; i < (int)m_meshes.size(); i++) {
				glm::mat4 model;

				if (i < 25)
					model = glm::scale(glm::mat4(1.0f),
						glm::vec3(0.1f, 0.1f, 0.1f));
				else
					model = glm::scale(glm::rotate(glm::translate(
						glm::mat4(1.0f), glm::vec3(0.0f, -0.25f, -5.0f)),
						glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
						glm::vec3(1.2f, 1.2f, 1.2f));

				//model = glm::mat4(1.0f);

				it.setMat4("u_model", model);
				it.setMat3("u_trn_inv_up_model",
					glm::transpose(glm::inverse(glm::mat3(model))));

				unsigned texture = m_materials[m_meshes[i].first.m_material_id].
					textures[TEXTURE_DIFFUSE];

				if (texture < UINT_MAX) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D,
						MaterialManager::getInstance().getTexture(texture));
					it.setInt("u_texture_0", 0);
				}

				texture = m_materials[m_meshes[i].first.m_material_id].
					textures[TEXTURE_SPECULAR];

				if (texture < UINT_MAX) {
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D,
						MaterialManager::getInstance().getTexture(
							m_materials[m_meshes[i].first.m_material_id].
							textures[TEXTURE_SPECULAR]));
					it.setInt("u_texture_1", 1);
				}

				texture = m_materials[m_meshes[i].first.m_material_id].
					textures[TEXTURE_HEIGHT];

				if (texture < UINT_MAX) {
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D,
						MaterialManager::getInstance().getTexture(
							m_materials[m_meshes[i].first.m_material_id].
							textures[TEXTURE_HEIGHT]));
					it.setInt("u_texture_2", 2);
				}

				it.update();
				m_meshes[i].first.draw();

				for (int t = 0; t < NUMBER_OF_TEXTURE_TYPES; t++) {
					glActiveTexture(GL_TEXTURE0 + t);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
		//}

		//---------------------------------------------------------------------
	}

	void GraphicsManager::destroy() {
		// TODO
	}

	void GraphicsManager::onWindowResizeEvent(EventPtr e) {
		auto evnt = std::static_pointer_cast<WindowResizeEvent>(e);

		int w, h;
		evnt->getSize(w, h);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Window size: " + std::to_string(w) + " " + std::to_string(h));
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG

		//---------------------------------------------------------------- TEST
		m_projection = glm::perspective(
			glm::radians(45.0f), (float)w / (float)h, 0.1f, 10000.0f);
		//---------------------------------------------------------------------

		glViewport(0, 0, w, h);
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
}