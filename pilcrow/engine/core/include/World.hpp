#pragma once
#include <algorithm>
#include <functional>
#include <slot_map.hpp>
#include <string>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <vector>

#include "entity/EntityID.hpp"
#include <utils/include/EventArena.hpp>
#include <utils/include/type_list.hpp>

class ArchetypeRef;
class SystemBase;
class Entity;
class EntityRef;
class ComponentPoolBase;
template <typename... Args>
class EntitiesWith;
template <typename T>
class ComponentPool;
class ComponentAggregate;

class World : public EventArena {
public:
  World(std::string name = "Nameless World");

  template <typename T>
  T &GetComponent(EntityID entity);
  template <typename T>
  const T &GetComponent(EntityID entity) const;
  void *   GetComponent(EntityID component, std::type_index ComponentType);

  Entity *      GetEntity(EntityID ID);
  const Entity *GetEntity(EntityID ID) const;
  EntityRef     Spawn(ArchetypeRef archetype);
  EntityRef     Spawn(EntityRef archetype);

  template <typename T, typename... Args>
  void AddSystem(const std::string &name = "Nameless System", Args &&... args);

  template <typename... Args>
  ComponentAggregate &GetAggregate();
  template <typename... Args>
  const ComponentAggregate &GetAggregate() const;

  template <typename... Args>
  void RegisterEntitiesWith(EntitiesWith<Args...> &ew);

  void Update(float dt);

  bool               operator==(const World &rhs);
  const std::string &Name() const;

protected:
  template <typename Component>
  friend class ComponentPool;
  friend class Simulation;
  friend class Entity;

  EntityRef CreateEntity(const std::string &name);
  template <typename T>
  ComponentPool<T> &GetComponentPool();
  template <typename T>
  const ComponentPool<T> &GetComponentPool() const;
  ComponentPoolBase *     GetComponentPool(std::type_index Component);
  template <typename... Args>
  ComponentAggregate &GetAggregate(type_list<Args...>);

private:
  friend class EntityRef;
  void        Kill(EntityRef entity);
  std::string m_Name;
  std::unordered_map<std::type_index, std::unique_ptr<ComponentPoolBase>>
                                           m_Components;
  slot_map<Entity>                         m_Entities;
  std::vector<ComponentAggregate>          m_Aggregates;
  std::vector<std::unique_ptr<SystemBase>> m_Systems;
};
#include "../src/World.inl"
