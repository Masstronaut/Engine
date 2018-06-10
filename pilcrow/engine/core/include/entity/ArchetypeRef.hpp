#pragma once
#include "EntityRef.hpp"

class ArchetypeRef : private EntityRef {
public:
  ArchetypeRef(const ArchetypeRef &) = default;
  ArchetypeRef(ArchetypeRef &&)      = default;
  ArchetypeRef()                     = delete;
  using EntityRef::EntityRef;
  using EntityRef::operator==;
  using EntityRef::Get;
  using EntityRef::Has;
  using EntityRef::ID;
  using EntityRef::Name;

  template <typename Component, typename... Args>
  Component &Add(Args &&... args);
  template <typename Component>
  void Remove();

  // These friend classes are needed for archetype spawning.
  // They need to access the world this archetype is in
  // to access the data needed to spawn an instance of it.
  friend class Simulation;
  friend class World;

private:
  World *GetWorld() { return m_World; }
  ArchetypeRef(EntityRef er);
};
#include "../../src/entity/ArchetypeRef.inl"
