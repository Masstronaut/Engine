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
  // Note: the type_list<Args...> parameter is required for the constructor to deduce the template arguments.
  // It is not possible to invoke as ComponentAggregate<Args...>() and must be of the form:
  // ComponentAggregate(type_list<Args...>{});
  template<typename... Args>
  ComponentAggregate( type_list<Args...> );

  template<typename... Args>
  bool Matches( ) const;

  // Expects the vector of type_index objects to be sorted. 
  bool Matches( const std::vector<std::type_index>& hashes ) const;
  std::vector<EntityRef>& GetEntities( );
  const std::vector<EntityRef>& GetEntities( ) const;
  // Check if the entity has all the components of this aggregate.
  // If it does
  void OnEntityCreated( const EntityRef &entity );
  void OnEntityDestroyed( const EntityRef &entity );
  template<typename... Args>
  void AddEntityList( EntitiesWith<Args...>& );
private:
  template<typename T>
  void AddType( );
  template<typename T, typename T2, typename... Args>
  void AddType( );
  std::vector<EntityRef> m_Entities;
  std::vector<std::type_index> m_Components;
  // These can be updated in constant time with events
  std::vector<EntitiesWithBase*> m_EntityLists;

};


// Note: the type_list<Args...> parameter is required for the constructor to deduce the template arguments.
// It is not possible to invoke as ComponentAggregate<Args...>() and must be of the form:
// ComponentAggregate(type_list<Args...>{});
template<typename ...Args>
inline ComponentAggregate::ComponentAggregate( type_list<Args...> ) { AddType<Args...>( ); }

template<typename... Args>
bool ComponentAggregate::Matches( ) const {
  if( sizeof...( Args ) != m_Components.size( ) ) return false;
  std::vector<std::type_index> hashes{ std::type_index( typeid( Args ) )... };
  std::sort( std::begin( hashes ), std::end( hashes ) );
  return std::equal( std::begin( hashes ), std::end( hashes ), std::begin( m_Components ) );
}


template<typename... Args>
void ComponentAggregate::AddEntityList( EntitiesWith<Args...> &ew ) {
  assert( this->Matches<Args...>( ) && "Only call ComponentAggregate::AddEntityList after verifying it matches." );
  m_EntityLists.push_back( &ew );
  m_EntityLists.back( )->SetEntities( m_Entities );
}

template<typename T>
inline void ComponentAggregate::AddType( ) {
  static_assert( !std::is_reference_v<T>, "References may not be used as components. Try changing \"Component&\" to \"Component\"." );
  m_Components.push_back( std::type_index( typeid( T ) ) );
}

template<typename T, typename T2, typename ...Args>
inline void ComponentAggregate::AddType( ) {
  AddType<T>( );
  AddType<T2, Args...>( );
  std::sort( std::begin( m_Components ), std::end( m_Components ) );
}


