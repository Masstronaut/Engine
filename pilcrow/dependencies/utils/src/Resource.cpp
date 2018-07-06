#include "../include/Resource.hpp"
#include <algorithm>
#include <array>
#include <cstdio>  // EOF
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
Resource::Resource(const std::string &name)
  : m_name(name.substr(0, name.find_last_of(".")))
  , m_extension(name.substr(name.find_last_of(".") + 1,
                            (name.length() - m_name.length()) + 1)) {}
Resource::Resource(const Resource &resource)
  : m_loaded(false)
  , m_name(resource.m_name)
  , m_extension(resource.m_extension)
  , m_data(resource.m_data) {}
Resource::Resource(Resource &&resource)
  : m_loaded(resource.m_loaded)
  , m_name(std::move(resource.m_name))
  , m_extension(std::move(resource.m_extension))
  , m_data(std::move(resource.m_data)) {
  resource.m_loaded = false;
}
Resource::~Resource() {}

std::string Resource::Path() const { return Directory() + this->Filename(); }
const std::string &Resource::Name() const { return m_name; }
const std::string &Resource::Extension() const { return m_extension; }
std::string        Resource::Filename() const {
  return this->Name() + '.' + this->Extension();
}
const std::string &Resource::Data() const { return m_data; }
bool               Resource::Save(IOType iotype) const {
  std::ofstream outfile(this->Path(), std::ios_base::trunc);
  if(!outfile.good()) return false;
  outfile << this->Data();
  return true;
}
bool Resource::Loaded() const { return m_loaded; }

bool Resource::Load(IOType iotype) {
  if(this->Loaded()) { return true; }
  if(this->Data().size() || this->Read(iotype)) {
    if(this->LoadImpl()) {
      m_loaded = true;
      return this->Loaded();
    }
  }
  return false;
}
void Resource::Unload() {
  if(this->Loaded()) {
    this->UnloadImpl();
    m_data.clear();
    m_loaded = false;
  }
}
bool Resource::Reload() {
  if(!this->Reloadable()) return false;
  this->Unload();
  return this->Load();
}

bool Resource::Read(IOType iotype) {
  std::ios_base::openmode mode{std::ios_base::in};
  if(iotype == IOType::binary) mode |= std::ios_base::binary;
  std::ifstream infile(this->Path(), mode);
  if(!infile.good()) {
    std::cout << "Failed to read resource file \"" << this->Path()
              << "\" from disk." << std::endl;
    return false;
  }
  m_data.clear();
  infile.seekg(0, std::ios_base::end);
  m_data.reserve(infile.tellg());
  infile.seekg(0);
  std::stringstream ss;
  ss << infile.rdbuf();
  m_data = ss.str();
  return true;
}