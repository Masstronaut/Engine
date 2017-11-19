#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

#include <slot_map.hpp>

#include "EntityID.hpp"
#include "EntityRef.hpp"
#include "type_list.hpp"
class ComponentAggregate {
public:
  template<typename... Args>
  ComponentAggregate( type_list<Args...> ) { AddType<Args...>( ); }

  template<typename... Args>
  bool matches( ) const;

  // Expects the vector of hashes to be sorted. 
  bool matches( const std::vector<std::size_t>& hashes );

  // Check if the entity has all the components of this aggregate.
  // If it does
  void OnEntityCreated( const EntityRef &entity );
  void OnEntityDestroyed( const EntityRef &entity );
private:
  template<typename T>
  void AddType( ) {
    // @@TODO: CONVERT all usage of typeid as a key to type_index(typeid(T)).
    m_Components.push_back( typeid( std::decay_t<T> ).hash_code( ) );
  }
  template<typename T, typename T2, typename... Args>
  void AddType( ) {
    AddType<T>( );
    AddType<T2>( );
    AddType<Args...>( );
    std::sort( std::begin( m_Components ), std::end( m_Components ) );
  }
  std::vector<EntityRef> m_Entities;
  std::vector<std::size_t> m_Components;

  std::vector<std::function<void( const std::vector<EntityRef> & )>> m_Systems;

};

template<typename... Args>
bool ComponentAggregate::matches( ) const {
  std::vector<std::size_t> hashes{ typeid( Args ).hash_code( )... };
  std::sort( std::begin( hashes ), std::end( hashes ) );
  return matches( hashes );
}