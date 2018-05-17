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

	void Mesh::create(VertexMask mask, const std::vector<Vertex>& vertices,
		const std::vector<unsigned> indices, unsigned material_id) {
		unsigned vbo_id, ebo_id;

		glGenVertexArrays(1, &m_vao_id);
		glBindVertexArray(m_vao_id);

		glGenBuffers(1, &vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() *
			sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
			sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

		if (mask & VERTEX_POSITION == VERTEX_POSITION) {
			glEnableVertexAttribArray(VERTEX_POSITION_LOCATION);
			glVertexAttribPointer(VERTEX_POSITION_LOCATION, 3, GL_FLOAT,
				GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		}

		if (mask & VERTEX_NORMAL == VERTEX_NORMAL) {
			glEnableVertexAttribArray(VERTEX_NORMAL_LOCATION);
			glVertexAttribPointer(VERTEX_NORMAL_LOCATION, 3, GL_FLOAT,
				GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		}

		if (mask & VERTEX_TEXTURE_COORDS == VERTEX_TEXTURE_COORDS) {
			glEnableVertexAttribArray(VERTEX_TEXTURE_COORDS_LOCATION);
			glVertexAttribPointer(VERTEX_TEXTURE_COORDS_LOCATION, 2, GL_FLOAT,
				GL_FALSE, sizeof(Vertex),
				(void*)offsetof(Vertex, texture_coords));
		}

		if (mask & VERTEX_TANGENT == VERTEX_TANGENT) {
			glEnableVertexAttribArray(VERTEX_TANGENT_LOCATION);
			glVertexAttribPointer(VERTEX_TANGENT_LOCATION, 3, GL_FLOAT,
				GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
		}

		if (mask & VERTEX_BONE_IDS == VERTEX_BONE_IDS) {
			glEnableVertexAttribArray(VERTEX_BONE_IDS_LOCATION);
			glVertexAttribIPointer(VERTEX_BONE_IDS_LOCATION, 4, GL_INT,
				sizeof(Vertex), (void*)offsetof(Vertex, bone_ids));
		}

		if (mask & VERTEX_BONE_WEIGHT == VERTEX_BONE_WEIGHT) {
			glEnableVertexAttribArray(VERTEX_BONE_WEIGHT_LOCATION);
			glVertexAttribPointer(VERTEX_BONE_WEIGHT_LOCATION, 4, GL_FLOAT,
				GL_FALSE, sizeof(Vertex),
				(void*)offsetof(Vertex, bone_weights));
		}

		if (mask & VERTEX_INSTANCE_MATRIX == VERTEX_INSTANCE_MATRIX) {
			glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX);
			glVertexAttribPointer(VERTEX_INSTANCE_MATRIX, 4, GL_FLOAT,
				GL_FALSE, sizeof(Vertex),
				(void*)offsetof(Vertex, instance_matrix));

			glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX + 1);
			glVertexAttribPointer(VERTEX_INSTANCE_MATRIX + 1, 4, GL_FLOAT,
				GL_FALSE, sizeof(Vertex),
				(void*)(offsetof(Vertex, instance_matrix) +
					sizeof(glm::vec4)));

			glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX + 2);
			glVertexAttribPointer(VERTEX_INSTANCE_MATRIX + 2, 4, GL_FLOAT,
				GL_FALSE, sizeof(Vertex),
				(void*)(offsetof(Vertex, instance_matrix) +
					2 * sizeof(glm::vec4)));

			glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX + 3);
			glVertexAttribPointer(VERTEX_INSTANCE_MATRIX + 3, 4, GL_FLOAT,
				GL_FALSE, sizeof(Vertex),
				(void*)(offsetof(Vertex, instance_matrix) +
					3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX, 1);
			glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX + 1, 1);
			glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX + 2, 1);
			glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX + 3, 1);
			glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX + 3, 1);
		}

		glBindVertexArray(0);
	}

	void Mesh::destroy() {
		if (glIsVertexArray(m_vao_id))
			glDeleteVertexArrays(1, &m_vao_id);

		// TODO
	}
}