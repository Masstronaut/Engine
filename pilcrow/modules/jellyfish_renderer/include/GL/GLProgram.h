#pragma once

// std
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// gl
#include <glad/include/glad.h>

// ours
#include <utils/include/Resource.hpp>

#include "../../include/GL/GLShader.h"

namespace Jellyfish {
class GLProgram : public Resource {
public:
  struct ShaderVariable {
    GLint       location;
    GLint       size;
    GLenum      type;
    std::string name;
  };

  GLProgram(const std::string &name);
  ~GLProgram();

  int ErrorCheck();

  virtual bool        Reloadable() const override;
  virtual std::string Directory() const override;

  unsigned ID() const;
  void     Use(bool use = true) const;

  bool SetUniform(const std::string &name, float x, float y, float z, float w);
  bool SetUniform(const std::string &name, const glm::vec3 &vec);
  bool SetUniform(const std::string &name, const glm::vec4 &vec);
  bool SetUniform(const std::string &name, float val);
  bool SetUniform(const std::string &name, int val);
  bool SetUniform(const std::string &name, const glm::mat4 &mat);

protected:
  const ShaderVariable *GetVariable(const std::string &name) const;
  virtual bool          LoadImpl() final;
  virtual void          UnloadImpl() final;
  bool                  Check() const;

  void GetAttributes();
  void GetUniforms();
  void WarnUniform(const std::string &uniformName) const;

private:
  unsigned                                        m_ProgramID;
  std::vector<GLShader>                           m_shaders;
  std::unordered_map<std::string, ShaderVariable> m_variables;
};  // end class GLProgram

}  // end namespace Jellyfish
