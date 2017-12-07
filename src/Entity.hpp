#pragma once

#include <unordered_map>
#include <typeindex>
#include "EntityID.hpp"
class World;

class Entity {
public:
  Entity::Entity( World &world, EntityID id = { 0,0 } );
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
  
  template<typename Component, typename... Args>
  Component& Add( Args&&... args );
  template<typename Component>
  void Remove( );

  EntityID ID( ) const;  
  const std::string& Name( ) const;
  Entity& Name( const std::string &name );
  
  friend class World;
private:
  // @@TODO: Refactor this API to be usable for regular people
  // or provide an overload that is.
  EntityID Clone( World &world, Entity &entity ) const;
  void* Get( std::type_index component );
  World &m_World;
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

template<typename Component, typename ...Args>
inline Component & Entity::Add( Args && ...args ) {
  assert( !this->Has<Component>( ) && "An entity may only be associated with a single instance of each component type." );
  EntityID compHandle{ m_World->GetComponentPool<Component>( ).components.emplace( std::forward<Args>( args )... ) };
  m_Components[ std::type_index( typeid( Component ) ) ] = EntityID;
  return this->Get<Component>( );
}

template<typename Component>
inline void Entity::Remove( ) {
  // @@TODO: Set this up to happen after the end of frame event
  auto it{ m_Components.find( std::type_index( typeid( Component ) ) ) };
  if( it != m_Components.end( ) ) {
    m_World->GetComponentPool<Component>( ).components.erase( it->second );
    m_Components.erase( it );
  }
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
