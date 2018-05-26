/*===========================================================================*
 * Arch Engine - "Utils/debugging.hpp"                                       *
 *                                                                           *
 * Utility functions for debugging.                                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/05/2018                                                       *
 * Last Modified: 25/05/2018                                                 *
 *===========================================================================*/


#ifndef UTILS_DEBUGGING_HPP
#define UTILS_DEBUGGING_HPP


#include "../Graphics/glad_3_3_core.hpp"
#include "serviceLocator.hpp"

#include <glm/glm.hpp>

#include <string>


namespace Utils {
	void checkOpenGLErrors(const std::string& info);
	void printMatrix(const glm::mat3& matrix);
	void printMatrix(const glm::mat4& matrix);
}


#endif	// UTILS_DEBUGGING_HPP