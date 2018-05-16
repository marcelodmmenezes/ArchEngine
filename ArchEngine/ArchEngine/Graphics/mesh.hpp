/*===========================================================================*
 * Arch Engine - "Graphics/mesh.hpp"                                         *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 16/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#ifndef GRAPHICS_MESH_HPP
#define GRAPHICS_MESH_HPP


#include "primitives.hpp"
#include "../Utils/serviceLocator.hpp"

#include <vector>


namespace Graphics {
	class Mesh {
	public:
		Mesh();
		~Mesh();

		void create(VertexMask mask, const std::vector<Vertex>& vertices);
		void destroy();

	private:
		// OpenGL handle of this mesh's vertex array object
		unsigned vao_id;
	};
}


#endif	// GRAPHICS_MESH_HPP