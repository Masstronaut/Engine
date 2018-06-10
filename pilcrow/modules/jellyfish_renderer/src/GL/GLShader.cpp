// std
#include <array>
#include <iostream>

// ours
#include "../../include/GL/GLShader.h"
#include "../../include/Util.h"  //ShaderPath()

namespace Jellyfish {
GLShader::GLShader(const std::string &name) : Resource(name) {
  auto ext{this->Extension()};

  // TODO(unknown): better / automatic extension checking
  if(ext == "vert") {
    m_GLShaderType = GL_VERTEX_SHADER;
  } else if(ext == "frag") {
    m_GLShaderType = GL_FRAGMENT_SHADER;
  }

  this->Load();
}

GLShader::GLShader(GLShader &&shader)
  : Resource(std::move(shader))
  , m_GLuID(shader.m_GLuID)
  , m_GLShaderType(shader.m_GLShaderType) {
  shader.m_GLuID        = 0;
  shader.m_GLShaderType = 0;
}

GLShader::~GLShader() { this->Unload(); }

unsigned GLShader::Type() const { return m_GLShaderType; }

unsigned GLShader::ID() const {
  // implicit cast here
  return m_GLuID;
}

void GLShader::Use() const {
  // TODO(unknown): , not necessary yet
}

bool GLShader::Check() const {
  int                   success{0};
  std::array<char, 512> info{};
  glGetShaderiv(m_GLuID, GL_COMPILE_STATUS, &success);

  if(success == 0) {
    glGetShaderInfoLog(m_GLuID, static_cast<GLsizei>(info.size()), nullptr,
                       info.data());
    std::cout << "Error: Shader compilation has failed for file: "
              << this->Filename() << ".\n"
              << info.data() << std::endl;
  }
  return success != 0;
}

bool GLShader::Reloadable() const { return true; }

std::string GLShader::Directory() const { return ShaderPath(); }

bool GLShader::LoadImpl() {
  m_GLuID = glCreateShader(m_GLShaderType);

  const GLchar *ShaderSrc{Data().c_str()};
  glShaderSource(m_GLuID, 1, &ShaderSrc, nullptr);
  glCompileShader(m_GLuID);

  if(this->Check()) { return true; }

  // else
  glDeleteShader(m_GLuID);
  return false;
}

void GLShader::UnloadImpl() {
  glDeleteShader(m_GLuID);
  m_GLuID = 0;
}

}  // namespace Jellyfish
