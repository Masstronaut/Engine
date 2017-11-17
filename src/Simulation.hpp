#pragma once

#include <unordered_map>
#include <string>
#include "ThreadPool.hpp"
#include "EventArena.hpp"
class World;
class Simulation : public EventArena, public ThreadPool<> {
public:
  void Run( double timestep );
  World& CreateWorld( const std::string &name );
  World& GetWorld( const std::string &name );


private:
  std::unordered_map<std::string, World> m_Worlds;

};

