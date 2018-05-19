/*===========================================================================*
 * Arch Engine - "Graphics/mesh.hpp"                                         *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_MESH_HPP
#define GRAPHICS_MESH_HPP


#include "glad_3_3_core.hpp"
#include "primitives.hpp"
#include "mesh.hpp"
#include "../Utils/serviceLocator.hpp"

#include <vector>


namespace Graphics {
	class Mesh {
	public:
		Mesh();
		~Mesh();

		void create(const std::vector<CompleteVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void create(const std::vector<BasicVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void create(const std::vector<NormalMappedVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void create(const std::vector<AnimatedVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void create(const std::vector<InstancedVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void create(const std::vector<AnimatedNormalMappedVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void create(const std::vector<InstancedNormalMappedVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void create(const std::vector<InstancedAnimatedVertex>& vertices,
			const std::vector<unsigned>& indices, unsigned material_id);

		void draw();
		void instancedDraw(int amount);

		void destroy();

	private:
		// Helper methods for vertex specification
		template<typename T>
		void specifyPosition();
		template<typename T>
		void specifyNormal();
		template<typename T>
		void specifyTextureCoords();
		template<typename T>
		void specifyTangent();
		template<typename T>
		void specifyBoneIds();
		template<typename T>
		void specifyBoneWeights();
		template<typename T>
		void specifyInstancedMatrix();

		// OpenGL handles of this mesh
		unsigned m_vao_id;
		unsigned m_vbo_id;
		unsigned m_ebo_id;

		unsigned m_number_of_indices;

		// Id to it's material
		unsigned m_material_id;
	};

	template<typename T>
	void Mesh::specifyPosition() {
		glEnableVertexAttribArray(VERTEX_POSITION_LOCATION);
		glVertexAttribPointer(VERTEX_POSITION_LOCATION, 3, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, position));
	}

	template<typename T>
	void Mesh::specifyNormal() {
		glEnableVertexAttribArray(VERTEX_NORMAL_LOCATION);
		glVertexAttribPointer(VERTEX_NORMAL_LOCATION, 3, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, normal));
	}

	template<typename T>
	void Mesh::specifyTextureCoords() {
		glEnableVertexAttribArray(VERTEX_TEXTURE_COORDS_LOCATION);
		glVertexAttribPointer(VERTEX_TEXTURE_COORDS_LOCATION, 2, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, texture_coords));
	}

	template<typename T>
	void Mesh::specifyTangent() {
		glEnableVertexAttribArray(VERTEX_TANGENT_LOCATION);
		glVertexAttribPointer(VERTEX_TANGENT_LOCATION, 3, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, tangent));
	}

	template<typename T>
	void Mesh::specifyBoneIds() {
		glEnableVertexAttribArray(VERTEX_BONE_IDS_LOCATION);
		glVertexAttribIPointer(VERTEX_BONE_IDS_LOCATION, 4, GL_INT,
			sizeof(T), (void*)offsetof(T, bone_ids));
	}

	template<typename T>
	void Mesh::specifyBoneWeights() {
		glEnableVertexAttribArray(VERTEX_BONE_WEIGHT_LOCATION);
		glVertexAttribPointer(VERTEX_BONE_WEIGHT_LOCATION, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, bone_weights));
	}

	template<typename T>
	void Mesh::specifyInstancedMatrix() {
		glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX_LOCATION);
		glVertexAttribPointer(VERTEX_INSTANCE_MATRIX_LOCATION, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, instance_matrix));

		glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX_LOCATION + 1);
		glVertexAttribPointer(VERTEX_INSTANCE_MATRIX_LOCATION + 1, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)(offsetof(T, instance_matrix) +
				sizeof(glm::vec4)));

		glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX_LOCATION + 2);
		glVertexAttribPointer(VERTEX_INSTANCE_MATRIX_LOCATION + 2, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)(offsetof(T, instance_matrix) +
				2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(VERTEX_INSTANCE_MATRIX_LOCATION + 3);
		glVertexAttribPointer(VERTEX_INSTANCE_MATRIX_LOCATION + 3, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)(offsetof(T, instance_matrix) +
				3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX_LOCATION, 1);
		glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX_LOCATION + 1, 1);
		glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX_LOCATION + 2, 1);
		glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX_LOCATION + 3, 1);
		glVertexAttribDivisor(VERTEX_INSTANCE_MATRIX_LOCATION + 3, 1);
	}
}


#endif	// GRAPHICS_MESH_HPP