/*===========================================================================*
 * Arch Engine - "API/entity.hpp                                             *
 *                                                                           *
 * Component id bag.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 26/05/2018                                                       *
 * Last Modified: 26/05/2018                                                 *
 *===========================================================================*/


#ifndef API_ENTITY_HPP
#define API_ENTITY_HPP


#include <glm/glm.hpp>

#include <string>
#include <vector>


struct Entity {
	unsigned shader;
	std::vector<unsigned> meshes;
	glm::mat4 model_matrix;
};

extern std::vector<Entity> g_entities;



#endif	// API_ENTITY_HPP