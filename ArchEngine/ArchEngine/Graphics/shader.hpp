/*===========================================================================*
 * Arch Engine - "Graphics/shader.hpp"                                       *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP


#include "../Config/engineMacros.hpp"
#include "../Core/eventManager.hpp"
#include "glad_3_3_core.hpp"
#include "uniforms.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/fileWatcher.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <map>
#include <string>
#include <vector>


namespace Graphics {
	/*
	 * ArchEngine GLSL shaders attributes are defined like this,
	 * in order to easily bind different VAOs to different shaders.
	 *
	 * layout(location = 0) in vec3 v_position;
	 * layout(location = 1) in vec3 v_normal;
	 * layout(location = 2) in vec2 v_texture_coords;
	 * layout(location = 3) in vec3 v_tangent;
	 * layout(location = 4) in ivec4 v_bone_ids;
	 * layout(location = 5) in vec4 v_bone_weights;
	 * layout(location = 6) in mat4 v_instance_model_matrix;
	 */

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

		void notifyDirty(const std::string& uniform_name);

		unsigned m_program_id;

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