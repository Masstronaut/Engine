#pragma once
#include "Simulation.hpp"
#include "World.hpp"


void Simulation::Run( double timestep ) {

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
  //@@TODO: make this stricter so get won't create a world
  return CreateWorld( name );
}

EntityRef Simulation::CreateArchetype( const std::string & name ) {
  auto it{ m_Worlds.find( "Archetype World" ) };
  if( it != m_Worlds.end( ) ) {
    return it->second.CreateEntity( name );
  }
  else {
    return this->CreateWorld( "Archetype World" ).CreateEntity( name );
  }
}
