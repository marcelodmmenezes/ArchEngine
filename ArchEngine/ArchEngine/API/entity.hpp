/*===========================================================================*
 * Arch Engine - "API/entity.hpp                                             *
 *                                                                           *
 * Component id bag.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 26/05/2018                                                       *
 * Last Modified: 07/06/2018                                                 *
 *===========================================================================*/


#ifndef API_ENTITY_HPP
#define API_ENTITY_HPP


#include <glm/glm.hpp>

#include <string>
#include <vector>


struct Entity {
	unsigned shader;
	// Stores the corresponding GraphicsManager's meshes ids
	std::vector<unsigned> meshes;
	// Stores the corresponding PhysicsManager's physics objects ids
	std::vector<unsigned> physics_objects;
	// One for each mesh
	std::vector<glm::mat4> transforms;
};

extern std::vector<Entity> g_entities;


#endif	// API_ENTITY_HPP