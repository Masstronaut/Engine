#pragma once
#include <typeindex>
#include <utils/include/type_list.hpp>
#include <vector>

template <typename... Args>
class EntitiesWith;
class EntitiesWithBase;
class EntityRef;

class ComponentAggregate {
public:
  // Note: the type_list<Args...> parameter is required for the constructor to
  // deduce the template arguments.
  // It is not possible to invoke as ComponentAggregate<Args...>() and must be
  // of the form:
  // ComponentAggregate(type_list<Args...>{});
  template <typename... Args>
  ComponentAggregate(type_list<Args...>);

  template <typename... Args>
  bool Matches() const;

  // Expects the vector of type_index objects to be sorted.
  bool Matches(const std::vector<std::type_index> &types) const;
  bool Matches(const EntityRef &entity) const;
  std::vector<EntityRef> &      GetEntities();
  const std::vector<EntityRef> &GetEntities() const;
  // Check if the entity has all the components of this aggregate.
  // If it does
  void OnEntityCreated(const EntityRef &entity);
  void OnEntityDestroyed(const EntityRef &entity);
  template <typename... Args>
  void AddEntityList(EntitiesWith<Args...> &);

private:
  template <typename T>
  void AddType();
  template <typename T, typename T2, typename... Args>
  void                         AddType();
  void                         AddVerifiedEntity(const EntityRef &entity);
  std::vector<EntityRef>       m_Entities;
  std::vector<std::type_index> m_Components;
  // These can be updated in constant time with events
  std::vector<EntitiesWithBase *> m_EntityLists;
};
#include "../../src/component/ComponentAggregate.inl"
