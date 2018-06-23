/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.hpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 22/06/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_GRAPHICS_MANAGER_HPP
#define GRAPHICS_GRAPHICS_MANAGER_HPP


#include "../Config/engineMacros.hpp"
#include "../API/entity.hpp"
#include "../Core/eventManager.hpp"
#include "animation.hpp"
#include "debugCamera.hpp"
#include "framebuffer.hpp"
#include "glad_3_3_core.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "materialManager.hpp"
#include "../Script/luaScript.hpp"
#include "../Utils/debugging.hpp"
#include "../Utils/serviceLocator.hpp"
#include "../Utils/timer.hpp"

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

		bool initialize(bool depth_test, bool face_culling,
			bool blending, int view_port[4], const glm::vec4& color,
			float fov, int active_camera);
		bool initializeFromConfigFile(const std::string& path);

		void update(float delta_time);

		void destroy();

		// Memory management helpers
		// Allocates + size space for meshes
		void reserveMeshes(int size);
		// Allocates + size space for materials
		void reserveMaterials(int size);
		// Allocates + size space for animations
		void reserveAnimations(int size);

		unsigned numberOfMeshes();

		// The functions below receives the path of the files which
		// describes the entity to be added and returns the added component
		// handle
		unsigned addCamera(Camera* camera);
		unsigned addShader(const std::string& vs_path,
			const std::string& fs_path);
		unsigned addShader(const std::string& vs_path,
			const std::string& gs_path, const std::string& fs_path);
		unsigned addMesh(const Mesh& mesh);
		unsigned addAnimation(const Animation& animation);
		unsigned addMaterial(const Material& material);
		unsigned addDirectionalLight(const DirectionalLight& light);
		unsigned addPointLight(const PointLight& light);
		unsigned addSpotLight(const SpotLight& light);

		// Helpers
		void setProjectionMatrix(const glm::mat4& matrix);
		glm::mat4 getProjectionMatrix();

		void setActiveCamera(int id);
		Camera* getActiveCamera();

		Shader* getShader(int id);

		void getScreenSize(int& w, int&h);

		// Drawing helpers
		void drawLine(const glm::vec3& from, const glm::vec3& to,
			const glm::vec3& color);
		void drawQuad(const glm::vec4& limits);

		// Removes the component by handle
		void removeCamera(unsigned handle);
		void removeShader(unsigned handle);
		void removeMesh(unsigned handle);
		void removeAnimation(unsigned handle);
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

		void renderDepthMaps();
		void renderScene();
		void bindLights(Shader& shader);
		void bind2DTextures(Shader& shader, unsigned material_id);

		State m_state;

		int m_screen_width;
		int m_screen_height;

		Core::EventListener m_window_size_listener;

		// Hard coded uniform variables for now
		std::string m_texture_names[NUMBER_OF_TEXTURE_TYPES] = {
			"u_texture_diffuse", "u_texture_specular", //"u_texture_ambient",
			"u_texture_specular", // TODO
			"u_texture_emissive", "u_texture_normals", // "u_texture_height",
			"u_texture_normals", // TODO
			"u_texture_displacement", "u_texture_cube_texture"
		};

		//------------------------------------------------ Some drawing helpers
		unsigned m_line_shader;
		unsigned m_line_vao;
		unsigned m_line_vbo;

		unsigned m_quad_shader;
		unsigned m_quad_vao;
		unsigned m_quad_vbo;

		//--------------------------------------------------- Projection matrix
		float m_fov;
		glm::mat4 m_projection;

		//--------------------------------- Post processing effects framebuffer
		Framebuffer m_pp_framebuffer;

		//------------------------------------------------------------- Cameras
		int m_active_camera;
		std::vector<Camera*> m_cameras;

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

		//---------------------------------------------------------- Animations
		std::vector<Animation> m_animations;

		//-------------------------------------------------------------- Lights
		std::vector<DirectionalLight> m_directional_lights;
		std::vector<PointLight> m_point_lights;
		std::vector<SpotLight> m_spot_lights;
	};
}


#endif	// GRAPHICS_GRAPHICS_MANAGER_HPP