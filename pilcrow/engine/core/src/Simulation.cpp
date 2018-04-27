#pragma once
#include "../include/Simulation.hpp"
#include "../include/World.hpp"
#include "../include/entity/ArchetypeRef.hpp"

void Simulation::Run( double timestep, const std::string &world ) {
  this->GetWorld( world ).Update( (float)timestep );
}
World& Simulation::CreateWorld( const std::string &name ) {
  auto result{ m_Worlds.emplace( name, name ) };

  return result.first->second;
}
World& Simulation::GetWorld( const std::string &name ) {
  auto result{ m_Worlds.find( name ) };
  if( result != m_Worlds.end( ) ) {
    return result->second;
  }
  assert( 0 && "The world you attempted to get doesn't exist. Try CreateWorld()" );
  return CreateWorld( name );
}

ArchetypeRef Simulation::CreateArchetype( const std::string & name ) {
  return this->CreateWorld( "Archetype World" ).CreateEntity( name );
}
