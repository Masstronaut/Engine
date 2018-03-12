#pragma once
#include "../EntityRef.hpp"
#include "../World.hpp"
template<typename Component>
bool EntityRef::Has( ) const {
  return Has<Component>( std::type_index( typeid( Component ) ) );
}
template<typename Component, typename... Components>
bool EntityRef::Has( ) const {
  return Has<Component>( ) && Has<Components...>( );
}

template<typename Component>
inline Component& EntityRef::Get( ) {
  return m_World->GetEntity( m_ID ).Get<Component>( );
}

template<typename Component>
inline const Component& EntityRef::Get( ) const {
  return m_World->GetEntity( m_ID ).Get<Component>( );
}