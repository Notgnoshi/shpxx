#include "shpxx/feature/multi_point.hpp"

#include "shp_casts.hpp"
#include "shplib_helpers.hpp"
#include "shpxx/concepts/is_coordinate.hpp"
#include "shpxx/coordinate.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

#include <shapefil.h>

namespace shpxx::feature {

multi_point_t::multi_point_t(shplib::opaque_object_t object) : m_underlying(std::move(object))
{
    const auto* shp_object = cast_shp_object(m_underlying.get());

    m_m_is_present = static_cast<bool>(shp_object->bMeasureIsUsed);
    m_type = static_cast<shape_type>(shp_object->nSHPType);
    m_points = shp_object->nVertices;

    m_min_bound = shplib::get_min_bound(shp_object);
    m_max_bound = shplib::get_max_bound(shp_object);
}

bool multi_point_t::is_compatible(shape_type t) noexcept
{
    return t == shape_type::multi_point || t == shape_type::multi_point_m ||
           t == shape_type::multi_point_z;
}

coordinate_xyzm_t multi_point_t::min_bound() const noexcept { return m_min_bound; }
coordinate_xyzm_t multi_point_t::max_bound() const noexcept { return m_max_bound; }

bool multi_point_t::has_z() const noexcept { return m_type == shape_type::multi_point_z; }
bool multi_point_t::has_m() const noexcept { return m_m_is_present; }

std::size_t multi_point_t::num_points() const noexcept { return m_points; }

coordinate_xyzm_t multi_point_t::at(std::size_t index) const
{
    const auto* shp_object = cast_shp_object(m_underlying.get());

    coordinate_xyzm_t coord{};

    // NOLINTBEGIN (*pointer-arithmetic)
    coord.x = shp_object->padfX[index];
    coord.y = shp_object->padfY[index];
    coord.z = has_z() ? shp_object->padfZ[index] : 0;
    coord.m = has_m() ? shp_object->padfM[index] : 0;
    // NOLINTEND

    return coord;
}

}  // namespace shpxx::feature
