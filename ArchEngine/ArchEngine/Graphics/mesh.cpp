/*===========================================================================*
 * Arch Engine - "Graphics/mesh.hpp"                                         *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 20/05/2018                                                 *
 *===========================================================================*/


#include "mesh.hpp"


using namespace Utils;


namespace Graphics {
	Mesh::Mesh() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Mesh constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	Mesh::~Mesh() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"Mesh destructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<CompleteVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(CompleteVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<CompleteVertex>(0);
		specifyNormal<CompleteVertex>(1);
		specifyTextureCoords<CompleteVertex>(2);
		specifyTangent<CompleteVertex>(3);
		specifyBitangent<CompleteVertex>(4);
		specifyBoneIds<CompleteVertex>(5);
		specifyBoneWeights<CompleteVertex>(6);
		specifyInstancedMatrix<CompleteVertex>(7);

		glBindVertexArray(0);
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<BasicVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(BasicVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<BasicVertex>(0);
		specifyNormal<BasicVertex>(1);
		specifyTextureCoords<BasicVertex>(2);

		glBindVertexArray(0);
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<NormalMappedVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(NormalMappedVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<NormalMappedVertex>(0);
		specifyNormal<NormalMappedVertex>(1);
		specifyTextureCoords<NormalMappedVertex>(2);
		specifyTangent<NormalMappedVertex>(3);
		specifyBitangent<NormalMappedVertex>(4);

		glBindVertexArray(0);
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<AnimatedVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(AnimatedVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<AnimatedVertex>(0);
		specifyNormal<AnimatedVertex>(1);
		specifyTextureCoords<AnimatedVertex>(2);
		specifyBoneIds<AnimatedVertex>(3);
		specifyBoneWeights<AnimatedVertex>(4);

		glBindVertexArray(0);
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<InstancedVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(InstancedVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<InstancedVertex>(0);
		specifyNormal<InstancedVertex>(1);
		specifyTextureCoords<InstancedVertex>(2);
		specifyInstancedMatrix<InstancedVertex>(3);

		glBindVertexArray(0);
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<AnimatedNormalMappedVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(AnimatedNormalMappedVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<AnimatedNormalMappedVertex>(0);
		specifyNormal<AnimatedNormalMappedVertex>(1);
		specifyTextureCoords<AnimatedNormalMappedVertex>(2);
		specifyTangent<AnimatedNormalMappedVertex>(3);
		specifyBitangent<AnimatedNormalMappedVertex>(4);
		specifyBoneIds<AnimatedNormalMappedVertex>(5);
		specifyBoneWeights<AnimatedNormalMappedVertex>(6);

		glBindVertexArray(0);
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<InstancedNormalMappedVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(InstancedNormalMappedVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<InstancedNormalMappedVertex>(0);
		specifyNormal<InstancedNormalMappedVertex>(1);
		specifyTextureCoords<InstancedNormalMappedVertex>(2);
		specifyTangent<InstancedNormalMappedVertex>(3);
		specifyBitangent<InstancedNormalMappedVertex>(4);
		specifyInstancedMatrix<InstancedNormalMappedVertex>(5);

		glBindVertexArray(0);
	}

	void Mesh::create(const std::string& name, unsigned material_id,
		const std::vector<InstancedAnimatedVertex>& vertices,
		const std::vector<unsigned>& indices) {
		m_name = name;
		m_material_id = material_id;
		m_number_of_indices = indices.size();

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &m_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(InstancedAnimatedVertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &m_ebo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		specifyPosition<InstancedAnimatedVertex>(0);
		specifyNormal<InstancedAnimatedVertex>(1);
		specifyTextureCoords<InstancedAnimatedVertex>(2);
		specifyBoneIds<InstancedAnimatedVertex>(3);
		specifyBoneWeights<InstancedAnimatedVertex>(4);
		specifyInstancedMatrix<InstancedAnimatedVertex>(5);

		glBindVertexArray(0);
	}

	void Mesh::draw() {
		glBindVertexArray(m_vao_id);
		glDrawElements(GL_TRIANGLES, m_number_of_indices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::instancedDraw(int amount) {
		glBindVertexArray(m_vao_id);
		glDrawElementsInstanced(GL_TRIANGLES, m_number_of_indices,
			GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}

	void Mesh::destroy() {
		if (glIsBuffer(m_ebo_id))
			glDeleteBuffers(1, &m_ebo_id);

		if (glIsBuffer(m_vbo_id))
			glDeleteBuffers(1, &m_vbo_id);

		if (glIsVertexArray(m_vao_id))
			glDeleteVertexArrays(1, &m_vao_id);
	}
}