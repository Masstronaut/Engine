#include <gtest/gtest.h>
#include "../../../tests/sample_test/src/unittest_sample.cpp"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
	return 0;
}