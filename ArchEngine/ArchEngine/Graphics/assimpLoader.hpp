/*===========================================================================*
 * Arch Engine - "Graphics/assimpLoader.hpp"                                 *
 *                                                                           *
 * Assimp loader implementation for loading several file formats.            *
 * Meshes should be loaded by Assimp for pre-processing only.                *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 13/05/2018                                                       *
 * Last Modified: 04/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_ASSIMP_LOADER_HPP
#define GRAPHICS_ASSIMP_LOADER_HPP


#include "animation.hpp"
#include "graphicsManager.hpp"
#include "materialManager.hpp"
#include "primitives.hpp"
#include "../Utils/serviceLocator.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <string>
#include <vector>


namespace Graphics {
	class AssimpLoader {
	public:
		bool importScene(const std::string& path, aiPostProcessSteps flags,
			std::vector<unsigned>& loaded_meshes_ids);

	private:
		void loadCameras(const aiScene* scene);
		void loadMeshes(const aiScene* scene,
			std::vector<unsigned>& loaded_meshes_ids);
		void loadMaterials(const aiScene* scene);
		void loadAnimations(const aiScene* scene,
			const aiMesh* mesh, unsigned mesh_id);
		void loadLights(const aiScene* scene);

		// Helpers to load meshes based on it's properties.
		// Instanced meshes are exported by the level editor, therefore
		// not loaded by assimp, but by a custom format loader.
		// Returns the added mesh GraphicsManager's id.
		unsigned basicVertexMesh(const aiMesh* mesh,
			unsigned mesh_id, unsigned material_id);
		unsigned normalMappedVertexMesh(const aiMesh* mesh,
			unsigned mesh_id, unsigned material_id);
		unsigned animatedVertexMesh(const aiMesh* mesh,
			unsigned mesh_id, unsigned material_id);
		unsigned animatedNormalMappedVertexMesh(const aiMesh* mesh,
			unsigned mesh_id, unsigned material_id);

		// Helper to load materials based on it's properties.
		bool loadMaterials(const aiMaterial* material,
			aiTextureType type, std::string& path);

		// Stores the current scene path for mesh naming
		std::string m_path;

		// Auxiliary Mesh for importing
		Mesh m_mesh;

		// Auxiliary to associate a mesh with it's
		// GraphicsManager's material id
		static unsigned m_material_base_index;
	};
}


#endif	// GRAPHICS_ASSIMP_LOADER_HPP