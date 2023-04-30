#include "shpxx/feature/linestring.hpp"

#include "shp_casts.hpp"
#include "shplib_helpers.hpp"
#include "shpxx/feature/polyline_part.hpp"
#include "shpxx/shape_type.hpp"

#include <shapefil.h>

namespace shpxx::feature {

linestring_t::linestring_t(shplib::opaque_object_t feature) : m_underlying(std::move(feature))
{
    const auto* shp_object = cast_shp_object(m_underlying.get());

    m_m_is_present = static_cast<bool>(shp_object->bMeasureIsUsed);
    m_type = static_cast<shape_type>(shp_object->nSHPType);
    m_parts = shp_object->nParts;
    m_points = shp_object->nVertices;

    m_min_bound = shplib::get_min_bound(shp_object);
    m_max_bound = shplib::get_max_bound(shp_object);
}

bool linestring_t::is_compatible(shape_type t) noexcept
{
    return t == shape_type::linestring || t == shape_type::linestring_z ||
           t == shape_type::linestring_m;
}

coordinate_xyzm_t linestring_t::min_bound() const noexcept { return m_min_bound; }
coordinate_xyzm_t linestring_t::max_bound() const noexcept { return m_max_bound; }

bool linestring_t::has_z() const noexcept { return m_type == shape_type::linestring_z; }
bool linestring_t::has_m() const noexcept { return m_m_is_present; }

std::size_t linestring_t::num_parts() const noexcept { return m_parts; }
std::size_t linestring_t::num_points() const noexcept { return m_points; }

polyline_part_t linestring_t::at(std::size_t index) const
{
    if (index >= m_parts)
    {
        throw std::out_of_range("part index greater than size of shape");
    }

    const auto* shp_object = cast_shp_object(m_underlying.get());

    // NOLINTBEGIN (*pointer-arithmetic)
    const auto part_start = shp_object->panPartStart[index];
    const auto part_end = index < m_parts - 1 ? shp_object->panPartStart[index + 1] : m_points;
    const auto part_size = part_end - part_start;

    const double* const x_start = shp_object->padfX + part_start;
    const double* const y_start = shp_object->padfY + part_start;
    const double* const z_start = has_z() ? shp_object->padfZ + part_start : nullptr;
    const double* const m_start = has_m() ? shp_object->padfM + part_start : nullptr;
    // NOLINTEND

    return {part_size, x_start, y_start, z_start, m_start};
}

}  // namespace shpxx::feature
