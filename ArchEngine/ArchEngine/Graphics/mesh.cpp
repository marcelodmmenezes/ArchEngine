/*===========================================================================*
 * Arch Engine - "Graphics/mesh.hpp"                                         *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
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

	void Mesh::create(const std::vector<CompleteVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<CompleteVertex>();
		specifyNormal<CompleteVertex>();
		specifyTextureCoords<CompleteVertex>();
		specifyTangent<CompleteVertex>();
		specifyBoneIds<CompleteVertex>();
		specifyBoneWeights<CompleteVertex>();
		specifyInstancedMatrix<CompleteVertex>();

		glBindVertexArray(0);
	}

	void Mesh::create(const std::vector<BasicVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<BasicVertex>();
		specifyNormal<BasicVertex>();
		specifyTextureCoords<BasicVertex>();

		glBindVertexArray(0);
	}

	void Mesh::create(const std::vector<NormalMappedVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<NormalMappedVertex>();
		specifyNormal<NormalMappedVertex>();
		specifyTextureCoords<NormalMappedVertex>();
		specifyTangent<NormalMappedVertex>();

		glBindVertexArray(0);
	}

	void Mesh::create(const std::vector<AnimatedVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<AnimatedVertex>();
		specifyNormal<AnimatedVertex>();
		specifyTextureCoords<AnimatedVertex>();
		specifyBoneIds<AnimatedVertex>();
		specifyBoneWeights<AnimatedVertex>();

		glBindVertexArray(0);
	}

	void Mesh::create(const std::vector<InstancedVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<InstancedVertex>();
		specifyNormal<InstancedVertex>();
		specifyTextureCoords<InstancedVertex>();
		specifyInstancedMatrix<InstancedVertex>();

		glBindVertexArray(0);
	}

	void Mesh::create(const std::vector<AnimatedNormalMappedVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<AnimatedNormalMappedVertex>();
		specifyNormal<AnimatedNormalMappedVertex>();
		specifyTextureCoords<AnimatedNormalMappedVertex>();
		specifyTangent<AnimatedNormalMappedVertex>();
		specifyBoneIds<AnimatedNormalMappedVertex>();
		specifyBoneWeights<AnimatedNormalMappedVertex>();

		glBindVertexArray(0);
	}

	void Mesh::create(const std::vector<InstancedNormalMappedVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<InstancedNormalMappedVertex>();
		specifyNormal<InstancedNormalMappedVertex>();
		specifyTextureCoords<InstancedNormalMappedVertex>();
		specifyTangent<InstancedNormalMappedVertex>();
		specifyInstancedMatrix<InstancedNormalMappedVertex>();

		glBindVertexArray(0);
	}

	void Mesh::create(const std::vector<InstancedAnimatedVertex>& vertices,
		const std::vector<unsigned>& indices, unsigned material_id) {

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

		specifyPosition<InstancedAnimatedVertex>();
		specifyNormal<InstancedAnimatedVertex>();
		specifyTextureCoords<InstancedAnimatedVertex>();
		specifyBoneIds<InstancedAnimatedVertex>();
		specifyBoneWeights<InstancedAnimatedVertex>();
		specifyInstancedMatrix<InstancedAnimatedVertex>();

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