/*===========================================================================*
 * Arch Engine - "Graphics/assimpLoader.hpp"                                 *
 *                                                                           *
 * Assimp loader implementation for loading several file formats.            *
 * Meshes should be loaded by Assimp for pre-processing only.                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 13/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_ASSIMP_LOADER_HPP
#define GRAPHICS_ASSIMP_LOADER_HPP


#include "../Utils/serviceLocator.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <string>


namespace Graphics {
	class AssimpLoader {
	public:
		bool importScene(const std::string& path, aiPostProcessSteps flags);



	private:
	};
}


#endif	// GRAPHICS_ASSIMP_LOADER_HPP