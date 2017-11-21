#include <typeinfo>

#include "Entity.hpp"
#include "World.hpp"
Entity::Entity( ) {

}
Entity::Entity( Entity &&rhs )
  : m_Components( std::move( rhs.m_Components ) )
  , m_ID( rhs.m_ID )
  , m_Name( std::move( rhs.m_Name ) ) {
  
}
Entity::~Entity( ) {

}


bool Entity::Has( std::type_index component_type ) {
  return m_Components.count( component_type ) > 0;
}

EntityID Entity::ID( ) const {
  return m_ID;
}

const std::string& Entity::Name( ) const {
  return m_Name;
}
Entity& Entity::Name( const std::string &name ) {
  m_Name = name;
  return *this;
}
