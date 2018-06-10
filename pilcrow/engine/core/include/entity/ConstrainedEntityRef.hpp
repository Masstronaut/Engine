#pragma once
#include "EntityRef.hpp"

template <typename... Args>
class ConstrainedEntityRef {
public:
  ConstrainedEntityRef(const EntityRef &ConstrainedEntity);
  ConstrainedEntityRef(const ConstrainedEntityRef &) = default;
  ~ConstrainedEntityRef()                            = default;

  template <typename Component,
            typename = std::enable_if_t<!std::is_const_v<Component>>>
  Component &Get();

  template <typename Component,
            typename = std::enable_if_t<std::is_const_v<Component>>>
  const Component &Get() const;

  template <typename Predicate>
  void Invoke(Predicate &&p);
  template <typename ReturnType, typename Class>
  void Invoke(ReturnType (Class::*Fn)(Args...) const, const Class &c);

private:
  EntityRef m_Self;

  template <typename Component, typename T1>
  Component &GetMutable();
  template <typename Component, typename T1, typename T2, typename... FnArgs>
  Component &GetMutable();
  template <typename Component, typename T1>
  const Component &GetImmutable() const;
  template <typename Component, typename T1, typename T2, typename... FnArgs>
  const Component &GetImmutable() const;
};
#include "../../src/entity/ConstrainedEntityRef.inl"
