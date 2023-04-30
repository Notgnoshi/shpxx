#include "shpxx/coordinate.hpp"
#include "shpxx/exceptions.hpp"
#include "shpxx/feature/polygon.hpp"
#include "shpxx/io/file.hpp"
#include "shpxx/io/openmode.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shpfile.hpp"
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

    const auto expected_min_bound = shpxx::coordinate_xyzm_t{-96.72412, 43.55302, 0, 0};
    const auto expected_max_bound = shpxx::coordinate_xyzm_t{-96.72211, 43.55627, 0, 0};

    EXPECT_PT_NEAR(shapefile.min_bound(), expected_min_bound);
    EXPECT_PT_NEAR(shapefile.max_bound(), expected_max_bound);

    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::point_t>(0)),
                 shpxx::incompatible_feature_type_error_t);
    EXPECT_THROW(SHPXX_UNUSED(shapefile.at<shpxx::feature::polygon_t>(4)), std::out_of_range);

    {
        const auto maybe_feature = shapefile.at<shpxx::feature::polygon_t>(0);
        ASSERT_TRUE(maybe_feature);
        const auto& feature = *maybe_feature;

        const auto expected_min_bound = shpxx::coordinate_xyzm_t{-96.72229, 43.55510, 0, 0};
        const auto expected_max_bound = shpxx::coordinate_xyzm_t{-96.72217, 43.55521, 0, 0};

        const std::vector<shpxx::coordinate_xyzm_t> expected_points{
            {-96.722177503540976, 43.555138701489163, 0, 0},
            {-96.722215876974673, 43.555104474190379, 0, 0},
            {-96.722283768434309, 43.555140840694712, 0, 0},
            {-96.722292623842065, 43.555200738418705, 0, 0},
            {-96.722266057618725, 43.555211434434582, 0, 0},
            {-96.722177503540976, 43.555138701489163, 0, 0}};

        EXPECT_PT_NEAR(feature.min_bound(), expected_min_bound);
        EXPECT_PT_NEAR(feature.max_bound(), expected_max_bound);

        ASSERT_EQ(feature.num_parts(), 1);
        EXPECT_EQ(feature.num_points(), expected_points.size());

        EXPECT_THROW(SHPXX_UNUSED(feature.at(1)), std::out_of_range);

        const auto part = feature.at(0);
        ASSERT_EQ(part.size(), expected_points.size());

        for (std::size_t i = 0; i < 6; ++i)
        {
            SCOPED_TRACE("feature = 0, i = " + std::to_string(i));
            EXPECT_PT_NEAR(part.at<shpxx::coordinate_xyzm_t>(i), expected_points[i]);
        }
    }

    {
        const auto maybe_feature = shapefile.at<shpxx::feature::polygon_t>(1);
        ASSERT_TRUE(maybe_feature);
        const auto& feature = *maybe_feature;

        const auto expected_min_bound = shpxx::coordinate_xyzm_t{-96.72243, 43.55605, 0, 0};
        const auto expected_max_bound = shpxx::coordinate_xyzm_t{-96.72211, 43.55627, 0, 0};

        const std::vector<shpxx::coordinate_xyzm_t> expected_points{
            {-96.722437459244418, 43.556067573506645, 0, 0},
            {-96.722186116243236, 43.556275066334315, 0, 0},
            {-96.722118092220001, 43.556231618230868, 0, 0},
            {-96.722130006031975, 43.556218806604647, 0, 0},
            {-96.722184963293671, 43.556246379449021, 0, 0},
            {-96.722416706152586, 43.556053369273521, 0, 0},
            {-96.722437459244418, 43.556067573506645, 0, 0}};

        EXPECT_PT_NEAR(feature.min_bound(), expected_min_bound);
        EXPECT_PT_NEAR(feature.max_bound(), expected_max_bound);

        ASSERT_EQ(feature.num_parts(), 1);
        EXPECT_EQ(feature.num_points(), expected_points.size());

        EXPECT_THROW(SHPXX_UNUSED(feature.at(1)), std::out_of_range);

        const auto part = feature.at(0);
        ASSERT_EQ(part.size(), expected_points.size());

        for (std::size_t i = 0; i < 6; ++i)
        {
            SCOPED_TRACE("feature = 1, i = " + std::to_string(i));
            EXPECT_PT_NEAR(part.at<shpxx::coordinate_xyzm_t>(i), expected_points[i]);
        }
    }

    {
        const auto maybe_feature = shapefile.at<shpxx::feature::polygon_t>(2);
        ASSERT_TRUE(maybe_feature);
        const auto& feature = *maybe_feature;

        const auto expected_min_bound = shpxx::coordinate_xyzm_t{-96.72412, 43.55302, 0, 0};
        const auto expected_max_bound = shpxx::coordinate_xyzm_t{-96.72296, 43.55482, 0, 0};

        const std::vector<std::vector<shpxx::coordinate_xyzm_t>> expected_points{
            {{-96.724127298963410, 43.554104572450768, 0, 0},
             {-96.723323308812823, 43.554829843058769, 0, 0},
             {-96.723077346242889, 43.554613711102427, 0, 0},
             {-96.723000482939739, 43.554220438079980, 0, 0},
             {-96.722960514022120, 43.553788169020827, 0, 0},
             {-96.723020467398555, 43.553217747372621, 0, 0},
             {-96.723066585380437, 43.553026120138611, 0, 0},
             {-96.723795249493932, 43.553290164482640, 0, 0},
             {-96.723758355108444, 43.553390434183626, 0, 0},
             {-96.723782951365436, 43.553567576914325, 0, 0},
             {-96.723853665604310, 43.553734692220665, 0, 0},
             {-96.723990482283838, 43.553951941426106, 0, 0},
             {-96.724127298963410, 43.554104572450768, 0, 0}},
            {{-96.723274116298839, 43.554190357601492, 0, 0},
             {-96.723264892702446, 43.553907377550395, 0, 0},
             {-96.723200327527834, 43.553839417576455, 0, 0},
             {-96.723161895876302, 43.553833847083382, 0, 0},
             {-96.723146523215675, 43.553873954621949, 0, 0},
             {-96.723183417601163, 43.553969766967178, 0, 0},
             {-96.723180343069032, 43.554156934830019, 0, 0},
             {-96.723204939326024, 43.554244948088382, 0, 0},
             {-96.723274116298839, 43.554190357601492, 0, 0}},
            {{-96.723530839731225, 43.554213753530497, 0, 0},
             {-96.723473960886935, 43.554228236720093, 0, 0},
             {-96.723380187657128, 43.554213753530497, 0, 0},
             {-96.723244908243643, 43.554277256720589, 0, 0},
             {-96.723241833711540, 43.554331847128758, 0, 0},
             {-96.723249520041819, 43.554377524779213, 0, 0},
             {-96.723277190830970, 43.554465537715402, 0, 0},
             {-96.723317159748561, 43.554459967280216, 0, 0},
             {-96.723378650391084, 43.554270572177423, 0, 0},
             {-96.723484721749372, 43.554269458086814, 0, 0},
             {-96.723541600593677, 43.554279484901485, 0, 0},
             {-96.723530839731225, 43.554213753530497, 0, 0}}};

        EXPECT_PT_NEAR(feature.min_bound(), expected_min_bound);
        EXPECT_PT_NEAR(feature.max_bound(), expected_max_bound);

        ASSERT_EQ(feature.num_parts(), expected_points.size());

        EXPECT_THROW(SHPXX_UNUSED(feature.at(4)), std::out_of_range);

        for (std::size_t j = 0; j < 3; ++j)
        {
            const auto part = feature.at(j);
            ASSERT_EQ(part.size(), expected_points[j].size());

            for (std::size_t i = 0; i < 6; ++i)
            {
                SCOPED_TRACE("feature = 2, part = " + std::to_string(j) +
                             ", i = " + std::to_string(i));
                EXPECT_PT_NEAR(part.at<shpxx::coordinate_xyzm_t>(i), expected_points[j][i]);
            }
        }
    }

}
