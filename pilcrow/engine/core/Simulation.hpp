#pragma once
#include <string>
#include <unordered_map>
#include "Utils/EventArena.hpp"
#include "Utils/ThreadPool.hpp"

#include "pilcrow/engine/core/World.hpp"
#include "pilcrow/engine/core/entity/ArchetypeRef.hpp"

class Simulation : public EventArena, public ThreadPool<> {
public:
  void         Run(double timestep, const std::string &world);
  World &      CreateWorld(const std::string &name);
  World &      GetWorld(const std::string &name);
  ArchetypeRef CreateArchetype(const std::string &name = "Nameless Entity");

private:
  std::unordered_map<std::string, World> m_Worlds;
};
