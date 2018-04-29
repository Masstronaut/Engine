#include <gtest/gtest.h>

#include <utils/include/EventArena.hpp>

namespace {

TEST(EventArena, CallOnce) {
  EventArena ea;
  int i{ 0 };
  ea.OnNext([&i](const int&) { ++i; });
  ea.Emit(0);
  ASSERT_EQ(i, 1);
  ea.Emit(0);
  ASSERT_EQ(i, 1);

  for (int j{ 0 }; j < 5; ++j)
    ea.OnNext([&i](const int&) { ++i; });
  ASSERT_EQ(i, 1);
  ea.Emit(0);
  ASSERT_EQ(i, 6);
  ea.Emit(0);
  ASSERT_EQ(i, 6);
}

TEST(EventArena, CallOnceMultiType) {
  EventArena ea;
  int i{ 0 };
  ea.OnNext([&i](const int&) { ++i; });
  ea.OnNext([&i](const float&) { i += 2; });
  ASSERT_EQ(i, 0);
  ea.Emit(0);
  ASSERT_EQ(i, 1);
  ea.Emit(0);
  ASSERT_EQ(i, 1);
  ea.OnNext([&i](const int&) { ++i; });
  ea.Emit(0.f);
  ASSERT_EQ(i, 3);
  ea.Emit(0);
  ASSERT_EQ(i, 4);
  ea.Emit(0);
  ASSERT_EQ(i, 4);
  ea.Emit(0.f);
  ASSERT_EQ(i, 4);
  for (int j{ 0 }; j < 5; ++j) {
    ea.OnNext([&i](const int&) { ++i; });
    ea.OnNext([&i](const float&) { i += 2; });
  }
  ea.Emit(0);
  ASSERT_EQ(i, 9);
  ea.Emit(0);
  ASSERT_EQ(i, 9);
  ea.Emit(0.f);
  ASSERT_EQ(i, 19);
}








} // namespace
