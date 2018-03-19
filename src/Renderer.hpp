#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLProgram.hpp"

class GLTextRenderer {
public:
  GLTextRenderer( const std::string &shader = "Text.sprog", const std::string &font = "arial.ttf", int size = 22);
  void Render( const std::string &text, glm::vec2 position, glm::mat4 proj, glm::vec3 color, float scale = 1.f );

private:
  struct Character {
    GLuint TextureID;      // ID handle for the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
  };
  const Character& Get( char c ) const {
    return m_Glyphs[ c - ' ' ];
  }
  GLProgram m_Shader;
  FT_Library m_FT;
  FT_Face m_Font;
  int m_Size;
  std::vector<Character> m_Glyphs;
  GLuint VAO{ 0 }, VBO{ 0 };
};