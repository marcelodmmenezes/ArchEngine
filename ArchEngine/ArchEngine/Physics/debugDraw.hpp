/*===========================================================================*
 * Arch Engine - "Physics/debugDraw.hpp"                                     *
 *                                                                           *
 * Class responsible to Bullet debugging draws.                              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 11/06/2018                                                       *
 * Last Modified: 11/06/2018                                                 *
 *===========================================================================*/


#include "../Config/engineMacros.hpp"
#include "../Graphics/graphicsManager.hpp"
#include "../Utils/serviceLocator.hpp"

#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>


namespace Physics {	
	class DebugDrawer : public btIDebugDraw {
	public:
		DebugDrawer(unsigned shader_id);
		~DebugDrawer();

		void drawLine(const btVector3& from, const btVector3& to,
			const btVector3& color);
		void drawContactPoint(const btVector3& PointOnB,
			const btVector3& normalOnB, btScalar distance, int lifeTime,
			const btVector3& color);
		void reportErrorWarning(const char* warningString);
		void draw3dText(const btVector3& location, const char* textString);
		void setDebugMode(int debugMode);
		int getDebugMode() const;

		void update();

	private:
		bool m_created;

		unsigned m_shader_id;

		unsigned m_line_vao;
		unsigned m_line_vbo;

		int m_debug_mode;

		std::vector<float> m_lines;
	};
}