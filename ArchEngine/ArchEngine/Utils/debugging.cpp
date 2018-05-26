/*===========================================================================*
 * Arch Engine - "Utils/debugging.cpp"                                       *
 *                                                                           *
 * Utility functions for debugging.                                          *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 25/05/2018                                                       *
 * Last Modified: 25/05/2018                                                 *
 *===========================================================================*/


#include "debugging.hpp"


namespace Utils {
	void checkOpenGLErrors(const std::string& info) {
		GLenum err(glGetError());
		std::string error(info);

		while (err != GL_NO_ERROR) {
			switch (err) {
			case GL_INVALID_ENUM:
				error += ": GL_INVALID_ENUM";
				break;
			case GL_INVALID_VALUE:
				error += ": GL_INVALID_VALUE";
				break;
			case GL_INVALID_OPERATION:
				error += ": GL_INVALID_OPERATION";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				error += ": GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			case GL_OUT_OF_MEMORY:
				error += ": GL_OUT_OF_MEMORY";
				break;
			}

			ServiceLocator::getFileLogger()->log<LOG_ERROR>("\n" + error + "\n");
			ServiceLocator::getConsoleLogger()->log<LOG_ERROR>("\n" + error + "\n");

			err = glGetError();
		}
	}

	void printMatrix(const glm::mat3& matrix) {
		std::cout
			<< matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << std::endl
			<< matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << std::endl
			<< matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << std::endl
			<< std::endl;
	}

	void printMatrix(const glm::mat4& matrix) {
		std::cout
			<< matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << " " << matrix[3][0] << std::endl
			<< matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << " " << matrix[3][1] << std::endl
			<< matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << " " << matrix[3][2] << std::endl
			<< matrix[0][3] << " " << matrix[1][3] << " " << matrix[2][3] << " " << matrix[3][3] << std::endl
			<< std::endl;
	}
}