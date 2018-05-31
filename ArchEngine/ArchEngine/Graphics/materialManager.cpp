/*===========================================================================*
 * Arch Engine - "Graphics/materialManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/05/2018                                                       *
 * Last Modified: 31/05/2018                                                 *
 *===========================================================================*/


#include "materialManager.hpp"


using namespace Utils;


namespace Graphics {
	MaterialManager::MaterialManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"MaterialManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	MaterialManager::~MaterialManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"MaterialManager destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	MaterialManager& MaterialManager::getInstance() {
		static MaterialManager instance;
		return instance;
	}

	void MaterialManager::destroy() {
		for (auto& it : m_2d_textures)
			if (glIsTexture(it.first))
				glDeleteTextures(1, &it.first);

		for (auto& it : m_cube_textures)
			if (glIsTexture(it.first))
				glDeleteTextures(1, &it.first);
	}

	Material MaterialManager::buildMaterial(const std::vector
		<std::pair<std::string, TextureType>>& textures) {
		Material material;

		for (auto& it : textures)
			material.textures[it.second] = add2DTexture(it.first);

		return material;
	}

	unsigned MaterialManager::add2DTexture(const std::string& path) {
		auto it = m_2d_texture_path_to_handle.find(path);

		// If the texture isn't in the vector
		if (it == this->m_2d_texture_path_to_handle.end()) {
			unsigned texture_id = load2DTexture(path);

			unsigned handle;

			// If there aren't spaces available from previous removes
			if (m_2d_textures_unused_spaces.empty()) {
				m_2d_textures.push_back(std::make_pair(texture_id, 1));
				m_2d_texture_path_to_handle[path] = m_2d_textures.size() - 1;
				handle = m_2d_textures.size() - 1;
			}
			else {
				handle = m_2d_textures_unused_spaces.top();
				m_2d_textures_unused_spaces.pop();

				m_2d_textures[handle] = std::make_pair(texture_id, 1);
				m_2d_texture_path_to_handle[path] = handle;
			}

			return handle;
		}

		// Increases texture reference
		m_2d_textures[it->second].second++;
		return it->second;
	}

	unsigned MaterialManager::get2DTexture(unsigned handle) {
		if (handle >= m_2d_textures.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"MaterialManager get2DTexture handle out of bounds");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return 0;
		}

		return m_2d_textures[handle].first;
	}

	void MaterialManager::remove2DTexture(unsigned handle) {
		if (handle >= m_2d_textures.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove 2d texture outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		if (m_2d_textures[handle].second > 0)
			m_2d_textures[handle].second--;
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
		else
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove 2d texture with 0 references");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING

		// If no one references the texture anymore
		if (m_2d_textures[handle].second == 0) {
			if (glIsTexture(m_2d_textures[handle].first))
				glDeleteTextures(1, &m_2d_textures[handle].first);

			// Remove reference from map
			for (auto it = m_2d_texture_path_to_handle.begin();
				it != m_2d_texture_path_to_handle.end(); ++it) {
				if (it->second == handle) {
					m_2d_texture_path_to_handle.erase(it);
					break;
				}
			}

			m_2d_textures_unused_spaces.push(handle);
		}
	}
	
	unsigned MaterialManager::load2DTexture(const std::string& path) {
		unsigned texture_id;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		int width, height, n_components;
		GLenum format;

		unsigned char* image = stbi_load(path.c_str(),
			&width, &height, &n_components, 0);

		if (n_components == 1)
			format = GL_RED;
		else if (n_components == 3)
			format = GL_RGB;
		else if (n_components == 4)
			format = GL_RGBA;

		if (image == nullptr) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				path + " stb_image load failed!");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return 0;
		}
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		else
			ServiceLocator::getFileLogger()->log<LOG_INFO>(
				path + " loaded successfully!");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
			0, format, GL_UNSIGNED_BYTE, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return texture_id;
	}

	unsigned MaterialManager::addCubeTexture(const std::string path[6]) {
		auto it = m_cube_texture_path_to_handle.find(path[0]);

		// If the texture isn't in the vector
		if (it == this->m_cube_texture_path_to_handle.end()) {
			unsigned texture_id = loadCubeTexture(path);

			unsigned handle;

			// If there aren't spaces available from previous removes
			if (m_cube_textures_unused_spaces.empty()) {
				m_cube_textures.push_back(std::make_pair(texture_id, 1));
				m_cube_texture_path_to_handle[path[0]] =
					m_cube_textures.size() - 1;
				handle = m_cube_textures.size() - 1;
			}
			else {
				handle = m_cube_textures_unused_spaces.top();
				m_cube_textures_unused_spaces.pop();

				m_cube_textures[handle] = std::make_pair(texture_id, 1);
				m_cube_texture_path_to_handle[path[0]] = handle;
			}

			return handle;
		}

		// Increases texture reference
		m_cube_textures[it->second].second++;
		return it->second;
	}

	unsigned MaterialManager::getCubeTexture(unsigned handle) {
		if (handle >= m_cube_textures.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"MaterialManager getCubeTexture handle out of bounds");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return 0;
		}

		return m_cube_textures[handle].first;
	}

	void MaterialManager::removeCubeTexture(unsigned handle) {
		if (handle >= m_cube_textures.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove cube texture outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		if (m_cube_textures[handle].second > 0)
			m_cube_textures[handle].second--;
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
		else
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove cube texture with 0 references");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING

		// If no one references the texture anymore
		if (m_cube_textures[handle].second == 0) {
			if (glIsTexture(m_cube_textures[handle].first))
				glDeleteTextures(1, &m_cube_textures[handle].first);

			// Remove reference from map
			for (auto it = m_cube_texture_path_to_handle.begin();
				it != m_cube_texture_path_to_handle.end(); ++it) {
				if (it->second == handle) {
					m_cube_texture_path_to_handle.erase(it);
					break;
				}
			}

			m_cube_textures_unused_spaces.push(handle);
		}
	}

	unsigned MaterialManager::loadCubeTexture(const std::string path[6]) {
		unsigned texture_id;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

		int width, height, n_components;
		GLenum format;

		for (unsigned i = 0; i < 6; i++) {
			unsigned char *image = stbi_load(path[i].c_str(),
				&width, &height, &n_components, 0);

			if (n_components == 1)
				format = GL_RED;
			else if (n_components == 3)
				format = GL_RGB;
			else if (n_components == 4)
				format = GL_RGBA;

			if (image == nullptr) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
				ServiceLocator::getFileLogger()->log<LOG_ERROR>(
					path[0] + " cube stb_image load failed!");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
				return 0;
			}
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
			else
				ServiceLocator::getFileLogger()->log<LOG_INFO>(
					path[0] + " cube loaded successfully!");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format,
				width, height, 0, format, GL_UNSIGNED_BYTE, image);

			stbi_image_free(image);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return texture_id;
	}
}