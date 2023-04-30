#pragma once

#include "shpxx/concepts/is_coordinate.hpp"
#include "shpxx/coordinate.hpp"

#include <cstdint>

namespace shpxx::feature {
//! @brief One "part" of a linestring of polygon shape
//!
//! @note Instances of this type are only valid while the feature object
//! that they reference remains in scope.
class polyline_part_t
{
  public:
    polyline_part_t(std::size_t size,
                    const double* x_values,
                    const double* y_values,
                    const double* z_values = nullptr,
                    const double* m_values = nullptr);

    [[nodiscard]] std::size_t size() const noexcept;

    template<concepts::IsCoordinate CoordT>
    [[nodiscard]] CoordT at(std::size_t index) const
    {
        return coordinate_cast<CoordT>(at(index));
    }

  private:
    [[nodiscard]] coordinate_xyzm_t at(std::size_t index) const;

    std::size_t m_size = 0;

    const double* m_x_values = nullptr;
    const double* m_y_values = nullptr;
    const double* m_z_values = nullptr;
    const double* m_m_values = nullptr;
};
}  // namespace shpxx::feature
