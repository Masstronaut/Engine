#pragma once
#include "ComponentAggregate.hpp"
#include "../EntityRef.hpp"


  // Check if the entity has all the components of this aggregate.
  // If it does, add it to the aggregate.
void ComponentAggregate::OnEntityCreated( const EntityRef &entity ) {
  if (!entity.Has(m_Components)) return;
  m_Entities.push_back( entity );
  for( auto entityList : m_EntityLists ) {
    entityList->PushEntity( entity );
  }
}
void ComponentAggregate::OnEntityDestroyed( const EntityRef &entity ) {
  auto it{ std::find( std::begin( m_Entities ), std::end( m_Entities ), entity ) };
  if (it == m_Entities.end()) return;
  auto index{ std::distance( std::begin( m_Entities ), it ) };
  *it = m_Entities.back( );
  m_Entities.pop_back( );
  for( auto entityList : m_EntityLists ) {
    entityList->SwapPopEntity( index );
  }
}


bool ComponentAggregate::Matches( const std::vector<std::type_index> &types ) const {
  if( types.size( ) != m_Components.size( ) ) return false;
  return std::equal( std::cbegin( types ), std::cend( types ), std::cbegin( m_Components ) );
}

std::vector<EntityRef>& ComponentAggregate::GetEntities( ) { return m_Entities; }

const std::vector<EntityRef>& ComponentAggregate::GetEntities( ) const { return m_Entities; }
