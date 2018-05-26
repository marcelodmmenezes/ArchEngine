/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.hpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 26/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_GRAPHICS_MANAGER_HPP
#define GRAPHICS_GRAPHICS_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "../Core/eventManager.hpp"
#include "debugCamera.hpp"
#include "framebuffer.hpp"
#include "glad_3_3_core.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "materialManager.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/debugging.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <stack>
#include <string>
#include <vector>


 //--------------------------
 //--------- Lua Graphics API
 //--------------------------
extern "C" {
	int addMesh(lua_State* lua);
	int removeMesh(lua_State* lua);
}
//-------------------------------------------------------------------------


namespace Graphics {
	class GraphicsManager {
	public:
		~GraphicsManager();

		GraphicsManager(const GraphicsManager&) = delete;
		void operator=(const GraphicsManager&) = delete;

		static GraphicsManager& getInstance();

		bool initialize(const glm::vec4& color, int width, int height);
		bool initializeFromConfigFile(const std::string& path);

		void update(float delta_time);

		void destroy();

		// Memory management helpers
		// Allocates + size space for meshes
		void reserveMeshes(int size);
		// Allocates + size space for materials
		void reserveMaterials(int size);

		// The functions below receives the path of the files which
		// describes the entity to be added and returns the added component
		// handle
		unsigned addCamera(const DebugCamera& camera);
		unsigned addShader(const std::string& vs_path,
			const std::string& fs_path);
		unsigned addShader(const std::string& vs_path,
			const std::string& gs_path, const std::string& fs_path);
		unsigned addMesh(const Mesh& mesh);
		unsigned addMaterial(const Material& material);
		unsigned addDirectionalLight(const DirectionalLight& light);
		unsigned addPointLight(const PointLight& light);
		unsigned addSpotLight(const SpotLight& light);

		// Getters
		DebugCamera* getActiveCamera();

		// Removes the component by handle
		void removeCamera(unsigned handle);
		void removeShader(unsigned handle);
		void removeMesh(unsigned handle);
		void removeMaterial(unsigned handle);
		void removeDirectionalLight(unsigned handle);
		void removePointLight(unsigned handle);
		void removeSpotLight(unsigned handle);

	private:
		enum State {
			CONSTRUCTED,
			INITIALIZED,
			SAFE_TO_DESTROY
		};

		GraphicsManager();
		void onWindowResizeEvent(Core::EventPtr e);

		State m_state;

		Core::EventListener m_window_size_listener;
		
		void bindLights(Shader& shader);
		void drawMeshes(Shader& shader, bool draw_textures);
		void bind2DTextures(Shader& shader, unsigned mesh_id);
		void unbind2DTextures(Shader& shader);

		// Hard coded uniform variables for now
		std::string m_texture_names[NUMBER_OF_TEXTURE_TYPES] = {
			"u_texture_diffuse", "u_texture_specular", "u_texture_ambient",
			"u_texture_emissive", "u_texture_height", "u_texture_normals",
			"u_texture_displacement", "u_texture_cube_texture"
		};
		
		//--------------------------------------------------- Projection matrix
		glm::mat4 m_projection;

		//------------------------------------------------------------- Cameras
		int m_active_camera;
		std::vector<DebugCamera> m_cameras;

		//------------------------------------------------------------- Shaders
		std::vector<Shader> m_shaders;

		//-------------------------------------------------------------- Meshes
		// All the game meshes are stored here.
		// - The pair tells how many game entities reference a given mesh.
		//   When it's reference count reaches 0 the mesh is marked as removed.
		//   New meshes can be added at the new empty position.
		std::vector<std::pair<Mesh, unsigned>> m_meshes;

		// Map used to reference a mesh by its path. Used internally by the
		// manager to control how meshes are added and removed.
		std::map<std::string, unsigned> m_mesh_name_to_handle;

		// Stack used to store unused spaces at m_meshes.
		std::stack<unsigned> m_meshes_unused_spaces;

		//----------------------------------------------------------- Materials
		std::vector<Material> m_materials;

		//-------------------------------------------------------------- Lights
		std::vector<DirectionalLight> m_directional_lights;
		std::vector<PointLight> m_point_lights;
		std::vector<SpotLight> m_spot_lights;
	};
}


#endif	// GRAPHICS_GRAPHICS_MANAGER_HPP