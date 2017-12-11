#pragma once
#include "EntityRef.hpp"
struct EditorUpdateEvent {
};

struct EntitySpawnedEvent {
  EntitySpawnedEvent( EntityRef er ) : entity( er ) { }
  EntitySpawnedEvent( const EntitySpawnedEvent& ) = default;
  EntityRef entity;
};

struct EntityDeathEvent {
  EntityDeathEvent( EntityRef er ) : entity( er ) { }
  EntityDeathEvent( const EntityDeathEvent& ) = default;
  EntityRef entity;
};