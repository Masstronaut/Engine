#include <gtest/gtest.h>
//#include <core/AllCoreTests.h>

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
	return 0;
}