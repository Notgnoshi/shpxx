#include "shpxx/feature/point.hpp"

#include "shp_casts.hpp"
#include "shpxx/coordinate.hpp"
#include "shpxx/shape_type.hpp"

#include <shapefil.h>

namespace shpxx::feature {

point_t::point_t(shplib::opaque_object_t object) : m_underlying(std::move(object))
{
    const auto* shp_object = cast_shp_object(m_underlying.get());

    // Regardless of the shape type, shplib always makes sure there's four
    // arrays here, and initializes Z, M to 0 when they're not present.

    m_data.x = shp_object->padfX[0];  // NOLINT(*pointer-arithmetic)
    m_data.y = shp_object->padfY[0];  // NOLINT(*pointer-arithmetic)
    m_data.z = shp_object->padfZ[0];  // NOLINT(*pointer-arithmetic)
    m_data.m = shp_object->padfM[0];  // NOLINT(*pointer-arithmetic)

    m_m_is_present = static_cast<bool>(shp_object->bMeasureIsUsed);
    m_type = static_cast<shape_type>(shp_object->nSHPType);
}

bool point_t::is_compatible(shape_type t) noexcept
{
    return t == shape_type::point || t == shape_type::point_m || t == shape_type::point_z;
}

bool point_t::has_z() const noexcept { return m_type == shape_type::point_z; }
bool point_t::has_m() const noexcept { return m_m_is_present; }

}  // namespace shpxx::feature
