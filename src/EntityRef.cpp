#pragma once
#include <tuple>
#include <string>
#include "EntityRef.hpp"

#include "World.hpp"

  bool EntityRef::Has( std::size_t component_hash ) const {
    return m_World->GetEntity( m_ID ).Has( component_hash );
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
  


