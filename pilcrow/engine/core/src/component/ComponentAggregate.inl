#pragma once
#include <algorithm>
#include <functional>

#include "../../include/entity/EntitiesWith.hpp"

// Note: the type_list<Args...> parameter is required for the constructor to
// deduce the template arguments.
// It is not possible to invoke as ComponentAggregate<Args...>() and must be of
// the form:
// ComponentAggregate(type_list<Args...>{});
template <typename... Args>
inline ComponentAggregate::ComponentAggregate(type_list<Args...>) {
  AddType<Args...>();
}

template <typename... Args>
bool ComponentAggregate::Matches() const {
  if(sizeof...(Args) != m_Components.size()) return false;
  std::vector<std::type_index> hashes{std::type_index(typeid(Args))...};
  std::sort(std::begin(hashes), std::end(hashes));
  return std::equal(std::begin(hashes), std::end(hashes),
                    std::begin(m_Components));
}

template <typename... Args>
void ComponentAggregate::AddEntityList(EntitiesWith<Args...> &ew) {
  assert(this->Matches<Args...>()
         && "Only call "
            "ComponentAggregate::AddEntityList after "
            "verifying it matches.");
  m_EntityLists.push_back(&ew);
  m_EntityLists.back()->SetEntities(m_Entities);
}

template <typename T>
inline void ComponentAggregate::AddType() {
  static_assert(!std::is_reference_v<T>, "References may not be used as "
                                         "components. Try changing "
                                         "\"Component&\" to \"Component\".");
  m_Components.push_back(std::type_index(typeid(T)));
}

template <typename T, typename T2, typename... Args>
inline void ComponentAggregate::AddType() {
  AddType<T>();
  AddType<T2, Args...>();
  std::sort(std::begin(m_Components), std::end(m_Components));
}
