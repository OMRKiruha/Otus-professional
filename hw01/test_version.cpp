#include "lib.h"

#include <gtest/gtest.h>

int main(int argc, char **argv) {
    testing::FLAGS_gtest_color = "yes";
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(test_version, test_valid_version) {
    ASSERT_NE(version(), 100);
}