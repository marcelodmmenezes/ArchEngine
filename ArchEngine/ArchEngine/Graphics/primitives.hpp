/*===========================================================================*
 * Arch Engine - "Graphics/primitives.hpp"                                   *
 *                                                                           *
 * Structs that represents the graphical primitives of ArchEngine.           *
 * (lights, vertices, materials...)                                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_PRIMITIVES_HPP
#define GRAPHICS_PRIMITIVES_HPP


#include <glm/glm.hpp>


namespace Graphics {
	//------------------------------------------------------------------ Lights

	struct DirectionalLight {
		glm::vec3 direction;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool emit_shadows;
	};

	struct PointLight {
		glm::vec3 position;

		// Attenuation properties
		float constant;
		float linear;
		float quadratic;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool emit_shadows;
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;

		// SpotLight radiuses
		float inner_cut_off;
		float outer_cut_off;

		// Attenuation properties
		float constant;
		float linear;
		float quadratic;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool emit_shadows;
	};

	//-------------------------------------------------------------------------

	//------------------------------------------------------------------ Vertex

	// Assigns a bit to represent each vertex attribute.
	// If the vertex contains the related attribute it's bit will be 1,
	// otherwise, false
	enum VertexAttributes {
		VERTEX_POSITION         = 1 << 0,
		VERTEX_NORMAL           = 1 << 1,
		VERTEX_TEXTURE_COORDS   = 1 << 2,
		VERTEX_TANGENT          = 1 << 3,
		VERTEX_BONE_IDS         = 1 << 4,
		VERTEX_BONE_WEIGHT      = 1 << 5,
		VERTEX_INSTANCE_MATRIX  = 1 << 6
	};

	// Bit mask for fast checking for vertex attributes
	typedef short int VertexMask;

	struct Vertex {
		glm::vec3 position;          // Coordinates in model space
		glm::vec3 normal;            // Normal vector
		glm::vec2 texture_coords;    // UV coordinates
		glm::vec3 tangent;           // Coordinates of tangent space
		glm::ivec4 bone_ids;         // Ids for bones (maximum of 4)
		glm::vec4 bone_weights;      // Weight of each bone upon this vertex
		glm::mat4 instance_matrix;   // Model matrix for instanced rendering
	};

	//-------------------------------------------------------------------------

	//---------------------------------------------------------------- Material

	struct Material {

	};

	//-------------------------------------------------------------------------
}


#endif	// GRAPHICS_PRIMITIVES_HPP