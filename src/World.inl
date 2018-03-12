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
template<typename ReturnType>
void World::AddSystem( ReturnType( *fn )( float ), const std::string& name ) {
  m_Updaters.emplace_back( std::move( name ),
                          [ fn ]( float dt ) {
                            fn( dt );
                          } );
}
template<typename ReturnType>
inline void World::AddSystem( ReturnType( *fn )( void ), const std::string & name ) {
  m_Updaters.emplace_back( std::move( name ),
                           [ fn ]( float ) {
                             fn( );
                           });
}


template<typename Predicate>
inline std::enable_if_t<HasOperatorFnCall_v<Predicate>> 
World::AddSystem( Predicate && pred, const std::string & name ) {
  this->AddSystem( &Predicate::operator(), std::forward<Predicate>( pred ), name );
}


// Member functions that run once per update tick
template<typename ReturnType, typename ClassName>
void World::AddSystem( ReturnType( ClassName::*fn )( float ), ClassName& instance, const std::string& name ) {
  m_Updaters.emplace_back( std::move( name ),
                           [ fn, &instance ]( float dt ) {
                             ( instance.*fn )( dt );
                           } );
}
template<typename ReturnType, typename ClassName>
inline void World::AddSystem( ReturnType( ClassName::* fn )( void ), ClassName & instance, const std::string & name ) { 
  m_Updaters.emplace_back( std::move( name ), std::function<void(float)>(
                           [ fn, &instance ]( float ) {
                             ( instance.*fn )( );
  } ) );
}
// const Member functions that run once per update tick
template<typename ReturnType, typename ClassName>
void World::AddSystem( ReturnType( ClassName::*fn )( float ) const, const ClassName& instance, const std::string& name ) {
  m_Updaters.emplace_back( std::move( name ),
                           [ fn, &instance ]( float dt ) {
                             ( instance.*fn )( dt );
                           } );
}
template<typename ReturnType, typename ClassName>
void World::AddSystem( ReturnType( ClassName::*fn )( void ) const, const ClassName& instance, const std::string& name ) {
  m_Updaters.emplace_back( std::move( name ),
                           [ fn, &instance ]( float ) {
                             ( instance.*fn )( );
                           } );
}
template<typename T>
void World::AddSystem( const std::string &name ) {
  if constexpr ( SystemTraits<T>::IsParallelSystem ) {
    AddParallelSystem<T>( name );
  }
  else if constexpr( SystemTraits<T>::IsPureSystem ) {
    // PURE system
    AddPureSystem<T>( name );
  }
  else if constexpr ( !SystemTraits<T>::IsPureSystem ) {
    // STATEFUL system
    AddStatefulSystem<T>( name );
  }
}



template<typename... Args>
void World::RegisterEntitiesWith( EntitiesWith<Args...> &ew ) {
  this->GetAggregate<Args...>( ).AddEntityList( ew );
}


template<typename T>
void World::AddPureSystem( const std::string &name ) {
  static_assert( 0, "implementation of World::AddPureSystem does not yet exist." );
}
template<typename T>
void World::AddStatefulSystem( const std::string &name ) {
  m_Systems.push_back( std::move( std::unique_ptr<SystemBase>( new System<T>( *this, name ) ) ) );
}

template<typename T>
void World::AddParallelSystem( const std::string &name) {
  m_Systems.push_back( std::move( std::unique_ptr<SystemBase>( new System<T>( *this, name ) ) ) );
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