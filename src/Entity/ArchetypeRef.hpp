#pragma once
#include "../EntityRef.hpp"
class ArchetypeRef : public EntityRef {
public:
  ArchetypeRef( const ArchetypeRef& ) = default;
  ArchetypeRef( ) = delete;
  using EntityRef::EntityRef;
  using EntityRef::operator==;

  template<typename Component, typename... Args>
  Component& Add( Args&&... args );
  template<typename Component>
  void Remove( );
  friend class Simulation;
private:
  ArchetypeRef( EntityRef er );
};
#include "ArchetypeRef.inl"
