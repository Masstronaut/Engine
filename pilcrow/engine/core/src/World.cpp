#include "../include/World.hpp"
#include "../include/Simulation.hpp"
#include "../include/WorldEvents.hpp"


World::World(const std::string &name)
  : m_Name(name) {
  this->On([&](const EntitySpawnedEvent &event) {
    for (auto &agg : m_Aggregates) agg.OnEntityCreated(event.entity);
  });
  this->On([&](const EntityDeathEvent &event) {
    for (auto &agg : m_Aggregates) agg.OnEntityDestroyed(event.entity);
  });
  this->On([&](const ComponentAddedEvent &event) {
    for (auto &agg : m_Aggregates) agg.OnEntityCreated(event.entity);
  });
  this->On([&](const ComponentRemovedEvent &event) {
    for (auto &agg : m_Aggregates) agg.OnEntityDestroyed(event.entity);
  });
}


void * World::GetComponent(EntityID component, std::type_index ComponentType) {
  auto it{ m_Components.find(ComponentType) };
  if (it != m_Components.end()) {
    return it->second->Get(component);
  }
  return nullptr;
}

Entity* World::GetEntity(EntityID ID) {
  auto it{ m_Entities.find(ID) };
  if(it != m_Entities.end()) return &(*it);
  else return nullptr;
}
const Entity* World::GetEntity(EntityID ID) const {
  auto it{ m_Entities.find(ID) };
  if (it != m_Entities.end()) return &(*it);
  else return nullptr;
}
EntityRef World::Spawn(ArchetypeRef archetype) {
  return Spawn(EntityRef{ archetype.ID(), archetype.GetWorld() });
}
EntityRef World::Spawn(EntityRef archetype) {
  std::string name{ archetype.Name() };
  EntityRef ent{ this->CreateEntity(name) };
  archetype.m_World->m_Entities[archetype.ID()].Clone(*this, m_Entities[ent.ID()]);
  return ent;
}

const std::string& World::Name() const {
  return m_Name;
}

EntityRef World::CreateEntity(const std::string & name) {
  auto ID{ m_Entities.emplace(*this) };
  std::string newname = name;
  m_Entities[ID].Name(newname);
  m_Entities[ID].m_ID = ID;
  return EntityRef(ID, this);
}

ComponentPoolBase * World::GetComponentPool(std::type_index Component) {
  auto it{ m_Components.find(Component) };
  if (it != m_Components.end()) {
    return it->second.get();
  }
  return nullptr;
}

void World::Kill(EntityRef entity) {
  m_Entities.erase(entity.ID());
}

void World::Update(float dt) {
  this->Emit(FrameStartEvent{});
  this->Emit(UpdateEvent{ dt });
  this->Emit(FrameEndEvent{});
}

bool World::operator==(const World &rhs) {
  return (this == &rhs);
}

