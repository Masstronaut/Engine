#pragma once
#include <functional>
#include <algorithm>

#include "Detectors.hpp"
#include "Entity/Entity.hpp"
#include "System/SystemTraits.hpp"
#include "Component/ComponentTraits.hpp"
#include "System/System.hpp"

template<typename T>
T& World::GetComponent( EntityID entity ) {
  return GetComponentPool<std::decay_t<T>>( ).components[ entity ];
}
template<typename T>
const T& World::GetComponent( EntityID entity ) const {
  return GetComponentPool<std::decay_t<T>>( ).components[ entity ];
}

template<typename... Args>
ComponentAggregate& World::GetAggregate( ) {
  std::vector<std::type_index> hashes{ std::type_index( typeid( Args ) )... };
  std::sort( std::begin( hashes ), std::end( hashes ) );
  for( auto& aggregate : m_Aggregates ) {
    if( aggregate.Matches( hashes ) ) {
      return aggregate;
    }
  }
  m_Aggregates.emplace_back( type_list<Args...>{} );
  for( const auto &entity : m_Entities ) {
    m_Aggregates.back( ).OnEntityCreated( EntityRef( entity.ID(), this ) );
  }
  return m_Aggregates.back( );
}

template<typename... Args>
ComponentAggregate& World::GetAggregate( type_list<Args...> ) {
  return this->GetAggregate<Args...>( );
}

template<typename T, typename... Args>
void World::AddSystem( const std::string &name, Args&&... args) {
  m_Systems.push_back(std::move(std::unique_ptr<SystemBase>(new System<T>(*this, name, std::forward<Args>(args)...))));
}



template<typename... Args>
void World::RegisterEntitiesWith( EntitiesWith<Args...> &ew ) {
  this->GetAggregate<Args...>( ).AddEntityList( ew );
}

template<typename T>
ComponentPool<T>& World::GetComponentPool( ) {
  std::type_index ti{ std::type_index( typeid( T ) ) };
  
  if( auto it{ m_Components.find( ti ) }; it != m_Components.end( ) )
    return *reinterpret_cast< ComponentPool<T>* >( it->second.get( ) );
  else {
    std::unique_ptr<ComponentPoolBase> &uptr{ m_Components[ ti ] };
    uptr.reset( new ComponentPool<T>( *this ) );
    return *reinterpret_cast< ComponentPool<T>* >( uptr.get( ) );
  }
}