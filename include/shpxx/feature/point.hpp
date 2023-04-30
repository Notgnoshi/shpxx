#pragma once

#include "shpxx/concepts/is_coordinate.hpp"
#include "shpxx/coordinate.hpp"
#include "shpxx/coordinate_cast.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

namespace shpxx::feature {
class point_t
{
  public:
    explicit point_t(shplib::opaque_object_t);

    [[nodiscard]] static bool is_compatible(shape_type t) noexcept;
    [[nodiscard]] static std::string class_name() noexcept { return "shpxx::feature::point_t"; }

    //! @brief Check if the feature contains a Z value
    //!
    //! @note Presence of a Z value in the file does not guarantee that it is
    //! valid data.
    [[nodiscard]] bool has_z() const noexcept;

    //! @brief Check if the feature contains an M value
    //!
    //! @note Presence of an M value in the file does not guarantee that it is
    //! valid data.
    [[nodiscard]] bool has_m() const noexcept;

    //! @brief Get the coordinate contained in this feature
    //!
    //! @tparam The concrete coordinate type to retrieve the data as
    //!
    //! @note If the concrete type requested contains values that are not
    //! actually present (such as z, m) they will be defaulted to 0
    template<shpxx::concepts::IsCoordinate CoordT>
    [[nodiscard]] CoordT coordinate() const noexcept
    {
        return shpxx::coordinate_cast<CoordT>(m_data);
    }

  private:
    shplib::opaque_object_t m_underlying;

    bool m_m_is_present = false;
    shape_type m_type = shape_type::null;
    coordinate_xyzm_t m_data;
};
}  // namespace shpxx::feature
