#include "shpxx/feature/polyline_part.hpp"

#include "shpxx/coordinate.hpp"

#include <cassert>
#include <cstdint>
#include <stdexcept>

namespace shpxx::feature {

polyline_part_t::polyline_part_t(std::size_t size,
                                 const double* x_values,
                                 const double* y_values,
                                 const double* z_values,
                                 const double* m_values) :
    m_size(size),
    m_x_values(x_values),
    m_y_values(y_values),
    m_z_values(z_values),
    m_m_values(m_values)
{
}

std::size_t polyline_part_t::size() const noexcept { return m_size; }

coordinate_xyzm_t polyline_part_t::at(std::size_t index) const
{
    if (index >= m_size)
    {
        throw std::out_of_range("index greater than size of part");
    }

    coordinate_xyzm_t coord{};

    // NOLINTBEGIN (*pointer-arithmetic)
    coord.x = m_x_values[index];
    coord.y = m_y_values[index];
    coord.z = m_z_values ? m_z_values[index] : 0;
    coord.m = m_m_values ? m_m_values[index] : 0;
    // NOLINTEND

    return coord;
}

}  // namespace shpxx::feature
