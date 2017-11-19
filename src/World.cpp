#pragma once
#include "World.hpp"


World::World( const std::string &name )
  : m_Name( name ) { }


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