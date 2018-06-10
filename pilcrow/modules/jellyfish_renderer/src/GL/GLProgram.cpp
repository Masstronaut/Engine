// std
#include <array>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

// glad MUST be included before other GL headers
#include <glad/include/glad.h>

// GL
#include <GL/glu.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// ours
#include "../../include/GL/GLProgram.h"
#include "../../include/GL/GLShader.h"
#include "../../include/Util.h"

namespace Jellyfish {
GLProgram::GLProgram(const std::string &name) : Resource(name), m_ProgramID(0) {
  this->Load();
}

GLProgram::~GLProgram() { this->Unload(); }

int GLProgram::ErrorCheck() {
  GLenum         errCode;
  const GLubyte *errString;
  if((errCode = glGetError()) != GL_NO_ERROR) {
    errString = gluErrorString(errCode);
    std::cout << "An error has occurred: " << errString;
    std::cout << " -- With shader program: " << this->Name()
              << " -- Type: " << this->Extension() << std::endl;
    return -1;
  }
  return 0;
  //__debugbreak( );
}

bool GLProgram::Reloadable() const { return true; }

std::string GLProgram::Directory() const { return ShaderPath(); }

unsigned GLProgram::ID() const { return m_ProgramID; }

void GLProgram::Use(bool use) const {
  if(use) {
    glUseProgram(this->ID());
  } else {
    glUseProgram(0);
  }
}

bool GLProgram::SetUniform(const std::string &name, float x, float y, float z,
                           float w) {
  const ShaderVariable *var{this->GetVariable(name)};
  if(var != nullptr) {
    // this->Use( );
    glUniform4f(var->location, x, y, z, w);
    return true;
  }
  WarnUniform(name);
  return false;
}

bool GLProgram::SetUniform(const std::string &name, const glm::vec3 &vec) {
  const ShaderVariable *var{this->GetVariable(name)};
  if(var != nullptr) {
    this->Use();
    glUniform3f(var->location, vec.x, vec.y, vec.z);
    return true;
  }
  WarnUniform(name);
  return false;
}

bool GLProgram::SetUniform(const std::string &name, const glm::vec4 &vec) {
  const ShaderVariable *var{this->GetVariable(name)};
  if(var != nullptr) {
    this->Use();
    glUniform4f(var->location, vec.x, vec.y, vec.z, vec.w);
    return true;
  }
  WarnUniform(name);
  return false;
}

bool GLProgram::SetUniform(const std::string &name, float val) {
  const ShaderVariable *var{this->GetVariable(name)};
  if(var != nullptr) {
    this->Use();
    glUniform1f(var->location, val);
    return true;
  }
  WarnUniform(name);
  return false;
}

bool GLProgram::SetUniform(const std::string &name, int val) {
  const ShaderVariable *var{this->GetVariable(name)};
  if(var != nullptr) {
    this->Use();
    glUniform1i(var->location, val);
    return true;
  }
  WarnUniform(name);
  return false;
}

bool GLProgram::SetUniform(const std::string &name, const glm::mat4 &mat) {
  const ShaderVariable *var{this->GetVariable(name)};
  if((var != nullptr) && var->type == GL_FLOAT_MAT4) {
    this->Use();
    glUniformMatrix4fv(var->location, 1, GL_FALSE, glm::value_ptr(mat));
    return true;
  }
  WarnUniform(name);
  return false;
}

const GLProgram::ShaderVariable *
GLProgram::GetVariable(const std::string &name) const {
  auto it{m_variables.find(name)};
  if(it != m_variables.end()) { return &(it->second); }
  return nullptr;
}

bool GLProgram::LoadImpl() {
  this->ErrorCheck();
  std::stringstream ss(Data());
  std::string       line;
  while(std::getline(ss, line)) {
    m_shaders.emplace_back(line);
    this->ErrorCheck();
  }

  bool success{true};
  for(auto &it : m_shaders) {
    success &= it.Loaded();
    this->ErrorCheck();
  }

  if(success) {
    m_ProgramID = glCreateProgram();
    this->ErrorCheck();
    for(const auto &shader : m_shaders) {
      glAttachShader(m_ProgramID, shader.ID());
      this->ErrorCheck();
    }
    glLinkProgram(m_ProgramID);
    success = Check();
    this->ErrorCheck();
    if(!success) {
      glDeleteProgram(m_ProgramID);
    } else {
      this->GetAttributes();
      this->ErrorCheck();
      this->GetUniforms();
      this->ErrorCheck();
    }
  } else {
    std::cout << "Failed to load shader program: " << this->Filename() << '.'
              << std::endl;
  }
  return success;
}

void GLProgram::UnloadImpl() {
  glDeleteProgram(m_ProgramID);
  m_ProgramID = 0;
  m_shaders.clear();
  m_variables.clear();
}

bool GLProgram::Check() const {
  int                   success{1};
  std::array<char, 512> info{};
  glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
  if(success == 0) {
    glGetProgramInfoLog(m_ProgramID, 512, nullptr, info.data());
    std::cout << "An error has occurred on the program: " << Filename()
              << ".\nInfo:\n"
              << info.data() << std::endl;
  }

  return success != 0;
}

void GLProgram::GetAttributes() {
  int count{0};
  glGetProgramiv(this->ID(), GL_ACTIVE_ATTRIBUTES, &count);
  std::array<char, 64> buffer{};
  for(unsigned i{0}; static_cast<int>(i) < count; ++i) {
    ShaderVariable sv;
    GLsizei        namelength;
    sv.location = i;
    glGetActiveAttrib(this->ID(), i, static_cast<GLsizei>(buffer.size()),
                      &namelength, &sv.size, &sv.type, buffer.data());
    sv.name              = buffer.data();
    m_variables[sv.name] = sv;
  }
}

void GLProgram::GetUniforms() {
  int count{0};
  glGetProgramiv(this->ID(), GL_ACTIVE_UNIFORMS, &count);
  std::array<char, 64> buffer{};
  for(unsigned i{0}; static_cast<int>(i) < count; ++i) {
    ShaderVariable sv;
    GLsizei        namelength;
    sv.location = i;
    glGetActiveUniform(this->ID(), i, static_cast<GLsizei>(buffer.size()),
                       &namelength, &sv.size, &sv.type, buffer.data());
    sv.name              = buffer.data();
    m_variables[sv.name] = sv;
  }
}

void GLProgram::WarnUniform(const std::string & /*uniformName*/) const {
  // turning off spam for now
  /*
  std::cout << "Warning: Shader program \""
  << Filename( )
  << "\": failed to set uniform \""
  << uniformName
  << "\"."
  << std::endl;
  */
}  // endfunc

}  // end namespace Jellyfish
