#pragma once
#include <tuple>
#include <string>
#include "EntityRef.hpp"

#include "World.hpp"


EntityRef::EntityRef( EntityID ID, World* world ) 
  : m_ID( ID )
  , m_World( world ) {
}

EntityRef & EntityRef::operator=( const EntityRef & rhs ) {
  m_ID = rhs.m_ID; 
  m_World = rhs.m_World;
  return *this;
}

  bool EntityRef::Has( std::type_index component_type ) const {
    return m_World->GetEntity( m_ID ).Has( component_type );
  }


  EntityID EntityRef::ID( ) const {
    return m_ID;
  }
  
  const std::string& EntityRef::Name( ) const {
    return m_World->GetEntity( m_ID ).Name( );
  }
  
  EntityRef& EntityRef::Name( const std::string &name ) {
    m_World->GetEntity( m_ID ).Name( name );
    return *this;
  }

  EntityRef EntityRef::Clone( ) const {
    Entity& entity{ m_World->GetEntity( m_ID ) };
    return entity.Clone( );
  }

  bool EntityRef::operator==( const EntityRef &rhs ) const {
    return ( m_ID == rhs.m_ID ) && ( m_World == rhs.m_World );
  }

