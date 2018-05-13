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

	}

	Shader::~Shader() {

	}

	bool Shader::initialize(
		const std::string& vs_path,
		const std::string& fs_path) {

		DirtyObserver observer;
		observer.bind<Shader, &Shader::notifyDirty>(this);
		m_uniforms_by_name["test"] = UniformPtr(new Uniform1f(0, 0, observer));

		return true;
	}

	bool Shader::initialize(
		const std::string& vs_path,
		const std::string& gs_path,
		const std::string& fs_path) {

		return true;
	}

	void Shader::destroy() {

	}

	void Shader::bind() {

	}

	void Shader::unbindShaders() {

	}

	void Shader::update() {
		for (auto& it : m_dirty_uniforms)
			it->update();

		m_dirty_uniforms.clear();
	}
	
	unsigned Shader::getProgramId() {
		return m_program_id;
	}

	void Shader::setBool(const std::string& name, bool value) {
		auto uniform = std::static_pointer_cast<Uniform1f>(
			m_uniforms_by_name[name]);

		uniform->setValue(value);
	}

	void Shader::setInt(const std::string& name, int value) {
		auto uniform = std::static_pointer_cast<Uniform1f>(
			m_uniforms_by_name[name]);

		uniform->setValue(value);
	}

	void Shader::setFloat(const std::string& name, float value) {
		auto uniform = std::static_pointer_cast<Uniform1f>(
			m_uniforms_by_name[name]);

		uniform->setValue(value);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& vec) {
		auto uniform = std::static_pointer_cast<Uniform1f>(
			m_uniforms_by_name[name]);

		uniform->setValue(vec);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3& matrix) {
		auto uniform = std::static_pointer_cast<Uniform1f>(
			m_uniforms_by_name[name]);

		uniform->setValue(matrix);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& matrix) {
		auto uniform = std::static_pointer_cast<Uniform1f>(
			m_uniforms_by_name[name]);

		uniform->setValue(matrix);
	}

	bool Shader::compileShader(const std::string& path, GLenum type) {
		// TODO
		return true;
	}

	void Shader::notifyDirty(UniformPtr uniform) {
		m_dirty_uniforms.push_back(uniform);
	}
}