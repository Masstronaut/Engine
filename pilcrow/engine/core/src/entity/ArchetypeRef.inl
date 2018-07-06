#pragma once
#include "../../include/World.hpp"

template <typename Component, typename... Args>
inline Component &ArchetypeRef::Add(Args &&... args) {
  return m_World->GetEntity(m_ID)->Add<Component>(std::forward<Args>(args)...);
}
template <typename Component>
inline void ArchetypeRef::Remove() {
  m_World->GetEntity(m_ID)->Remove<Component>();
}
