#pragma once

#include "shpxx/concepts/is_coordinate.hpp"
#include "shpxx/coordinate.hpp"
#include "shpxx/coordinate_cast.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

namespace shpxx::feature {
class multi_point_t
{
  public:
    explicit multi_point_t(shplib::opaque_object_t);

    [[nodiscard]] static bool is_compatible(shape_type t) noexcept;
    [[nodiscard]] static std::string class_name() noexcept
    {
        return "shpxx::feature::multi_point_t";
    }

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

    //! @brief Get the total number of points from all parts of the feature
    [[nodiscard]] std::size_t num_points() const noexcept;

    //! @brief Get the part at a specific index
    template<concepts::IsCoordinate CoordT>
    [[nodiscard]] CoordT at(std::size_t index) const
    {
        return coordinate_cast<CoordT>(at(index));
    }

  private:
    shplib::opaque_object_t m_underlying;

    shape_type m_type = shape_type::null;
    bool m_m_is_present = false;

    std::size_t m_points = 0;

    coordinate_xyzm_t m_min_bound{};
    coordinate_xyzm_t m_max_bound{};

    [[nodiscard]] coordinate_xyzm_t at(std::size_t index) const;
};
}  // namespace shpxx::feature
