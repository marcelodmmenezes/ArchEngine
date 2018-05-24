/*===========================================================================*
 * Arch Engine - "Graphics/assimpLoader.cpp"                                 *
 *                                                                           *
 * Assimp loader implementation for loading several file formats.            *
 * Meshes should be loaded by Assimp for pre-processing only.                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 22/05/2018                                                 *
 *===========================================================================*/


#include "assimpLoader.hpp"


using namespace Utils;


namespace Graphics {
	unsigned AssimpLoader::m_material_base_index = 0u;

	bool AssimpLoader::importScene(const std::string& path,
		aiPostProcessSteps flags) {
		m_path = path;

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

		m_material_base_index += scene->mNumMaterials;

		return true;
	}

	void AssimpLoader::loadCameras(const aiScene* scene) {
		// TODO
	}

	void AssimpLoader::loadMeshes(const aiScene* scene) {
		// Requests more space to GraphicsManager for efficiency
		GraphicsManager::getInstance().reserveMeshes(scene->mNumMeshes);

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
					i, scene->mMeshes[i]->mMaterialIndex);
			else if (scene->mMeshes[i]->HasTangentsAndBitangents())
				normalMappedVertexMesh(scene->mMeshes[i],
					i, scene->mMeshes[i]->mMaterialIndex);
			else if (scene->mMeshes[i]->HasBones())
				animatedVertexMesh(scene->mMeshes[i],
					i, scene->mMeshes[i]->mMaterialIndex);
			else
				basicVertexMesh(scene->mMeshes[i],
					i, scene->mMeshes[i]->mMaterialIndex);
		}
	}

	void AssimpLoader::loadMaterials(const aiScene* scene) {
		// Requests more space to GraphicsManager for efficiency
		GraphicsManager::getInstance().reserveMaterials(scene->mNumMaterials);

		std::string str;
		std::vector<std::pair<std::string, TextureType>> materials;
		materials.reserve(NUMBER_OF_TEXTURE_TYPES);

		for (unsigned i = 0; i < scene->mNumMaterials; i++) {
			if (loadMaterials(scene->mMaterials[i],
				aiTextureType_DIFFUSE, str))
				materials.push_back(std::make_pair(str, TEXTURE_DIFFUSE));

			if (loadMaterials(scene->mMaterials[i],
				aiTextureType_SPECULAR, str))
				materials.push_back(std::make_pair(str, TEXTURE_SPECULAR));

			if (loadMaterials(scene->mMaterials[i],
				aiTextureType_AMBIENT, str))
			materials.push_back(std::make_pair(str, TEXTURE_AMBIENT));

			if (loadMaterials(scene->mMaterials[i],
				aiTextureType_EMISSIVE, str))
			materials.push_back(std::make_pair(str, TEXTURE_EMISSIVE));

			if (loadMaterials(scene->mMaterials[i],
				aiTextureType_HEIGHT, str))
			materials.push_back(std::make_pair(str, TEXTURE_HEIGHT));

			if (loadMaterials(scene->mMaterials[i],
				aiTextureType_NORMALS, str))
			materials.push_back(std::make_pair(str, TEXTURE_NORMALS));

			if (loadMaterials(scene->mMaterials[i],
				aiTextureType_DISPLACEMENT, str))
			materials.push_back(std::make_pair(str, TEXTURE_DISPLACEMENT));

			GraphicsManager::getInstance().addMaterial(
				MaterialManager::getInstance().buildMaterial(
					materials));

			materials.clear();
		}
	}

	void AssimpLoader::loadAnimations(const aiScene* scene) {
		// TODO
	}

	void AssimpLoader::loadLights(const aiScene* scene) {
		// TODO
	}

	void AssimpLoader::basicVertexMesh(const aiMesh* mesh,
		unsigned mesh_id, unsigned material_id) {
		std::vector<BasicVertex> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces);

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

		// Case mesh doesn't have a name
		if (mesh->mName.length == 0)
			m_mesh.m_name = m_path + std::to_string(mesh_id);

		m_mesh.create(m_mesh.m_name,
			m_material_base_index + material_id,
			vertices, indices);

		GraphicsManager::getInstance().addMesh(m_mesh);
	}

	void AssimpLoader::normalMappedVertexMesh(const aiMesh* mesh,
		unsigned mesh_id, unsigned material_id) {
		std::vector<NormalMappedVertex> vertices;
		std::vector<unsigned int> indices;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			const aiVector3D* p_pos = &(mesh->mVertices[i]);
			const aiVector3D* p_normal = &(mesh->mNormals[i]);
			const aiVector3D* p_tex_coord = &(mesh->mTextureCoords[0][i]);
			const aiVector3D* p_tangent = &(mesh->mTangents[i]);
			const aiVector3D* p_bitangent = &(mesh->mBitangents[i]);

			vertices.push_back(std::move(NormalMappedVertex{
				glm::vec3(p_pos->x, p_pos->y, p_pos->z),
				glm::vec3(p_normal->x, p_normal->y, p_normal->z),
				glm::vec2(p_tex_coord->x, p_tex_coord->y),
				glm::vec3(p_tangent->x, p_tangent->y, p_tangent->z),
				glm::vec3(p_bitangent->x, p_bitangent->y, p_bitangent->z)}));
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
#ifndef ARCH_ENGINE_REMOVE_ASSERTIONS
			assert(mesh->mFaces[i].mNumIndices == 3);
#endif	// ARCH_ENGINE_REMOVE_ASSERTIONS

			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		m_mesh.m_name = m_path + std::to_string(mesh_id) + mesh->mName.C_Str();

		m_mesh.create(m_mesh.m_name,
			m_material_base_index + material_id,
			vertices, indices);

		GraphicsManager::getInstance().addMesh(m_mesh);
	}

	void AssimpLoader::animatedVertexMesh(const aiMesh* mesh,
		unsigned mesh_id, unsigned material_id) {
		// TODO
	}

	void AssimpLoader::animatedNormalMappedVertexMesh(const aiMesh* mesh,
		unsigned mesh_id, unsigned material_id) {
		// TODO
	}

	bool AssimpLoader::loadMaterials(const aiMaterial* material,
		aiTextureType type, std::string& path) {
		// TODO
		// Loading only one texture by type. Improve later.
		if (material->GetTextureCount(type) > 0) {
			aiString str;
			material->GetTexture(type, 0, &str);

			int index = m_path.find_last_of('/');
			path = m_path.substr(0, index) + "/" + str.C_Str();

			index = path.find_last_of('.');

			// TODO
			// Stbi is failing to load tga images.
			// Transform all tga to jpg before loading the mesh
			// Fix this later
			if (path.substr(index + 1) == "tga" || path.substr(index + 1) == "TGA")
				path = path.substr(0, index) + ".jpg";

			return true;
		}

		return false;
	}
}