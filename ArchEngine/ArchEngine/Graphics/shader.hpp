/*===========================================================================*
 * Arch Engine - "Graphics/shader.hpp"                                       *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 01/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP


#include "../Config/engineMacros.hpp"
#include "../Core/eventManager.hpp"
#include "glad_3_3_core.hpp"
#include "uniforms.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/debugging.hpp"
#include "../Utils/fileWatcher.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <map>
#include <string>
#include <vector>


namespace Graphics {
	class Shader {
	public:
		Shader();
		~Shader();

		// Vertex and fragment shaders
		bool initialize(
			const std::string& vs_path,
			const std::string& fs_path);

		// Vertex, geometry and fragment shaders
		bool initialize(
			const std::string& vs_path,
			const std::string& gs_path,
			const std::string& fs_path);

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		void onFileModifiedEvent(Core::EventPtr e);
#endif	// ARCH_ENGINE_HOT_RELOAD_ON

		void bind();
		static void unbindShaders();

		// Sends values to GPU
		void update();

		void destroy();

		unsigned getProgramId();
		bool hasNormalMap();
		bool hasDirLights();
		bool hasPointLights();
		bool hasSpotLights();

		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setVec3(const std::string& name, const glm::vec3& vec);
		void setMat3(const std::string& name, const glm::mat3& matrix);
		void setMat4(const std::string& name, const glm::mat4& matrix);

	private:
		bool compileShader(const std::string& path,
			GLenum type, unsigned& shader_handle);

		bool linkProgram(unsigned vs, unsigned fs);
		bool linkProgram(unsigned vs, unsigned gs, unsigned fs);

		void getAttributes();
		void getUniforms();

		unsigned m_program_id;

		// Shader properties
		bool m_has_normal_map;
		bool m_has_dir_lights;
		bool m_has_point_lights;
		bool m_has_spot_lights;

		// Stores the ids to dirty uniforms
		std::vector<std::string> m_dirty_uniforms;
		// References to the uniforms vector by name
		std::map<std::string, UniformPtr> m_uniforms_by_name;

#if defined(ARCH_ENGINE_HOT_RELOAD_ON)
		// Hot-reloading configuration
		std::string m_vs_path, m_gs_path, m_fs_path;
		bool m_watch_file;
		Core::EventListener m_file_modified_listener;
#endif	// ARCH_ENGINE_HOT_RELOAD_ON
	};
}


#endif	// GRAPHICS_SHADER_HPP