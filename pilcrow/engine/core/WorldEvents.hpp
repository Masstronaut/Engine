#pragma once

#include <typeindex>

#include "pilcrow/engine/core/entity/EntityRef.hpp"

struct EditorUpdateEvent {};

struct EntitySpawnedEvent {
  EntitySpawnedEvent(EntityRef er) : entity(er) {}
  EntitySpawnedEvent(const EntitySpawnedEvent &) = default;
  EntityRef entity;
};

struct EntityDeathEvent {
  EntityDeathEvent(EntityRef er) : entity(er) {}
  EntityDeathEvent(const EntityDeathEvent &) = default;
  EntityRef entity;
};

struct UpdateEvent {
  float Dt = 1.f / 60.f;
};

struct FrameStartEvent {};
struct FrameEndEvent {};

struct ComponentAddedEvent {
  const EntityRef entity;
  std::type_index ComponentType;
};

struct ComponentRemovedEvent {
  const EntityRef entity;
  std::type_index ComponentType;
};