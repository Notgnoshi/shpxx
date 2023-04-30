#pragma once

#include "shpxx/feature/arc.hpp"
#include "shpxx/feature/multi_patch.hpp"
#include "shpxx/feature/multi_point.hpp"
#include "shpxx/feature/point.hpp"
#include "shpxx/feature/polygon.hpp"

#include <concepts>

namespace shpxx::concepts {

template<class ObjectT>
concept IsFeature = std::same_as<ObjectT, shpxx::feature::point_t> ||
                    std::same_as<ObjectT, shpxx::feature::multi_point_t> ||
                    std::same_as<ObjectT, shpxx::feature::arc_t> ||
                    std::same_as<ObjectT, shpxx::feature::polygon_t> ||
                    std::same_as<ObjectT, shpxx::feature::multi_patch_t>;

}  // namespace shpxx::concepts
