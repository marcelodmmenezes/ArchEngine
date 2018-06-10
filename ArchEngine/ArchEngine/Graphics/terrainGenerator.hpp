/*===========================================================================*
 * Arch Engine - "Graphics/terrainGenerator.hpp"                             *
 *                                                                           *
 * Representation of different kinds of terrains. Procedurally generated or  *
 * created with a height map.                                                *
 *                                                                           *
 * Based in:                                                                 *
 * - (http://www.lighthouse3d.com/opengl/terrain/index.php?introduction)     *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 08/06/2018                                                       *
 * Last Modified: 10/06/2018                                                 *
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
			float block_width, float block_length, float maximum_height,
			const std::string& height_map_path);

		// Procedural generation
		void genFaultTerrain();
		void genCirclesTerrain();
		void genMPDTerrain();

	private:
		void calcNormals(int terrain_width, int terrain_length,
			std::vector<BasicVertex>& vertices);
	};
}


#endif	// GRAPHICS_TERRAIN_GENERATOR_HPP