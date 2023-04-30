#pragma once

#include "shpxx/exceptions.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"
#include "shpxx/shpfile.hpp"

#include <cstddef>
#include <limits>
#include <optional>
#include <stdexcept>

namespace shpxx {

template<shpxx::concepts::IsFeature FeatureT>
auto shpfile_t::at(std::size_t index) const -> std::optional<FeatureT>
{
    if (index >= size())
    {
        throw std::out_of_range("index greater than size of shapefile");
    }

    auto object = read_index(index);
    if (!object)
    {
        return std::nullopt;
    }

    throw_if_incompatible<FeatureT>();

    return FeatureT(*object);
}

template<shpxx::concepts::HasCompatibilityCheck CompatibilityCheckT>
void shpfile_t::throw_if_incompatible() const
{
    if (!CompatibilityCheckT::is_compatible(type()))
    {
        throw incompatible_feature_type_error_t(
            "specified feature type is not compatible with file");
    }
}


}  // namespace shpxx
