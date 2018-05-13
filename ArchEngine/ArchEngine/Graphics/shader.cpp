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

		// TODO
		DirtyObserver observer;
		observer.bind<Shader, &Shader::notifyDirty>(this);
		m_uniforms_by_name["test"] = std::make_shared<Uniform<float>>("test", 0, observer);

		return true;
	}

	bool Shader::initialize(
		const std::string& vs_path,
		const std::string& gs_path,
		const std::string& fs_path) {

		// TODO

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

	bool Shader::compileShader(const std::string& path, GLenum type) {
		// TODO
		return true;
	}

	void Shader::notifyDirty(const std::string& uniform_name) {
		m_dirty_uniforms.push_back(uniform_name);
	}
}