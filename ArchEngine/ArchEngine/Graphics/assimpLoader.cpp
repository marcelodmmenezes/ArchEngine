/*===========================================================================*
 * Arch Engine - "Graphics/assimpLoader.cpp"                                 *
 *                                                                           *
 * Assimp loader implementation for loading several file formats.            *
 * Meshes should be loaded by Assimp for pre-processing only.                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 13/05/2018                                                 *
 *===========================================================================*/


#include "assimpLoader.hpp"


using namespace Utils;


namespace Graphics {
	bool AssimpLoader::importScene(const std::string& path) {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace | aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

		if (!scene) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Assimp could not load " + path);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

			return false;
		}

		// TODO

		return true;
	}
}