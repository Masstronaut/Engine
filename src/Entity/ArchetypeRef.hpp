#pragma once
#include "../EntityRef.hpp"
class ArchetypeRef : private EntityRef {
public:
  ArchetypeRef( const ArchetypeRef& ) = default;
  ArchetypeRef( ArchetypeRef&& ) = default;
  ArchetypeRef( ) = delete;
  using EntityRef::EntityRef;
  using EntityRef::operator==;
  using EntityRef::Has;
  using EntityRef::Get;
  using EntityRef::ID;
  using EntityRef::Name;


  template<typename Component, typename... Args>
  Component& Add( Args&&... args );
  template<typename Component>
  void Remove( );
  friend class Simulation;
  friend class World;
private:
  World* GetWorld( ) { return m_World; }
  ArchetypeRef( EntityRef er );

};
#include "ArchetypeRef.inl"
