#include "shpxx/shpfile.hpp"

#include "shp_casts.hpp"
#include "shpxx/shp_handle.hpp"

#include <shapefil.h>

#include <array>
#include <cassert>

namespace shpxx {
//! @brief Convert an array of doubles into a point
static coordinate_xyzm_t make_point(const std::array<double, 4>& data);

shpfile_t::shpfile_t(shplib::opaque_file_t handle) : m_handle(std::move(handle))
{
    assert(m_handle);

    int number_of_shapes = 0;
    int shape_type = SHPT_NULL;
    std::array<double, 4> min_bound{};
    std::array<double, 4> max_bound{};

    SHPGetInfo(cast_shp_info(m_handle.get()),
               &number_of_shapes,
               &shape_type,
               min_bound.data(),
               max_bound.data());

    m_shape_type = static_cast<shpxx::shape_type>(shape_type);
    m_num_shapes = static_cast<std::size_t>(number_of_shapes);
    m_min_bound = make_point(min_bound);
    m_max_bound = make_point(max_bound);
}

std::size_t shpfile_t::size() const noexcept { return m_num_shapes; }
shape_type shpfile_t::type() const noexcept { return m_shape_type; }
coordinate_xyzm_t shpfile_t::min_bound() const noexcept { return m_min_bound; }
coordinate_xyzm_t shpfile_t::max_bound() const noexcept { return m_max_bound; }

coordinate_xyzm_t make_point(const std::array<double, 4>& data)
{
    coordinate_xyzm_t point{};
    point.x = data[0];
    point.y = data[1];
    point.z = data[2];
    point.m = data[3];
    return point;
}

}  // namespace shpxx
