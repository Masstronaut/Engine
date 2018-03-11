#pragma once
#include <type_traits>
#include <vector>
#include "EntityRef.hpp"
class EntitiesWithBase {
  friend class ComponentAggregate;

protected:
  virtual void SetEntities( const std::vector<EntityRef> & ) = 0;
  virtual void SwapPopEntity( std::size_t index ) = 0;
  virtual void PushEntity( const EntityRef& ) = 0;
};
template<typename... Args>
class EntitiesWith : public EntitiesWithBase {
public:
  using iterator = typename std::vector< ConstrainedEntityRef<Args...> >::iterator;
  using const_iterator = typename std::vector< ConstrainedEntityRef<Args...> >::const_iterator;

  iterator begin( );
  iterator end( );
  const_iterator begin( ) const;
  const_iterator end( ) const;
  const_iterator cbegin( ) const;

  const_iterator cend( ) const;
  ConstrainedEntityRef<Args...>& operator[]( std::size_t pos );
  const ConstrainedEntityRef<Args...>& operator[]( std::size_t pos ) const;
private:
  // @@TODO:
  // If you can make this just a reference to the vector 
  // in the ComponentAggregate responsible for this EntitiesWith,
  // It will use way less memory be more efficient.
  // Those benefits are because many systems with the same requirements
  // could reference a single vector of constrained entity refs. (less mem)
  // Since they reference the original, updates would not need to be pushed. (less work)
  // Optionally, use style guide to enforce alphabetical ordering instead
  std::vector<ConstrainedEntityRef<Args...>> m_Entities;
  void SetEntities( const std::vector<EntityRef>& ) final;
  void SwapPopEntity( std::size_t index ) final;
  void PushEntity( const EntityRef& ) final;
};

template<typename ...Args>
inline typename EntitiesWith<Args...>::iterator EntitiesWith<Args...>::begin( ) {
  return m_Entities.begin( );
}

template<typename... Args>
inline typename EntitiesWith<Args...>::iterator EntitiesWith<Args...>::end( ) {
  return m_Entities.end( );
}

template<typename ...Args>
inline typename EntitiesWith<Args...>::const_iterator EntitiesWith<Args...>::begin( ) const {
  return m_Entities.cbegin( );
}

template<typename... Args>
inline typename EntitiesWith<Args...>::const_iterator EntitiesWith<Args...>::end( ) const {
  return m_Entities.cend( );
}

template<typename ...Args>
inline typename EntitiesWith<Args...>::const_iterator EntitiesWith<Args...>::cbegin( ) const {
  return m_Entities.cbegin( );
}

template<typename ...Args>
inline typename EntitiesWith<Args...>::const_iterator EntitiesWith<Args...>::cend( ) const {
  return m_Entities.cend( );
}

template<typename... Args>
void EntitiesWith<Args...>::SetEntities( const std::vector<EntityRef> &entities ) {
  m_Entities.assign( std::begin( entities ), std::end( entities ) );
}

template<typename... Args>
void EntitiesWith<Args...>::SwapPopEntity( std::size_t index ) {
  m_Entities[ index ] = m_Entities.back( );
  m_Entities.pop_back( );
}

template<typename... Args>
void EntitiesWith<Args...>::PushEntity( const EntityRef& entity ) {
  m_Entities.push_back( entity );
}

template<typename ReturnType, typename Class, typename... Args>
EntitiesWith<std::remove_reference_t<Args>...> EntitiesToProcess( ReturnType( Class::*Fn )( Args... )const ) {
  return EntitiesWith<std::remove_reference_t<Args>...>{};
}
template<typename... Args>
ConstrainedEntityRef<Args...>& EntitiesWith<Args...>::operator[]( std::size_t pos ) {
  return m_Entities[ pos ];
}

template<typename... Args>
const ConstrainedEntityRef<Args...>& EntitiesWith<Args...>::operator[]( std::size_t pos ) const {
  return m_Entities[ pos ];
}