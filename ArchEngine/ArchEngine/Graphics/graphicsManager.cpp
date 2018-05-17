/*===========================================================================*
 * Arch Engine - "Graphics/graphicsManager.cpp"                              *
 *                                                                           *
 * TODO: description                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 12/05/2018                                                       *
 * Last Modified: 16/05/2018                                                 *
 *===========================================================================*/


#include "graphicsManager.hpp"


using namespace Script;
using namespace Utils;


namespace Graphics {
	GraphicsManager::GraphicsManager() : m_state(CONSTRUCTED) {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GraphicsManager::~GraphicsManager() {
#ifndef ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
		ServiceLocator::getFileLogger()->log<LOG_DEBUG>(
			"GraphicsManager constructor");
#endif	// ARCH_ENGINE_LOGGER_SUPPRESS_DEBUG
	}

	GraphicsManager& GraphicsManager::getInstance() {
		static GraphicsManager instance;
		return instance;
	}

	bool GraphicsManager::initialize(const glm::vec4& color) {
		// TODO
		glClearColor(color.r, color.g, color.b, color.a);

		//---------------------------------------------------------------- TEST
		std::vector<Vertex> vertices;
		Vertex vertex;

		vertex.position = glm::vec3(-1.0f, -1.0f,  1.0f);
		vertices.push_back(vertex);
		vertex.position = glm::vec3( 1.0f, -1.0f,  1.0f);
		vertices.push_back(vertex);
		vertex.position = glm::vec3( 1.0f,  1.0f,  1.0f);
		vertices.push_back(vertex);
		vertex.position = glm::vec3(-1.0f,  1.0f,  1.0f);
		vertices.push_back(vertex);
		vertex.position = glm::vec3(-1.0f, -1.0f, -1.0f);
		vertices.push_back(vertex);
		vertex.position = glm::vec3( 1.0f, -1.0f, -1.0f);
		vertices.push_back(vertex);
		vertex.position = glm::vec3( 1.0f,  1.0f, -1.0f);
		vertices.push_back(vertex);
		vertex.position = glm::vec3(-1.0f,  1.0f, -1.0f);
		vertices.push_back(vertex);

		std::vector<unsigned> indices = {
			0, 1, 2,
			2, 3, 0,
			3, 2, 6,
			6, 7, 3,
			7, 6, 5,
			5, 4, 7,
			4, 5, 1,
			1, 0, 4,
			4, 0, 3,
			3, 7, 4,
			1, 5, 6,
			6, 2, 1,
		};

		mesh.create(VERTEX_POSITION, vertices, indices, 0);

		shader.initialize("../../ArchEngine/Testers/simplevs.glsl",
			"../../ArchEngine/Testers/simplefs.glsl");
		//---------------------------------------------------------------------

		return true;
	}

	bool GraphicsManager::initializeFromConfigFile(const std::string& path) {
		// TODO
		return initialize(glm::vec4(0.05f, 0.08f, 0.07f, 1.0f));
	}

	void GraphicsManager::update(float delta_time) {
		// TODO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//---------------------------------------------------------------- TEST
		shader.bind();

		shader.setMat4("u_model", glm::mat4(1.0f));
		shader.setMat4("u_view", glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		shader.setMat4("u_projection", glm::perspective(
			glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f));
		shader.update();

		mesh.draw();
		Shader::unbindShaders();
		//---------------------------------------------------------------------
	}

	void GraphicsManager::destroy() {
		// TODO
	}
}