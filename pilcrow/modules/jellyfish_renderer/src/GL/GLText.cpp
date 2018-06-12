#pragma once

//std
#include <iostream> //cout

//GL
#include <glad/include/glad.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//ours
#include "../../include/GL/GLText.h"
#include "Utils/include/ResourceSettings.h"

namespace Jellyfish
{
	std::string FontPath()
	{
		return std::string(g_ResourcePath) + "Fonts/";
	}

	const GLText::Character& GLText::Get(char c) const
	{
		return m_Glyphs[c - ' '];
	}

	GLText::GLText(const std::string &shader, const std::string &font, int size) : 
		m_Shader(shader),
		m_Size(size) 
	{

		if (FT_Init_FreeType(&m_FT))
		{
			std::cout << "ERROR: Failed to initialize FreeType Library." << std::endl;
		}

		if (FT_New_Face(m_FT, (FontPath() + font).c_str(), 0, &m_Font))
		{
			std::cout << "Error: Freetype failed to load the font \""
				<< font
				<< "\" from the folder \""
				<< FontPath()
				<< "\"."
				<< std::endl;
		}

		FT_Set_Pixel_Sizes(m_Font, 0, m_Size);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (GLubyte c{ ' ' }; c <= '~'; ++c)
		{
			if (FT_Load_Char(m_Font, c, FT_LOAD_RENDER))
			{
				std::cout << "Warning: FreeType failed to load a glyph for '"
					<< (char)c
					<< "' for font face \""
					<< font
					<< "\"."
					<< std::endl;
				continue;
			}

			GLuint texture{ 0 };
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RED,
				m_Font->glyph->bitmap.width,
				m_Font->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				m_Font->glyph->bitmap.buffer
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			m_Glyphs.emplace_back(Character{ texture,
				{ m_Font->glyph->bitmap.width, m_Font->glyph->bitmap.rows },
				{ m_Font->glyph->bitmap_left, m_Font->glyph->bitmap_top },
				(GLuint)m_Font->glyph->advance.x });
		}

		FT_Done_Face(m_Font);
		FT_Done_FreeType(m_FT);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void GLText::Render(const std::string &text, glm::vec2 position, glm::mat4 proj, glm::vec3 color, float scale)
	{
		m_Shader.Use();
		m_Shader.SetUniform("projection", proj);
		m_Shader.SetUniform("textColor", color);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		for (char c : text)
		{
			const Character &ch{ this->Get(c) };
			glm::vec2 pos{ position.x + ch.Bearing.x * scale, position.y - (ch.Size.y - ch.Bearing.y) * scale };
			glm::vec2 size{ ch.Size.x * scale, ch.Size.y * scale };
			GLfloat vertices[6][4] = {
				{ pos.x,          pos.y + size.y,   0.0, 0.0 },
			{ pos.x,          pos.y,            0.0, 1.0 },
			{ pos.x + size.x, pos.y,            1.0, 1.0 },

			{ pos.x,          pos.y + size.y,   0.0, 0.0 },
			{ pos.x + size.x, pos.y,            1.0, 1.0 },
			{ pos.x + size.x, pos.y + size.y,   1.0, 0.0 }
			};
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			position.x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}//end namespace Jellyfish