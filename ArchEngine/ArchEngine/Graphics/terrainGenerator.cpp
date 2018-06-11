/*===========================================================================*
 * Arch Engine - "Graphics/terrainGenerator.cpp"                             *
 *                                                                           *
 * Representation of different kinds of terrains. Procedurally generated or  *
 * created with a height map.                                                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 08/06/2018                                                       *
 * Last Modified: 10/06/2018                                                 *
 *===========================================================================*/


#include "terrainGenerator.hpp"


using namespace Physics;
using namespace Utils;


namespace Graphics {
	std::pair<unsigned, unsigned> TerrainGenerator::genHeightMapTerrain(
		int terrain_width, int terrain_length, float block_width,
		float block_length, float maximum_height,
		const std::string& height_map_path) {
		if (terrain_width < 2 || terrain_length < 2) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Terrain width and height must be at least 2 units");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return std::make_pair(0, 0); // TODO
		}

		std::vector<BasicVertex> vertices;
		std::vector<unsigned> indices;

		int width, height, n_components, channel_count = 3;

		unsigned char* height_map = stbi_load(height_map_path.c_str(),
			&width, &height, &n_components, channel_count);

		if (!height_map) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Could not load " + height_map_path);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return std::make_pair(0, 0); // TODO
		}

		float max_pixel_height = height_map[0];
		float min_pixel_height = height_map[0];
		int image_size = width * height * channel_count;
		for (int i = 0; i < image_size; i++) {
			if (height_map[i] > max_pixel_height)
				max_pixel_height = height_map[i];
			if (height_map[i] < min_pixel_height)
				min_pixel_height = height_map[i];
		}

		vertices.resize(terrain_width * terrain_length);
		indices.reserve(terrain_width * terrain_length * 6);

		// Steps to cover the whole texture
		int u_step = width / terrain_width;
		int v_step = height / terrain_length;

		for (int i = 0, u = 0; i < terrain_width; i++, u += u_step) {
			for (int j = 0, v = 0; j < terrain_length; j++, v += v_step) {
				int index = j * terrain_width + i;

				// Pixel info
				unsigned byte_per_pixel = channel_count;
				unsigned char* pixel_offset = height_map +
					(v + height * u) * byte_per_pixel;

				// Positions
				vertices[index].position.x = block_width *
					(terrain_width / 2 - i);

				vertices[index].position.y = (0.30 * pixel_offset[0] + 0.59 *
					pixel_offset[1] + 0.11 * pixel_offset[2]);
				vertices[index].position.y -= min_pixel_height;
				vertices[index].position.y /= max_pixel_height;
				vertices[index].position.y *= maximum_height;

				vertices[index].position.z = block_length *
					(terrain_length / 2 - j);

				// Texture Coords
				vertices[index].texture_coords.x = (float)i / terrain_width;
				vertices[index].texture_coords.y = (float)j / terrain_length;
			}
		}

		calcNormals(terrain_width, terrain_length, vertices);

		for (int i = 0; i < terrain_width - 1; i++) {
			for (int j = 0; j < terrain_length - 1; j++) {
				int index = j * terrain_width + i;
				int next_row_index = (j + 1) * terrain_width + i;

				// Indices
				indices.push_back(index);
				indices.push_back(index + 1);
				indices.push_back(next_row_index);

				indices.push_back(next_row_index);
				indices.push_back(index + 1);
				indices.push_back(next_row_index + 1);
			}
		}

		stbi_image_free(height_map);

		Mesh mesh;
		mesh.create("testterrain", 1, vertices, indices);

		return std::make_pair(GraphicsManager::getInstance().addMesh(mesh),
			PhysicsManager::getInstance().addStaticBody(vertices, indices));
	}

	void TerrainGenerator::genFaultTerrain() {
		// TODO
	}

	void TerrainGenerator::genCirclesTerrain() {
		// TODO
	}

	void TerrainGenerator::genMPDTerrain() {
		// TODO
	}
	
	void TerrainGenerator::calcNormals(int terrain_width, int terrain_length,
		std::vector<BasicVertex>& vertices) {
		// Corner normals
		// "0, 0"
		vertices[0].normal = glm::cross(
			vertices[terrain_width].position - vertices[0].position,
				vertices[1].position - vertices[0].position
		);
		// "0, terrain_width - 1"
		vertices[terrain_width - 1].normal = glm::cross(
			vertices[terrain_width - 2].position -
				vertices[terrain_width - 1].position,
			vertices[2 * terrain_width - 1].position -
				vertices[terrain_width - 1].position
		);
		// "terrain_height - 1, 0"
		vertices[(terrain_length - 1) * terrain_width].normal = glm::cross(
			vertices[(terrain_length - 1) * terrain_width + 1].position -
				vertices[(terrain_length - 1) * terrain_width].position,
			vertices[(terrain_length - 2) * terrain_width].position -
				vertices[(terrain_length - 1) * terrain_width].position
		);
		// "terrain_width - 1, terrain_height - 1"
		vertices[terrain_length * terrain_width - 1].normal = glm::cross(
			vertices[terrain_length * (terrain_width - 1) - 1].position -
				vertices[terrain_length * terrain_width - 1].position,
			vertices[terrain_length * terrain_width - 2].position -
				vertices[terrain_length * terrain_width - 1].position
		);
		
		// Border normals
		for (int i = 1; i < terrain_width - 1; i++) {
			vertices[i].normal = glm::mix(
				glm::cross(
					vertices[i - 1].position -
						vertices[i].position,
					vertices[terrain_width + i].position -
						vertices[i].position
				),
				glm::cross(
					vertices[terrain_width + i].position -
						vertices[i].position,
					vertices[i + 1].position -
						vertices[i].position
				),
				0.5f
			);
			
			int row_index = (terrain_length - 1) * terrain_width;
			int previous_row_index = (terrain_length - 2) * terrain_width;

			vertices[row_index + i].normal = glm::mix(
				glm::cross(
					vertices[row_index + i + 1].position -
						vertices[row_index + i].position,
					vertices[previous_row_index + i].position -
						vertices[row_index + i].position
				),
				glm::cross(
					vertices[previous_row_index + i].position -
						vertices[row_index + i].position,
					vertices[row_index + i - 1].position -
						vertices[row_index + i].position
				),
				0.5f
			);
		}
		
		for (int i = 1; i < terrain_length - 1; i++) {
			vertices[terrain_width * i].normal = glm::mix(
				glm::cross(
					vertices[terrain_width * (i + 1)].position -
						vertices[terrain_width * i].position,
					vertices[terrain_width * i + 1].position -
						vertices[terrain_width * i].position
				),
				glm::cross(
					vertices[terrain_width * i + 1].position -
						vertices[terrain_width * i].position,
					vertices[terrain_width * (i - 1)].position -
						vertices[terrain_width * i].position
				),
				0.5f
			);

			vertices[terrain_width * (i + 1) - 1].normal = glm::mix(
				glm::cross(
					vertices[terrain_width * i - 1].position -
						vertices[terrain_width * (i + 1) - 1].position,
					vertices[terrain_width * (i + 1) - 2].position -
						vertices[terrain_width * (i + 1) - 1].position
				),
				glm::cross(
					vertices[terrain_width * (i + 1) - 2].position -
						vertices[terrain_width * (i + 1) - 1].position,
					vertices[terrain_width * (i + 2) - 1].position -
						vertices[terrain_width * (i + 1) - 1].position
				),
				0.5f
			);
		}
		
		// Interior normals
		for (int i = 1; i < terrain_width - 1; i++) {
			for (int j = 1; j < terrain_length - 1; j++) {
				int index = j * terrain_width + i;

				glm::vec3 vec1 = glm::cross(
					vertices[(j - 1) * terrain_width + i].position -
						vertices[index].position,
					vertices[index - 1].position -
						vertices[index].position
				);
				
				glm::vec3 vec2 = glm::cross(
					vertices[index - 1].position -
						vertices[index].position,
					vertices[(j + 1) * terrain_width + i].position -
						vertices[index].position
				);
				
				glm::vec3 vec3 = glm::cross(
					vertices[(j + 1) * terrain_width + i].position -
						vertices[index].position,
					vertices[index + 1].position -
						vertices[index].position
				);
				
				glm::vec3 vec4 = glm::cross(
					vertices[index + 1].position -
						vertices[index].position,
					vertices[(j - 1) * terrain_width + i].position -
						vertices[index].position
				);
				
				vertices[index].normal = glm::mix(
					glm::mix(
						glm::mix(vec1, vec2, 0.5f),
						vec3, 0.5f
					),
					vec4, 0.4f
				);
			}
		}
	}
}