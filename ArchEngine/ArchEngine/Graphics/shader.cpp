/*===========================================================================*
 * Arch Engine - "Graphics/shader.cpp"                                       *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 13/05/2018                                                 *
 *===========================================================================*/


#include "shader.hpp"


using namespace Script;
using namespace Utils;


namespace Graphics {
	Shader::Shader() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Shader constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	Shader::~Shader() {
		m_dirty_uniforms.clear();
		m_uniforms_by_name.clear();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Shader destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	bool Shader::initialize(
		const std::string& vs_path,
		const std::string& fs_path) {
		unsigned vs, fs;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"\n\n---------- BUILDING SHADER PROGRAM ----------\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		if (!compileShader(vs_path, GL_VERTEX_SHADER, vs))
			return false;

		if (!compileShader(fs_path, GL_FRAGMENT_SHADER, fs))
			return false;

		if (!linkProgram(vs, fs))
			return false;

		getAttributes();
		getUniforms();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"\n\n---------------------------------------------\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		return true;
	}

	bool Shader::initialize(
		const std::string& vs_path,
		const std::string& gs_path,
		const std::string& fs_path) {
		unsigned vs, gs, fs;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"\n\n---------- BUILDING SHADER PROGRAM ----------\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		if (!compileShader(vs_path, GL_VERTEX_SHADER, vs))
			return false;

		if (!compileShader(gs_path, GL_GEOMETRY_SHADER, gs))
			return false;

		if (!compileShader(fs_path, GL_FRAGMENT_SHADER, fs))
			return false;

		if (!linkProgram(vs, gs, fs))
			return false;

		getAttributes();
		getUniforms();

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"\n\n---------------------------------------------\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		return true;
	}

	void Shader::destroy() {
		if (glIsProgram(m_program_id))
			glDeleteProgram(m_program_id);
	}

	void Shader::bind() {
		glUseProgram(m_program_id);
	}

	void Shader::unbindShaders() {
		glUseProgram(0);
	}

	void Shader::update() {
		for (auto& it : m_dirty_uniforms)
			m_uniforms_by_name[it]->update();

		m_dirty_uniforms.clear();
	}
	
	unsigned Shader::getProgramId() {
		return m_program_id;
	}

	void Shader::setBool(const std::string& name, bool value) {
		auto uniform = std::static_pointer_cast<Uniform<bool>>(
			m_uniforms_by_name[name]);

		uniform->setValue(value);
	}

	void Shader::setInt(const std::string& name, int value) {
		auto uniform = std::static_pointer_cast<Uniform<int>>(
			m_uniforms_by_name[name]);

		uniform->setValue(value);
	}

	void Shader::setFloat(const std::string& name, float value) {
		auto uniform = std::static_pointer_cast<Uniform<float>>(
			m_uniforms_by_name[name]);

		uniform->setValue(value);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& vec) {
		auto uniform = std::static_pointer_cast<Uniform<glm::vec3>>(
			m_uniforms_by_name[name]);

		uniform->setValue(vec);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3& matrix) {
		auto uniform = std::static_pointer_cast<Uniform<glm::mat3>>(
			m_uniforms_by_name[name]);

		uniform->setValue(matrix);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& matrix) {
		auto uniform = std::static_pointer_cast<Uniform<glm::mat4>>(
			m_uniforms_by_name[name]);

		uniform->setValue(matrix);
	}

	bool Shader::compileShader(const std::string& path,
		GLenum type, unsigned& shader_handle) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			path + " shader compilation:");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		std::string source_code;
		std::stringstream ss;
		std::fstream file;

		file.open(path);
		ss << file.rdbuf();
		file.close();
		source_code = ss.str();

		const char* src = source_code.c_str();

		shader_handle = glCreateShader(type);
		glShaderSource(shader_handle, 1, &src, nullptr);
		glCompileShader(shader_handle);
		
		int success;
		int info_log_length;
		char* info_log;

		glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &info_log_length);

			info_log = new char[info_log_length + 1];
			glGetShaderInfoLog(shader_handle, info_log_length,
				nullptr, info_log);
			glDeleteShader(shader_handle);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"\n\n    " + path + " shader compilation failed:\n"
				+ std::string(info_log) + "\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

			delete info_log;

			return false;
		}

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			path + " sucessfully compiled");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		return true;
	}

	bool Shader::linkProgram(unsigned vs, unsigned fs) {
		m_program_id = glCreateProgram();
		
		glAttachShader(m_program_id, vs);
		glAttachShader(m_program_id, fs);

		glLinkProgram(m_program_id);

		glDetachShader(m_program_id, vs);
		glDetachShader(m_program_id, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);

		int success;
		int info_log_length;
		char* info_log;

		glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &info_log_length);

			info_log = new char[info_log_length + 1];
			glGetProgramInfoLog(m_program_id, info_log_length,
				nullptr, info_log);
			glDeleteProgram(m_program_id);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"\n\n    Program linking failed\n"
				+ std::string(info_log) + "\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

			delete info_log;

			return false;
		}

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Program sucessfully linked");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		return true;
	}

	bool Shader::linkProgram(unsigned vs, unsigned gs, unsigned fs) {
		m_program_id = glCreateProgram();
		
		glAttachShader(m_program_id, vs);
		glAttachShader(m_program_id, gs);
		glAttachShader(m_program_id, fs);

		glLinkProgram(m_program_id);

		glDetachShader(m_program_id, vs);
		glDetachShader(m_program_id, gs);
		glDetachShader(m_program_id, fs);

		glDeleteShader(vs);
		glDeleteShader(gs);
		glDeleteShader(fs);

		int success;
		int info_log_length;
		char* info_log;

		glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &info_log_length);

			info_log = new char[info_log_length + 1];
			glGetProgramInfoLog(m_program_id, info_log_length,
				nullptr, info_log);
			glDeleteProgram(m_program_id);

			delete info_log;

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"\n\n    Program linking failed\n");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

			return false;
		}

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Program sucessfully linked");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		return true;
	}

	void Shader::getAttributes() {
		// TODO
	}

	void Shader::getUniforms() {
		DirtyObserver observer;
		observer.bind<Shader, &Shader::notifyDirty>(this);

		int count, size;
		GLenum type;

		const int BUF_SIZE = 32;
		char name[BUF_SIZE];
		int name_length;

		glGetProgramiv(m_program_id, GL_ACTIVE_UNIFORMS, &count);

#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_INFO
		ServiceLocator::getFileLogger()->log<LOG_INFO>(
			"Found " + std::to_string(count) + " uniforms");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_INFO

		for (unsigned i = 0; i < (unsigned)count; i++) {
			glGetActiveUniform(m_program_id, i, BUF_SIZE,
				&name_length, &size, &type, name);

			int location = glGetUniformLocation(m_program_id, name);

			switch (type) {
			case GL_BOOL:
				m_uniforms_by_name.insert(std::make_pair(
					name, std::make_shared<Uniform<bool>>(
						name, location, observer)));
				break;
			case GL_INT:
				m_uniforms_by_name.insert(std::make_pair(
					name, std::make_shared<Uniform<int>>(
						name, location, observer)));
				break;
			case GL_FLOAT:
				m_uniforms_by_name.insert(std::make_pair(
					name, std::make_shared<Uniform<float>>(
						name, location, observer)));
				break;
			case GL_FLOAT_VEC3:
				m_uniforms_by_name.insert(std::make_pair(
					name, std::make_shared<Uniform<glm::vec3>>(
						name, location, observer)));
				break;
			case GL_FLOAT_MAT3:
				m_uniforms_by_name.insert(std::make_pair(
					name, std::make_shared<Uniform<glm::mat3>>(
						name, location, observer)));
				break;
			case GL_FLOAT_MAT4:
				m_uniforms_by_name.insert(std::make_pair(
					name, std::make_shared<Uniform<glm::mat4>>(
						name, location, observer)));
				break;
			case GL_SAMPLER_2D:
				m_uniforms_by_name.insert(std::make_pair(
					name, std::make_shared<Uniform<int>>(
						name, location, observer)));
				break;
			}
		}
	}

	void Shader::notifyDirty(const std::string& uniform_name) {
		m_dirty_uniforms.push_back(uniform_name);
	}
}