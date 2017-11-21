#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <typeindex>

#include <slot_map.hpp>
#include "EntityID.hpp"
#include "EntityRef.hpp"
#include "type_list.hpp"
#include "EntitiesWith.hpp"
class ComponentAggregate {
public:
  template<typename... Args>
  ComponentAggregate( type_list<Args...> ) { AddType<Args...>( ); }

  template<typename... Args>
  bool matches( ) const;

  // Expects the vector of hashes to be sorted. 
  bool matches( const std::vector<std::type_index>& hashes );

  // Check if the entity has all the components of this aggregate.
  // If it does
  void OnEntityCreated( const EntityRef &entity );
  void OnEntityDestroyed( const EntityRef &entity );
private:
  template<typename T>
  void AddType( ) {
    static_assert( !std::is_reference_v<T>, "References may not be used as components. Try changing \"Component&\" to \"Component\"." );
    m_Components.push_back( std::type_index( typeid( T ) ) );
  }
  template<typename T, typename T2, typename... Args>
  void AddType( ) {
    AddType<T>( );
    AddType<T2>( );
    AddType<Args...>( );
    std::sort( std::begin( m_Components ), std::end( m_Components ) );
  }
  std::vector<EntityRef> m_Entities;
  std::vector<std::type_index> m_Components;
  // These can be updated in constant time with events
  std::vector<EntitiesWithBase*> m_EntityLists;
  std::vector<std::function<void( const std::vector<EntityRef> & )>> m_Systems;

};

template<typename... Args>
bool ComponentAggregate::matches( ) const {
  if( sizeof...( Args ) != m_Components.size( ) ) return false;
  std::vector<std::type_index> hashes{ std::type_index( typeid( Args ) )... };
  std::sort( std::begin( hashes ), std::end( hashes ) );
  return std::equal( std::begin( hashes ), std::end( hashes ), std::begin( m_Components ) );
}