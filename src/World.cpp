#pragma once
#include "World.hpp"


World::World( const std::string &name )
  : m_Name( name ) { }


void * World::GetComponent( EntityID component, std::type_index ComponentType ) {
  auto it{ m_Components.find( ComponentType ) };
  if( it != m_Components.end( ) ) {
    return it->second->Get( component );
  }
  return nullptr;
}

Entity& World::GetEntity( EntityID ID ) {
  return m_Entities[ ID ];
}
const Entity& World::GetEntity( EntityID ID ) const {
  return m_Entities[ ID ];
}

const std::string& World::Name( ) const {
  return m_Name;
}

bool World::operator==( const World &rhs ) {
  return ( this == &rhs );
}