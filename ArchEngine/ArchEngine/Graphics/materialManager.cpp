/*===========================================================================*
 * Arch Engine - "Graphics/materialManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/05/2018                                                       *
 * Last Modified: 19/05/2018                                                 *
 *===========================================================================*/


#include "materialManager.hpp"


using namespace Utils;


namespace Graphics {
	MaterialManager::MaterialManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Material constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	MaterialManager::~MaterialManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Material destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	MaterialManager& MaterialManager::getInstance() {
		static MaterialManager instance;
		return instance;
	}

	unsigned MaterialManager::addTexture(const std::string& path) {
		auto it = m_texture_path_to_handle.find(path);

		// If the texture isn't in the vector
		if (it == this->m_texture_path_to_handle.end()) {
			//------------------------------------------------------------ TEST
			unsigned texture_id = loadTexture(path);

			unsigned handle;

			// If there aren't spaces available from previous removes
			if (m_textures_unused_spaces.empty()) {
				m_textures.push_back(std::make_pair(texture_id, 1));
				m_texture_path_to_handle[path] = m_textures.size() - 1;
				handle = m_textures.size() - 1;
			}
			else {
				handle = m_textures_unused_spaces.top();
				m_textures_unused_spaces.pop();

				m_textures[handle] = std::make_pair(texture_id, 1);
				m_texture_path_to_handle[path] = handle;
			}

			return handle;
		}

		// Increases texture reference
		m_textures[it->second].second++;
		return it->second;
	}

	void MaterialManager::removeTexture(unsigned handle) {
		if (handle >= m_textures.size()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove texture outside boundaries");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
			return;
		}

		if (m_textures[handle].second > 0)
			m_textures[handle].second--;
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
		else
			ServiceLocator::getFileLogger()->log<LOG_WARNING>(
				"Attempt to remove texture with 0 references");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING

		// If no one references the mesh anymore
		if (m_textures[handle].second == 0) {
			if (glIsTexture(m_textures[handle].first))
				glDeleteTextures(1, &m_textures[handle].first);

			// Remove reference from map
			for (auto it = m_texture_path_to_handle.begin();
				it != m_texture_path_to_handle.end(); ++it) {
				if (it->second == handle) {
					m_texture_path_to_handle.erase(it);
					break;
				}
			}

			m_textures_unused_spaces.push(handle);
		}
	}

	unsigned MaterialManager::loadTexture(const std::string& path) {
		unsigned texture_id;

		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		int width, height;
		unsigned char* image = stbi_load(path.c_str(), &width, &height, 0, 4);

		if (image == nullptr) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				path + " stb_image load failed!");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return 0;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return texture_id;
	}
}