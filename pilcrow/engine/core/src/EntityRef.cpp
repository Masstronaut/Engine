#pragma once
#include <tuple>
#include <string>
#include "../include/entity/EntityRef.hpp"

#include "../include/World.hpp"


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
    return m_World->GetEntity( m_ID )->Has( component_type );
  }

  bool EntityRef::Has(const std::vector<std::type_index> &components) const {
    const Entity *entity{ m_World->GetEntity(m_ID) };
    if (!entity) return false;
    for (const auto &component : components) {
      if (!entity->Has(component)) return false;
    }
    return true;
  }


  void EntityRef::Kill() {
    m_World->Emit(EntityDeathEvent{ *this });
    m_World->OnNext<FrameEndEvent>([world = m_World, self = *this](const FrameEndEvent&) {
      world->Kill(self);
    });
  }

  EntityID EntityRef::ID( ) const {
    return m_ID;
  }
  
  const std::string& EntityRef::Name( ) const {
    return m_World->GetEntity( m_ID )->Name( );
  }
  
  EntityRef& EntityRef::Name( const std::string &name ) {
    m_World->GetEntity( m_ID )->Name( name );
    return *this;
  }

  EntityRef EntityRef::Clone( ) const {
    Entity* entity{ m_World->GetEntity( m_ID ) };
    if(entity) return entity->Clone( );
    else throw std::range_error("Tried to clone invalid EntityRef - Entity doesn't exist.");
  }

  bool EntityRef::operator==( const EntityRef &rhs ) const {
    return ( m_ID == rhs.m_ID ) && ( m_World == rhs.m_World );
  }

