#pragma once

#include <unordered_map>
#include <string>
#include "ThreadPool.hpp"
#include "EventArena.hpp"
class World;
class ArchetypeRef;
class EntityRef;
class Simulation : public EventArena, public ThreadPool<> {
public:
  void Run( double timestep );
  World& CreateWorld( const std::string &name );
  World& GetWorld( const std::string &name );
  ArchetypeRef CreateArchetype( const std::string &name = "Nameless Entity" );

private:
  std::unordered_map<std::string, World> m_Worlds;

};
