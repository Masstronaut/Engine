#pragma once
#include <tuple>
#include <string>

#include "EntityID.hpp"
class World;
class Entity;

class EntityRef {
public:

  template<typename Component>
  bool Has( ) const;
  template<typename Component, typename... Components>
  bool Has( ) const;

  bool Has( std::size_t component_hash ) const;

  template<typename Component>
  Component& Get( );
  template<typename Component>
  const Component& Get( ) const;

  EntityID ID( ) const;
  
  const std::string& Name( ) const;
  EntityRef& Name( const std::string &name );
  
  bool operator==( const EntityRef &rhs ) {
    return ( m_ID == rhs.m_ID ) && ( m_World == rhs.m_World );
  }
private:
  EntityID m_ID{ 0, 0 };
  World* m_World;

};
template<typename Component>
bool EntityRef::Has( ) const {
  return Has( typeid( Component ).hash_code( ) );
}
template<typename Component, typename... Components>
bool EntityRef::Has( ) const {
  return Has<Component>( ) && Has<Components...>( );
}