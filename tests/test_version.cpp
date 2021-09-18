#include "shpxx/version.hpp"

#include <gtest/gtest.h>
TEST(Shpxx, Version)
{
    ASSERT_EQ(shpxx::version(), 0u);
}
