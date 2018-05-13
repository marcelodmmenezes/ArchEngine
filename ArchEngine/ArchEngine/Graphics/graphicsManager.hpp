/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.hpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 12/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_GRAPHICS_MANAGER_HPP
#define GRAPHICS_GRAPHICS_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "glad_3_3_core.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>

#include <string>


namespace Graphics {
	class GraphicsManager {
	public:
		~GraphicsManager();

		GraphicsManager(const GraphicsManager&) = delete;
		void operator=(const GraphicsManager&) = delete;

		static GraphicsManager& getInstance();

		bool initialize(const glm::vec4& color);
		bool initializeFromConfigFile(const std::string& path);

		void update(float delta_time);

		void destroy();

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		State m_state;

		GraphicsManager();
	};
}


#endif	// GRAPHICS_GRAPHICS_MANAGER_HPP