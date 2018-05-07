#pragma once
#include <unordered_map>
#include <string>
#include <Utils/include/ThreadPool.hpp>
#include <Utils/include/EventArena.hpp>

#include "Entity/ArchetypeRef.hpp"
#include "World.hpp"

class Simulation : public EventArena, public ThreadPool<> {
public:
  void Run( double timestep, const std::string &world );
  World& CreateWorld( const std::string &name );
  World& GetWorld( const std::string &name );
  ArchetypeRef CreateArchetype( const std::string &name = "Nameless Entity" );

private:
  std::unordered_map<std::string, World> m_Worlds;

};
