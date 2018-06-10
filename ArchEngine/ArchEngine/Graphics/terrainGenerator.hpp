/*===========================================================================*
 * Arch Engine - "Graphics/terrainGenerator.hpp"                             *
 *                                                                           *
 * Representation of different kinds of terrains. Procedurally generated or  *
 * created with a height map.                                                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 08/06/2018                                                       *
 * Last Modified: 09/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_TERRAIN_GENERATOR_HPP
#define GRAPHICS_TERRAIN_GENERATOR_HPP


#include "graphicsManager.hpp"
#include "primitives.hpp"
#include "stb_image.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>

#include <string>


namespace Graphics {
	class TerrainGenerator {
	public:
		unsigned genHeightMapTerrain(int terrain_width, int terrain_length,
			const std::string& height_map_path);

		// Procedural generation
		void genFaultTerrain();
		void genCirclesTerrain();
		void genMPDTerrain();

	private:
		unsigned char* rgbToGreyScale(unsigned char* image,
			int width, int height, int size_of_pixel);

		void calcNormals(int terrain_width, int terrain_length,
			std::vector<BasicVertex>& vertices);
	};
}


#endif	// GRAPHICS_TERRAIN_GENERATOR_HPP