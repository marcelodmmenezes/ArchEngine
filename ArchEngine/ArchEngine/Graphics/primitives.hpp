/*===========================================================================*
 * Arch Engine - "Graphics/primitives.hpp"                                   *
 *                                                                           *
 * Structs that represents the graphical primitives of ArchEngine.           *
 * (lights, vertices, materials...)                                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 26/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_PRIMITIVES_HPP
#define GRAPHICS_PRIMITIVES_HPP


#include "framebuffer.hpp"

#include <glm/glm.hpp>

#include <climits>
#include <string>


namespace Graphics {
	//------------------------------------------------------------------ Lights

	struct DirectionalLight {
		glm::vec3 direction;

		float shineness;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		// Shadow properties
		bool emit_shadows;
		glm::mat4 projection; // Shadow mapping projection matrix
		glm::mat4 view; // Shadow mapping view matrix
		Framebuffer depth_map; // Holds the depth map texture
	};

	struct PointLight {
		glm::vec3 position;

		float shineness;

		// Attenuation properties
		float constant;
		float linear;
		float quadratic;

		// Color properties
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		// Shadow properties
		bool emit_shadows;
		glm::mat4 projection; // Shadow mapping projection matrix
		glm::mat4 view[6]; // Shadow mapping view matrices
		Framebuffer depth_map; // Holds the depth map texture
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;

		float shineness;

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

		// Shadow properties
		bool emit_shadows;
		glm::mat4 projection; // Shadow mapping projection matrix
		glm::mat4 view; // Shadow mapping view matrix
		Framebuffer depth_map; // Holds the depth map texture
	};

	//-------------------------------------------------------------------------

	//------------------------------------------------------------------ Vertex

	// All types of vertices have their own struct in order to
	// easily specify OpenGL vertex attributes in packed arrays.
	
	struct CompleteVertex {
		glm::vec3 position;          // Coordinates in model space
		glm::vec3 normal;            // Normal vector
		glm::vec2 texture_coords;    // UV coordinates
		glm::vec3 tangent;           // Coordinates of tangent space
		glm::vec3 bitangent;         // Coordinates of tangent space
		glm::ivec4 bone_ids;         // Ids for bones (maximum of 4)
		glm::vec4 bone_weights;      // Weight of each bone upon this vertex
		glm::mat4 instance_matrix;   // Model matrix for instanced rendering
	};

	struct BasicVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
	};

	struct NormalMappedVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	struct AnimatedVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
		glm::ivec4 bone_ids;
		glm::vec4 bone_weights;
	};

	struct InstancedVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
		glm::mat4 instance_matrix;
	};

	struct AnimatedNormalMappedVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::ivec4 bone_ids;
		glm::vec4 bone_weights;
	};

	struct InstancedNormalMappedVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::mat4 instance_matrix;
	};

	struct InstancedAnimatedVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coords;
		glm::ivec4 bone_ids;
		glm::vec4 bone_weights;
		glm::mat4 instance_matrix;
	};

	//-------------------------------------------------------------------------

	//---------------------------------------------------------------- Material

	enum TextureType {
		TEXTURE_DIFFUSE,
		TEXTURE_SPECULAR,
		TEXTURE_AMBIENT,
		TEXTURE_EMISSIVE,
		TEXTURE_HEIGHT,
		TEXTURE_NORMALS,
		TEXTURE_DISPLACEMENT,
		TEXTURE_CUBE_TEXTURE,
		NUMBER_OF_TEXTURE_TYPES
	};

	struct Material {
		Material() {
			for (unsigned i = 0; i < NUMBER_OF_TEXTURE_TYPES; i++)
				textures[i] = UINT_MAX; // Unitialized
		}

		// Each position is a handle to MaterialManager's m_textures vector
		unsigned textures[NUMBER_OF_TEXTURE_TYPES];
	};

	//-------------------------------------------------------------------------
}


#endif	// GRAPHICS_PRIMITIVES_HPP