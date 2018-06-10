/*===========================================================================*
 * Arch Engine - "Graphics/terrainGenerator.cpp"                             *
 *                                                                           *
 * Representation of different kinds of terrains. Procedurally generated or  *
 * created with a height map.                                                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 08/06/2018                                                       *
 * Last Modified: 09/06/2018                                                 *
 *===========================================================================*/


#include "terrainGenerator.hpp"


using namespace Utils;


namespace Graphics {
	unsigned TerrainGenerator::genHeightMapTerrain(int terrain_width,
		int terrain_length, const std::string& height_map_path) {
		if (terrain_width < 2 || terrain_length < 2) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Terrain width and height must be at least 2 units");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			return 0; // TODO
		}

		std::vector<BasicVertex> vertices;
		std::vector<unsigned> indices;

		int width, height, n_components;

		unsigned char* height_map = stbi_load(height_map_path.c_str(),
			&width, &height, &n_components, 0);

		unsigned char* new_height_map =
			rgbToGreyScale(height_map, width, height, n_components);

		vertices.resize(terrain_width * terrain_length);
		indices.reserve(terrain_width * terrain_length * 6);

		for (int i = 0; i < terrain_width; i++) {
			for (int j = 0; j < terrain_length; j++) {
				int index = j * terrain_width + i;

				// Positions
				vertices[index].position.x = (float)i;
				vertices[index].position.y = 5 *
					new_height_map[index + (n_components - 1)] / 255.0f;
				vertices[index].position.z = (float)j;

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
		delete[] new_height_map;

		Mesh mesh;
		mesh.create("testterrain", 1, vertices, indices);

		return GraphicsManager::getInstance().addMesh(mesh);
	}

	void TerrainGenerator::genFaultTerrain() {

	}

	void TerrainGenerator::genCirclesTerrain() {

	}

	void TerrainGenerator::genMPDTerrain() {

	}

	unsigned char* TerrainGenerator::rgbToGreyScale(unsigned char* image,
		int width, int height, int size_of_pixel) {
		unsigned char* new_image = new unsigned char[width * height];

		for (int i = 0, j = 0; j < width * height; i += size_of_pixel, j++)
			new_image[j] = (char)
				(0.30 * image[i] + 0.59 * image[i + 1] + 0.11 * image[i + 2]);

		return new_image;
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