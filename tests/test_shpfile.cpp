#include "shpxx/exceptions.hpp"
#include "shpxx/io/file.hpp"
#include "shpxx/io/openmode.hpp"
#include "shpxx/shape_type.hpp"
#include "test_data.hpp"

#include <gtest/gtest.h>

#define SHPXX_UNUSED(expr) (void)(expr)

TEST(ShpfileOpenTestData, Linestring)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/linestring.shp";
    const auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);

    ASSERT_EQ(shapefile.size(), 1);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::linestring);

    // const auto feature = shapefile.at<shpxx::feature::linestring_t>(0);
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::point_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
}

TEST(ShpfileOpenTestData, MultiPointZ)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/multipoint-z.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 3);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::multi_point_z);

    // const auto feature = shapefile.at<shpxx::feature::multi_point_t>(0);
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::point_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
}

TEST(ShpfileOpenTestData, Point)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/point.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 2);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::point);

    const auto feature = shapefile.at<shpxx::feature::point_t>(0);
    // EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::linestring_t>(0)),
    //              shpxx::incompatible_feature_type_error_t);
}

TEST(ShpfileOpenTestData, PointM)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/point-m.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 2);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::point_m);

    // const auto feature = shapefile.at<shpxx::feature::linestring_t>(0);
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::linestring_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
}

TEST(ShpfileOpenTestData, PointZ)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/point-z.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 3);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::point_z);

    const auto feature = shapefile.at<shpxx::feature::point_t>(0);
    // EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::linestring_t>(0)),
    //              shpxx::incompatible_feature_type_error_t);
}

TEST(ShpfileOpenTestData, Polygon)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/polygon.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 3);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::polygon);

    // const auto feature = shapefile.at<shpxx::feature::polygon_t>(0);
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::point_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
}
