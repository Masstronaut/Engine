#pragma once
#include <string>
#include <tuple>
#include <typeindex>
#include <vector>

#include "pilcrow/engine/core/entity/EntityID.hpp"

class World;
class Entity;

class EntityRef {
public:
  EntityRef(EntityID ID, World *world);
  EntityRef(const EntityRef &) = default;
  EntityRef(EntityRef &&)      = default;
  ~EntityRef()                 = default;
  EntityRef &operator          =(const EntityRef &rhs);

  template <typename Component>
  bool Has() const;
  template <typename Component, typename Component2, typename... Components>
  bool Has() const;
  bool Has(std::type_index component_type) const;
  bool Has(const std::vector<std::type_index> &components) const;

  template <typename Component>
  Component &Get();
  template <typename Component>
  const Component &Get() const;

  template <typename Component, typename... Args>
  Component &Add(Args &&... args);
  template <typename Component>
  void Remove();

  void Kill();

  EntityID ID() const;

  const std::string &Name() const;
  EntityRef &        Name(const std::string &name);

  bool operator==(const EntityRef &rhs) const;

  EntityRef Clone() const;
  // World needs access to the m_World in order to implement spawning across
  // worlds
  // This is useful for tasks such as spawning an archetype or moving something
  // to
  // a new World. Since the World is not exposed, it must be accessed as a
  // friend.
  friend class World;

protected:
  EntityID m_ID{0, 0};
  World *  m_World;
};
#include "../entity/EntityRef.inl"
