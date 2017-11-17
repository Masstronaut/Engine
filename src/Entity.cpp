#include <typeinfo>

#include "Entity.hpp"
#include "World.hpp"
Entity::Entity( ) {

}
Entity::Entity( const Entity & ) {

}
Entity::Entity( Entity && ) {

}
Entity::~Entity( ) {

}

template<typename Component>
bool Entity::Has( ) const {
  return m_Components.count( typeid( Component ).hash_code( ) ) > 0;
}

template<typename Component>
Component& Entity::Get( ) {

}
template<typename Component>
const Component& Entity::Get( ) const {

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
