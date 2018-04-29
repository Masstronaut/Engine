#include <gtest/gtest.h>

#include <utils/include/EventArena.hpp>

namespace {

TEST(EventArena, StatelessLambdaRegistration) {
// This is a compilation test, and will fail at compile time if broken
  EventArena ea;
  ea.On([](const int&) {});
}

TEST(EventArena, StatefulLambdaRegistration) {
  // This is a compilation test, and will fail at compile time if broken
  EventArena ea;
  float f{ 3.14f };
  ea.On([&f](const int&) { f = 1.f; });
}

TEST(EventArena, LambdaExecution) {
  EventArena ea;
  int i{ 0 };
  ea.On([&i](const int&) { ++i; });
  ea.Emit(5);
  ASSERT_EQ(i, 1);
  ea.On([&i](const int&) { ++i; });
  ea.Emit(0);
  ASSERT_EQ(i, 3);
  ea.On([&i](const int&) { ++i; });
  ea.Emit(0);
  ASSERT_EQ(i, 6);
  ea.On([&i](const int& event) { i -= event; });
  ea.Emit(3);
  ASSERT_EQ(i, 6);
}
TEST(EventArena, LambdaExecutionMultiType) {
  EventArena ea;
  int i{ 0 };
  ea.On([&i](const int&) { ++i; });
  ea.On([&i](const int&) { ++i; });
  ea.On([&i](const int&) { ++i; });
  ea.On([&i](const int& event) {i -= event; });
  ea.On([&i](const float& event) { i = static_cast<int>(event); });
  ea.Emit(1);
  ASSERT_EQ(i, 2);
  ea.Emit(5.f);
  ASSERT_EQ(i, 5);
}




} // namespace