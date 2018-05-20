/*===========================================================================*
 * Arch Engine - "Graphics/materialManager.hpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 19/05/2018                                                       *
 * Last Modified: 19/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_MATERIAL_MANAGER_HPP
#define GRAPHICS_MATERIAL_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "glad_3_3_core.hpp"
#include "primitives.hpp"
#include "../Utils/serviceLocator.hpp"

#include "stb_image.hpp"

#include <map>
#include <stack>
#include <string>
#include <vector>


namespace Graphics {
	class MaterialManager {
	public:
		~MaterialManager();

		MaterialManager(const MaterialManager&) = delete;
		void operator=(const MaterialManager&) = delete;

		static MaterialManager& getInstance();

		unsigned addTexture(const std::string& path);
		void removeTexture(unsigned handle);

	private:
		MaterialManager();

		unsigned loadTexture(const std::string& path);

		// All the game textures are stored here.
		// - The pair tells how many game entities reference a given texture.
		//   When it's reference count reaches 0 the texture is marked as
		//   removed. New textures can be added at the new empty position.
		std::vector<std::pair<unsigned, unsigned>> m_textures;

		// Map used to reference a texture by its path. Used internally by the
		// manager to control how textures are added and removed.
		std::map<std::string, unsigned> m_texture_path_to_handle;

		// Stack used to store unused spaces at m_textures.
		std::stack<unsigned> m_textures_unused_spaces;
	};
}


#endif	// GRAPHICS_MATERIAL_MANAGER_HPP