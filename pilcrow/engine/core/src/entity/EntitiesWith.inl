#pragma once
#include "../../include/entity/ConstrainedEntityRef.hpp"

template <typename... Args>
inline typename EntitiesWith<Args...>::iterator EntitiesWith<Args...>::begin() {
  return m_Entities.begin();
}

template <typename... Args>
inline typename EntitiesWith<Args...>::iterator EntitiesWith<Args...>::end() {
  return m_Entities.end();
}

template <typename... Args>
inline typename EntitiesWith<Args...>::const_iterator
EntitiesWith<Args...>::begin() const {
  return m_Entities.cbegin();
}

template <typename... Args>
inline typename EntitiesWith<Args...>::const_iterator
EntitiesWith<Args...>::end() const {
  return m_Entities.cend();
}

template <typename... Args>
inline typename EntitiesWith<Args...>::const_iterator
EntitiesWith<Args...>::cbegin() const {
  return m_Entities.cbegin();
}

template <typename... Args>
inline typename EntitiesWith<Args...>::const_iterator
EntitiesWith<Args...>::cend() const {
  return m_Entities.cend();
}

template <typename... Args>
void EntitiesWith<Args...>::SetEntities(
  const std::vector<EntityRef> &entities) {
  m_Entities.assign(std::begin(entities), std::end(entities));
}

template <typename... Args>
void EntitiesWith<Args...>::SwapPopEntity(std::size_t index) {
  m_Entities[index] = m_Entities.back();
  m_Entities.pop_back();
}

template <typename... Args>
void EntitiesWith<Args...>::PushEntity(const EntityRef &entity) {
  m_Entities.push_back(entity);
}

template <typename... Args>
ConstrainedEntityRef<Args...> &EntitiesWith<Args...>::
                               operator[](std::size_t pos) {
  return m_Entities[pos];
}

template <typename... Args>
const ConstrainedEntityRef<Args...> &EntitiesWith<Args...>::
                                     operator[](std::size_t pos) const {
  return m_Entities[pos];
}
