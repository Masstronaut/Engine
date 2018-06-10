#pragma once

#include "EntityID.hpp"
#include <string>
#include <typeindex>
#include <unordered_map>
class World;

class Entity {
public:
  Entity(World &world, EntityID id = {0, 0});
  Entity(const Entity &) = delete;
  Entity(Entity &&);
  Entity &operator=(Entity &&);
  ~Entity();

  template <typename Component>
  bool Has() const;
  template <typename Component, typename Component2, typename... Components>
  bool Has() const;
  bool Has(std::type_index component_type) const;
  template <typename Component>
  Component &Get();
  template <typename Component>
  const Component &Get() const;

  template <typename Component, typename... Args>
  Component &Add(Args &&... args);
  template <typename Component>
  void Remove();

  EntityID           ID() const;
  const std::string &Name() const;
  Entity &           Name(const std::string &name);

  friend class World;

private:
  void     SafelyDisposeComponents();
  EntityID Clone(World &world, Entity &entity) const;
  void *   Get(std::type_index component);
  World &  m_World;
  std::unordered_map<std::type_index, EntityID> m_Components;
  EntityID                                      m_ID{0, 0};
  std::string                                   m_Name{"Nameless Entity"};
};
#include "../../src/entity/Entity.inl"
