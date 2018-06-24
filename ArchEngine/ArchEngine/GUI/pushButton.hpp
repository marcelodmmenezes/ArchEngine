/*===========================================================================*
 * Arch Engine - "GUI/pushButton.hpp"                                        *
 *                                                                           *
 * Clickable button.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 23/06/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
 *===========================================================================*/


#ifndef GUI_PUSH_BUTTON_HPP
#define GUI_PUSH_BUTTON_HPP


#include "../Core/eventManager.hpp"
#include "guiComponent.hpp"
#include "renderableComponent.hpp"
#include "writableComponent.hpp"

#include <string>


namespace GUI {
	class PushButton : public GUIComponent {
	public:
		PushButton();
		PushButton(int button_id,
			unsigned shader_id,
			const glm::vec2& screen_size,
			const glm::vec4& limits,
			unsigned font_id,
			float font_scale,
			const glm::vec3& text_color,
			const std::string& texture_path);
		~PushButton();

		void initialize(int button_id,
			unsigned shader_id,
			const glm::vec2& screen_size,
			const glm::vec4& limits,
			unsigned font_id,
			float font_scale,
			const glm::vec3& text_color,
			const std::string& texture_path);
		
		void update(float delta_time) override;

		void click();

		void setText(const std::string& text);
		std::string getText() const;

		void justifyLeft();
		void centralize();
		void justifyRight();

		//----------------------------------------------------------- Overrides
		void trackMouse() override;
		void untrackMouse() override;
		//---------------------------------------------------------------------

		//------------------------------------------------ Renderable component
		void setRenderColor(const glm::vec4& color);
		void setRenderHoverColor(const glm::vec4& color);
		void setRenderBorderColor(const glm::vec4& color);
		void setRenderBorderWidth(int width);
		void setRenderLimits(const glm::vec4& limits);
		void setRenderProjection(const glm::mat4& projection);

		void removeRenderTexture();
		void removeRenderColor();
		void removeRenderBorder();

		unsigned getRenderTextureId() const;
		glm::vec4 getRenderColor() const;
		glm::vec4 getRenderHoverColor() const;
		glm::vec4 getRenderBorderColor() const;
		int getRenderBorderWidth() const;
		glm::vec4 getRenderLimits() const;
		glm::mat4 getRenderProjection() const;

		bool hasRenderTexture() const;
		bool hasRenderColor() const;
		bool hasRenderBorder() const;
		//---------------------------------------------------------------------

	private:
		void mouseHover(int x, int y) override;
		void mouseOut() override;

		void onWindowResizeEvent(Core::EventPtr e);
		Core::EventListener m_window_resize_listener;
		
		int m_id;

		glm::vec2 m_size;
		glm::vec2 m_screen_size;
		glm::vec4 m_limits;

		glm::vec2 m_mouse_pos;

		int m_font_id;
		float m_font_scale;
		std::string m_text;
		int m_text_width;
		glm::vec2 m_text_pos;
		glm::vec3 m_text_color;
		int m_text_justification; // -1 left, 0 center, 1 right

		glm::vec4 m_body_color;

		RenderableComponent m_renderable;
	};
}


#endif	// GUI_PUSH_BUTTON_HPP