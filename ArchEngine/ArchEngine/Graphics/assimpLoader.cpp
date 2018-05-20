/*===========================================================================*
 * Arch Engine - "Graphics/assimpLoader.cpp"                                 *
 *                                                                           *
 * Assimp loader implementation for loading several file formats.            *
 * Meshes should be loaded by Assimp for pre-processing only.                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 20/05/2018                                                 *
 *===========================================================================*/


#include "assimpLoader.hpp"


using namespace Utils;


namespace Graphics {
	bool AssimpLoader::importScene(const std::string& path,
		aiPostProcessSteps flags) {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path, flags);

		if (!scene) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_ERROR
			ServiceLocator::getFileLogger()->log<LOG_ERROR>(
				"Assimp could not load " + path);
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_ERROR

			return false;
		}

		if (scene->HasCameras())
			loadCameras(scene);

		if (scene->HasMeshes())
			loadMeshes(scene);

		if (scene->HasMaterials())
			loadMaterials(scene);

		if (scene->HasAnimations())
			loadAnimations(scene);

		if (scene->HasLights())
			loadLights(scene);

		return true;
	}

	void AssimpLoader::loadCameras(const aiScene* scene) {
		// TODO
	}

	void AssimpLoader::loadMeshes(const aiScene* scene) {
		for (unsigned i = 0; i < scene->mNumMeshes; i++) {
			// A mesh should contain at least position,
			// normal, texture_coords and faces.
			if (!scene->mMeshes[i]->HasPositions() ||
				!scene->mMeshes[i]->HasNormals() ||
				!scene->mMeshes[i]->HasTextureCoords(0) ||
				!scene->mMeshes[i]->HasFaces()) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
				ServiceLocator::getFileLogger()->log<LOG_WARNING>(
					std::string(scene->mMeshes[i]->mName.C_Str()) +
					" is incomplete");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_WARNING
				continue;
			}

			if (scene->mMeshes[i]->HasTangentsAndBitangents() &&
				scene->mMeshes[i]->HasBones())
				animatedNormalMappedVertexMesh(scene->mMeshes[i],
					scene->mMeshes[i]->mMaterialIndex);
			else if (scene->mMeshes[i]->HasTangentsAndBitangents())
				normalMappedVertexMesh(scene->mMeshes[i],
					scene->mMeshes[i]->mMaterialIndex);
			else if (scene->mMeshes[i]->HasBones())
				animatedVertexMesh(scene->mMeshes[i],
					scene->mMeshes[i]->mMaterialIndex);
			else
				basicVertexMesh(scene->mMeshes[i],
					scene->mMeshes[i]->mMaterialIndex);
		}
	}

	void AssimpLoader::loadMaterials(const aiScene* scene) {
		// TODO
	}

	void AssimpLoader::loadAnimations(const aiScene* scene) {
		// TODO
	}

	void AssimpLoader::loadLights(const aiScene* scene) {
		// TODO
	}

	void AssimpLoader::basicVertexMesh(const aiMesh* mesh,
		unsigned material_id) {
		std::vector<BasicVertex> vertices(mesh->mNumVertices);
		std::vector<unsigned int> indices(mesh->mNumFaces);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			const aiVector3D* p_pos = &(mesh->mVertices[i]);
			const aiVector3D* p_normal = &(mesh->mNormals[i]);
			const aiVector3D* p_tex_coord = &(mesh->mTextureCoords[0][i]);

			vertices.push_back(std::move(BasicVertex{
				glm::vec3(p_pos->x, p_pos->y, p_pos->z),
				glm::vec3(p_normal->x, p_normal->y, p_normal->z),
				glm::vec2(p_tex_coord->x, p_tex_coord->y)}));
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
			assert(mesh->mFaces[i].mNumIndices == 3);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		Mesh arch_mesh;
		arch_mesh.create(mesh->mName.C_Str(), material_id, vertices, indices);

		GraphicsManager::getInstance().addMesh(arch_mesh);
	}

	void AssimpLoader::normalMappedVertexMesh(const aiMesh* mesh,
		unsigned material_id) {
		// TODO
	}

	void AssimpLoader::animatedVertexMesh(const aiMesh* mesh,
		unsigned material_id) {
		// TODO
	}

	void AssimpLoader::animatedNormalMappedVertexMesh(const aiMesh* mesh,
		unsigned material_id) {
		// TODO
	}
}