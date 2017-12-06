#pragma once

#include <unordered_map>
#include <typeindex>
#include "EntityID.hpp"
class World;

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

  bool Has( std::type_index component_type );


  template<typename Component>
  Component& Get( );
  template<typename Component>
  const Component& Get( ) const;

  EntityID ID( ) const;
  
  const std::string& Name( ) const;
  Entity& Name( const std::string &name );
  
private:
  void* Get( std::type_index component );
  World *m_World{ nullptr };
  std::unordered_map<std::type_index, EntityID> m_Components;
  EntityID m_ID{ 0, 0 };
  std::string m_Name{"Nameless Entity"};
};

template<typename Component>
bool Entity::Has( ) const {
  return m_Components.count( std::type_index( typeid( std::decay_t<Component> ) ) ) > 0;
}

template<typename Component, typename... Components>
bool Entity::Has( ) const {
  return Has<Component>( ) && Has<Components...>( );
}

template<typename Component>
Component& Entity::Get( ) {
  using DecayedType = std::decay_t<Component>;
  constexpr std::type_index TypeIndex = std::type_index( typeid( DecayedType ) );
  return m_World->GetComponent<DecayedType>( m_Components[ TypeIndex ] );
}
template<typename Component>
const Component& Entity::Get( ) const {
  return m_World->GetComponent<std::decay_t<Component>>( m_Components[ std::type_index( typeid( std::decay_t<Component> ) ) ] );
}
