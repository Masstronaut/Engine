#pragma once

#include <unordered_map>
class Entity;
#include "EntityID.hpp"

class Entity {
public:
  Entity( );
  Entity( const Entity & ) = delete;
  Entity( Entity && );
  ~Entity( );

  template<typename Component>
  bool Has( ) const;

  template<typename Component, typename... Components>
  bool Has( ) const;

  bool Has( std::size_t component_hash );


  template<typename Component>
  Component& Get( );
  template<typename Component>
  const Component& Get( ) const;

  EntityID ID( ) const;
  
  const std::string& Name( ) const;
  Entity& Name( const std::string &name );
  
private:
  std::unordered_map<std::size_t, EntityID> m_Components;
  EntityID m_ID{ 0, 0 };
  std::string m_Name{"Nameless Entity"};
};

template<typename Component>
bool Entity::Has( ) const {
  return m_Components.count( typeid( std::decay_t<Component> ) ) > 0;
}

template<typename Component, typename... Components>
bool Entity::Has( ) const {
  return Has<Component>( ) && Has<Components...>( );
}

template<typename Component>
Component& Entity::Get( ) {

}
template<typename Component>
const Component& Entity::Get( ) const {

}
