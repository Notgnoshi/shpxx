#include "shpxx/version.hpp"

#include <gtest/gtest.h>
TEST(Shpxx, Version)
{
    ASSERT_GE(shpxx::version_t::major, 0);
    ASSERT_GE(shpxx::version_t::minor, 0);
    ASSERT_GE(shpxx::version_t::patch, 0);

    ASSERT_GT(shpxx::version_t::version_str.size(), 0);
    ASSERT_GT(shpxx::version_t::build_timestamp.size(), 0);
    ASSERT_GT(shpxx::version_t::git_hash.size(), 0);
}
