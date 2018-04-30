#include <gtest/gtest.h>

#include <Simulation.hpp>
#include <World.hpp>
#include <Entity/EntityRef.hpp>
#include <components/Transform.h>
TEST(Entity, SetParent)
{
  Simulation sim;
  ArchetypeRef parentarch{ sim.CreateArchetype("Parent") };
  ArchetypeRef childarch{ sim.CreateArchetype("Child") };
  World& tw{ sim.CreateWorld("Test World") };
  EntityRef parent{ tw.Spawn(parentarch) };
  EntityRef child{ tw.Spawn(childarch) };

  child.SetParent(parent);
  ASSERT_TRUE(child.HasParent());

}

TEST(Entity, SetParentComponents) {
  Simulation sim;
  ArchetypeRef parentarch{ sim.CreateArchetype("Parent") };
  ArchetypeRef childarch{ sim.CreateArchetype("Child") };
  World& tw{ sim.CreateWorld("Test World") };
  EntityRef parent{ tw.Spawn(parentarch) };
  EntityRef child{ tw.Spawn(childarch) };
  Transform& ptf{ parent.Add<Transform>() };
  ptf.position = glm::vec3{ 0.f,1.f,0.f };
  Transform& ctf{ child.Add<Transform>() };
  ctf.position = glm::vec3{ 1.f,2.f,3.f };
  child.SetParent(parent);
  ASSERT_EQ(ptf, ctf.Parent.Ref());
}

//TEST(SimulationTests,)