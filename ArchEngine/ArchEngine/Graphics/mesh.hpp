/*===========================================================================*
 * Arch Engine - "Graphics/mesh.hpp"                                         *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 02/06/2018                                                 *
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

		void create(const std::string& name, unsigned material_id,
			const std::vector<CompleteVertex>& vertices,
			const std::vector<unsigned>& indices);

		void create(const std::string& name, unsigned material_id,
			const std::vector<BasicVertex>& vertices,
			const std::vector<unsigned>& indices);

		void create(const std::string& name, unsigned material_id,
			const std::vector<NormalMappedVertex>& vertices,
			const std::vector<unsigned>& indices);

		void create(const std::string& name, unsigned material_id,
			const std::vector<AnimatedVertex>& vertices,
			const std::vector<unsigned>& indices);

		void create(const std::string& name, unsigned material_id,
			const std::vector<InstancedVertex>& vertices,
			const std::vector<unsigned>& indices);

		void create(const std::string& name, unsigned material_id,
			const std::vector<AnimatedNormalMappedVertex>& vertices,
			const std::vector<unsigned>& indices);

		void create(const std::string& name, unsigned material_id,
			const std::vector<InstancedNormalMappedVertex>& vertices,
			const std::vector<unsigned>& indices);

		void create(const std::string& name, unsigned material_id,
			const std::vector<InstancedAnimatedVertex>& vertices,
			const std::vector<unsigned>& indices);

		void draw();
		void instancedDraw(int amount);

		void destroy();

		std::string m_name;
		unsigned m_material_id;

	protected:
		// Helper methods for vertex specification
		template<typename T>
		void specifyPosition(unsigned location);
		template<typename T>
		void specifyNormal(unsigned location);
		template<typename T>
		void specifyTextureCoords(unsigned location);
		template<typename T>
		void specifyTangent(unsigned location);
		template<typename T>
		void specifyBitangent(unsigned location);
		template<typename T>
		void specifyBoneIds(unsigned location);
		template<typename T>
		void specifyBoneWeights(unsigned location);
		template<typename T>
		void specifyInstancedMatrix(unsigned location);

		// OpenGL handles of this mesh
		unsigned m_vao_id;
		unsigned m_vbo_id;
		unsigned m_ebo_id;

		unsigned m_number_of_indices;
	};

	template<typename T>
	void Mesh::specifyPosition(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, position));
	}

	template<typename T>
	void Mesh::specifyNormal(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, normal));
	}

	template<typename T>
	void Mesh::specifyTextureCoords(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 2, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, texture_coords));
	}

	template<typename T>
	void Mesh::specifyTangent(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, tangent));
	}

	template<typename T>
	void Mesh::specifyBitangent(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, bitangent));
	}

	template<typename T>
	void Mesh::specifyBoneIds(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribIPointer(location, 4, GL_INT,
			sizeof(T), (void*)offsetof(T, bone_ids));
	}

	template<typename T>
	void Mesh::specifyBoneWeights(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, bone_weights));
	}

	template<typename T>
	void Mesh::specifyInstancedMatrix(unsigned location) {
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)offsetof(T, instance_matrix));

		glEnableVertexAttribArray(location + 1);
		glVertexAttribPointer(location + 1, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)(offsetof(T, instance_matrix) +
				sizeof(glm::vec4)));

		glEnableVertexAttribArray(location + 2);
		glVertexAttribPointer(location + 2, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)(offsetof(T, instance_matrix) +
				2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(location + 3);
		glVertexAttribPointer(location + 3, 4, GL_FLOAT,
			GL_FALSE, sizeof(T), (void*)(offsetof(T, instance_matrix) +
				3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(location, 1);
		glVertexAttribDivisor(location + 1, 1);
		glVertexAttribDivisor(location + 2, 1);
		glVertexAttribDivisor(location + 3, 1);
	}
}


#endif	// GRAPHICS_MESH_HPP