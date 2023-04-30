#include "shpxx/coordinate.hpp"
#include "shpxx/exceptions.hpp"
#include "shpxx/io/file.hpp"
#include "shpxx/io/openmode.hpp"
#include "shpxx/shape_type.hpp"
#include "test_data.hpp"

#include <stdexcept>

#include <gtest/gtest.h>

#define SHPXX_UNUSED(expr) (void)(expr)

#define EXPECT_PT_NEAR(l, r)                                                                       \
    EXPECT_NEAR((l).x, (r).x, 0.00001);                                                            \
    EXPECT_NEAR((l).y, (r).y, 0.00001);                                                            \
    EXPECT_NEAR((l).z, (r).z, 0.00001);                                                            \
    EXPECT_NEAR((l).m, (r).m, 0.00001)

TEST(ShpfileOpenTestData, Linestring)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/linestring.shp";
    const auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);

    ASSERT_EQ(shapefile.size(), 1);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::linestring);

    const auto expected_min_bound = shpxx::coordinate_xyzm_t{-96.72190, 43.55615, 0, 0};
    const auto expected_max_bound = shpxx::coordinate_xyzm_t{-96.72155, 43.55698, 0, 0};

    const std::vector<shpxx::coordinate_xyzm_t> expected_points{
        {-96.721557624996549, 43.556150537227644, 0, 0},
        {-96.721843949848036, 43.556460716028290, 0, 0},
        {-96.721846901650622, 43.556691744718172, 0, 0},
        {-96.721900034097288, 43.556817954461671, 0, 0},
        {-96.721849853453193, 43.556963416211026, 0, 0},
        {-96.721814431822111, 43.556982668475051, 0, 0}};

    EXPECT_PT_NEAR(shapefile.min_bound(), expected_min_bound);
    EXPECT_PT_NEAR(shapefile.max_bound(), expected_max_bound);

    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::point_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::linestring_t>(1)), std::out_of_range);

    const auto maybe_feature = shapefile.at<shpxx::feature::linestring_t>(0);
    ASSERT_TRUE(maybe_feature);
    const auto& feature = *maybe_feature;

    EXPECT_PT_NEAR(feature.min_bound(), expected_min_bound);
    EXPECT_PT_NEAR(feature.max_bound(), expected_max_bound);

    ASSERT_EQ(feature.num_parts(), 1);
    EXPECT_EQ(feature.num_points(), expected_points.size());

    EXPECT_THROW(SHPXX_UNUSED(feature.at(1)), std::out_of_range);

    const auto part = feature.at(0);
    ASSERT_EQ(part.size(), expected_points.size());

    for (std::size_t i = 0; i < part.size(); ++i)
    {
        SCOPED_TRACE("i = " + std::to_string(i));
        EXPECT_PT_NEAR(part.at<shpxx::coordinate_xyzm_t>(i), expected_points[i]);
    }
}

TEST(ShpfileOpenTestData, MultiPointZ)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/multipoint-z.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 3);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::multi_point_z);

    const auto feature = shapefile.at<shpxx::feature::multi_point_t>(0);
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
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::linestring_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
}

TEST(ShpfileOpenTestData, PointM)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/point-m.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 2);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::point_m);

    const auto feature = shapefile.at<shpxx::feature::point_t>(0);
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
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::linestring_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
}

TEST(ShpfileOpenTestData, Polygon)
{
    const auto path = std::string(shpxx::testing::data_directory) + "/polygon.shp";
    auto shapefile = shpxx::io::file::open_shp(path, shpxx::io::read_only);
    ASSERT_EQ(shapefile.size(), 3);
    EXPECT_EQ(shapefile.type(), shpxx::shape_type::polygon);

    const auto feature = shapefile.at<shpxx::feature::polygon_t>(0);
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::point_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
}
