#pragma once
#include <cassert>

#include "pilcrow/engine/core/World.hpp"

template <typename Component>
bool Entity::Has() const {
  return m_Components.count(std::type_index(typeid(std::decay_t<Component>)))
         > 0;
}

template <typename Component, typename Component2, typename... Components>
bool Entity::Has() const {
  return Has<Component>() && Has<Component2>() && Has<Components...>();
}

template <typename Component, typename... Args>
Component &Entity::Add(Args &&... args) {
  assert(this->Has<Component>() == false
         && "An entity may only be associated "
            "with a single instance of each "
            "component type.");
  EntityID compHandle{m_World.GetComponentPool<Component>().components.emplace(
    std::forward<Args>(args)...)};
  auto     it{m_Components.find(std::type_index(typeid(Component)))};
  if(it != m_Components.end()) it->second = compHandle;
  return this->Get<Component>();
}

template <typename Component>
void Entity::Remove() {
  auto it{m_Components.find(std::type_index(typeid(Component)))};
  if(it != m_Components.end()) {
    m_World.GetComponentPool<Component>().components.erase(it->second);
    m_Components.erase(it);
  }
}

template <typename Component>
Component &Entity::Get() {
  using DecayedType                      = std::decay_t<Component>;
  static const std::type_index TypeIndex = std::type_index(typeid(DecayedType));
  return m_World.GetComponent<DecayedType>(m_Components[TypeIndex]);
}
template <typename Component>
const Component &Entity::Get() const {
  auto it{m_Components.find(std::type_index(typeid(Component)))};
  if(it != m_Components.end()) {
    return m_World.GetComponent<std::decay_t<Component>>(it->second);
  } else {
    assert(it != m_Components.end()
           && "Tried to get a component from an entity which does not have "
              "that component.");
  }
}
