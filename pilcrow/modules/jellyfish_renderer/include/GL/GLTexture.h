#pragma once

// GL
#include <glad/include/glad.h>

// ours
#include <utils/include/Resource.hpp>

#include "../iTexture.h"

namespace Jellyfish {
class GLTexture : public iTexture, public Resource {
public:
  GLTexture(const std::string &name);
  ~GLTexture();

  // Overriding iTexture:
  void        Use(int TextureUnit = 0) const override;
  TextureType Type() const override;
  unsigned    ID() const override;

  // Overriding Resource:
  virtual bool        Reloadable() const override;
  virtual std::string Directory() const override;

  GLTexture &Type(TextureType type);

private:
  // Overriding Resource:
  bool LoadImpl() final;
  void UnloadImpl() final;

  GLint TextureFromData();

  GLuint m_GLuID{};
  GLenum m_Format{};

};  // class GLTexture

}  // namespace Jellyfish
