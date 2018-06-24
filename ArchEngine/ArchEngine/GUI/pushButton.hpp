/*===========================================================================*
 * Arch Engine - "GUI/pushButton.hpp"                                        *
 *                                                                           *
 * Clickable button.                                                         *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 23/06/2018                                                       *
 * Last Modified: 23/06/2018                                                 *
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
		PushButton(unsigned shader_id,
			const glm::vec2& screen_size,
			const glm::vec4& limits,
			unsigned font_id,
			float font_scale,
			int text_spacing,
			const glm::vec3& text_color,
			const std::string& texture_path);
		~PushButton();

		void initialize(unsigned shader_id,
			const glm::vec2& screen_size,
			const glm::vec4& limits,
			unsigned font_id,
			float font_scale,
			int text_spacing,
			const glm::vec3& text_color,
			const std::string& texture_path);
		
		void update(const glm::mat4& projection);

		void click();

		void setText(const std::string& text);
		std::string getText() const;

		//------------------------------------------------ Renderable component
		void setRenderColor(const glm::vec4& color);
		void setRenderHoverColor(const glm::vec4& color);
		void setRenderBorderColor(const glm::vec4& color);
		void setRenderBorderWidth(int width);
		void setRenderLimits(const glm::vec4& limits);

		void removeRenderTexture();
		void removeRenderColor();
		void removeRenderBorder();

		unsigned getRenderTextureId() const;
		glm::vec4 getRenderColor() const;
		glm::vec4 getRenderHoverColor() const;
		glm::vec4 getRenderBorderColor() const;
		int getRenderBorderWidth() const;
		glm::vec4 getRenderLimits() const;

		bool hasRenderTexture() const;
		bool hasRenderColor() const;
		bool hasRenderBorder() const;
		//---------------------------------------------------------------------

		//-------------------------------------------------- Writable component
		void setTextFont(unsigned id);
		void setTextScale(float scale);
		void setTextPosition(const glm::vec2& pos);
		void setTextColor(const glm::vec3& color);
		void setTextHoverColor(const glm::vec3& color);
		void setTextMaximumSize(const glm::vec2& size);

		unsigned getTextFont() const;
		float getTextScale() const;
		glm::vec2 getTextPosition() const;
		glm::vec3 getTextColor() const;
		glm::vec3 getTextHoverColor() const;
		glm::vec2 getTextMaximumSize() const;
		//---------------------------------------------------------------------

	private:
		void mouseHover();
		void mouseOut();

		std::string m_text;

		RenderableComponent m_renderable;
		WritableComponent m_writable;
	};
}


#endif	// GUI_PUSH_BUTTON_HPP