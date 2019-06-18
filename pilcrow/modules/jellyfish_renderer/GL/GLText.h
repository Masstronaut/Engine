#pragma once

// std
#include <string>
#include <vector>

// freetype
#include <ft2build.h>
#include FT_FREETYPE_H

// ours
#include <utils/include/Resource.hpp>

#include "GLProgram.h"

#ifdef DEBUG_TEXT_SIZE
#undef DEBUG_TEXT_SIZE
#endif
#define DEBUG_TEXT_SIZE 18

namespace Jellyfish {
class GLText {
public:
  GLText(const std::string &shader = "Text.sprog",
         const std::string &font = "arial.ttf", int size = DEBUG_TEXT_SIZE);

  void Render(const std::string &text, glm::vec2 position, glm::mat4 proj,
              glm::vec3 color, float scale = 1.f);

private:
  struct Character {
    GLuint     TextureID;  // ID handle for the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
  };

  const Character &Get(char c) const;

  GLProgram              m_Shader;
  FT_Library             m_FT{};
  FT_Face                m_Font{};
  int                    m_Size;
  std::vector<Character> m_Glyphs;
  GLuint                 VAO{0}, VBO{0};
};

}  // namespace Jellyfish
