#pragma once
#include "ComponentAggregate.hpp"
#include "EntityRef.hpp"


  // Check if the entity has all the components of this aggregate.
  // If it does
void ComponentAggregate::OnEntityCreated( const EntityRef &entity ) {
  for( const auto& comp : m_Components ) {
    if( !entity.Has( comp ) ) {
      return;
    }
  }
  m_Entities.push_back( entity );
  for( auto entityList : m_EntityLists ) {
    entityList->PushEntity( entity );
  }
}
void ComponentAggregate::OnEntityDestroyed( const EntityRef &entity ) {
  auto it{ std::find( std::begin( m_Entities ), std::end( m_Entities ), entity ) };
  auto index{ std::distance( std::begin( m_Entities ), it ) };
  *it = m_Entities.back( );
  m_Entities.pop_back( );
  for( auto entityList : m_EntityLists ) {
    entityList->SwapPopEntity( index );
  }
}


bool ComponentAggregate::matches( const std::vector<std::type_index> &hashes ) {
  if( hashes.size( ) != m_Components.size( ) ) return false;
  return std::equal( std::begin( hashes ), std::end( hashes ), std::begin( m_Components ) );
}