#include <gtest/gtest.h>
#include "../include/sample.h"

TEST(testSample, sample)
{
  EXPECT_EQ(1000, cubic(10));
}