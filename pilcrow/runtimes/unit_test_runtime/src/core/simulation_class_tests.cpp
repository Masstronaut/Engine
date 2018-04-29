#include <gtest/gtest.h>

#include <Simulation.hpp>
TEST(SimulationTests, CreateWorld)
{
  Simulation sim;
  World& TestWorld{ sim.CreateWorld("Test World") };
  ASSERT_EQ(TestWorld.Name(), "Test World");
}

//TEST(SimulationTests,)