#pragma once

#include "shpxx/coordinate.hpp"
#include "shpxx/feature/polyline_part.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

namespace shpxx::feature {
class polygon_t
{
  public:
    explicit polygon_t(shplib::opaque_object_t);

    [[nodiscard]] static bool is_compatible(shape_type t) noexcept;
    [[nodiscard]] static std::string class_name() noexcept { return "shpxx::feature::polygon_t"; }

    //! @brief Get the minimum x, y, z, m data for the shape
    //!
    //! @note This is the value reported in the shapefile, it may not actually
    //! match the minimum value across all points in the feature.
    [[nodiscard]] coordinate_xyzm_t min_bound() const noexcept;

    //! @brief Get the maximum x, y, z, m data for the shape
    //!
    //! @note This is the value reported in the shapefile, it may not actually
    //! match the maximum value across all points in the feature.
    [[nodiscard]] coordinate_xyzm_t max_bound() const noexcept;

    //! @brief Check if the feature contains Z values
    //!
    //! @note Presence of a Z value in the file does not guarantee that it is
    //! valid data.
    [[nodiscard]] bool has_z() const noexcept;

    //! @brief Check if the feature contains M values
    //!
    //! @note Presence of a M value in the file does not guarantee that it is
    //! valid data.
    [[nodiscard]] bool has_m() const noexcept;

    //! @brief Get the number of parts in the feature
    [[nodiscard]] std::size_t num_parts() const noexcept;

    //! @brief Get the total number of points from all parts of the feature
    [[nodiscard]] std::size_t num_points() const noexcept;

    //! @brief Get the part at a specific index
    [[nodiscard]] polyline_part_t at(std::size_t index) const;

  private:
    shplib::opaque_object_t m_underlying;

    shape_type m_type = shape_type::null;
    bool m_m_is_present = false;

    std::size_t m_parts = 0;
    std::size_t m_points = 0;

    coordinate_xyzm_t m_min_bound{};
    coordinate_xyzm_t m_max_bound{};
};
}  // namespace shpxx::feature
