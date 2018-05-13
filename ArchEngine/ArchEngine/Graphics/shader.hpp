/*===========================================================================*
 * Arch Engine - "Graphics/shader.hpp"                                       *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 13/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP


#include "../Config/engineMacros.hpp"
#include "glad_3_3_core.hpp"
#include "uniforms.hpp"
#include "../Script/luaScript.hpp"
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

		void destroy();

		void bind();
		static void unbindShaders();

		// Sends values to GPU
		void update();

		unsigned getProgramId();

		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setVec3(const std::string& name, const glm::vec3& vec);
		void setMat3(const std::string& name, const glm::mat3& matrix);
		void setMat4(const std::string& name, const glm::mat4& matrix);

	private:
		bool compileShader(const std::string& path, GLenum type);
		void notifyDirty(UniformPtr uniform);

		unsigned m_program_id;

		// Stores the ids to dirty uniforms
		std::vector<UniformPtr> m_dirty_uniforms;
		// References to the uniforms vector by name
		std::map<std::string, UniformPtr> m_uniforms_by_name;
	};
}


#endif	// GRAPHICS_SHADER_HPP