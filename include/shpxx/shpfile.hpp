#pragma once

#include "shpxx/geometry/point.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

#include <cstdint>

namespace shpxx {

//! @brief Handle representing an opened shp file
//!
//! The shpfile_t can be used to query file-level details and access the
//! individual elements of the file.
class shpfile_t
{
  public:
    //! @brief Construct a shpfile_t for a specific file handle
    //!
    //! The shpfile_t will take ownership of the file handle and close it when
    //! it is destroyed.
    //!
    //! @param[in] handle Pointer to the handle for the opened file. Must not be
    //! null.
    explicit shpfile_t(shp_handle_t handle);
    ~shpfile_t() = default;

    shpfile_t(const shpfile_t&) = delete;
    shpfile_t& operator=(const shpfile_t&) = delete;

    shpfile_t(shpfile_t&&) = default;
    shpfile_t& operator=(shpfile_t&&) = default;

    //! @brief Get the number of elements in the file, including NULL elements
    [[nodiscard]] std::size_t size() const noexcept;

    //! @brief Get the type of shapes in the file
    //!
    //! @note According to the SHP standard, any given file is only allowed to
    //! contain one type of shape. However, it's not uncommon for user-data to
    //! contain multiple types, and there's technically nothing that prevents
    //! creating a non-standard file that does so. This returns the type that
    //! the file header indicates is stored, but there is no guarantee that it
    //! actually holds true for all shapes in the file.
    [[nodiscard]] shape_type type() const noexcept;

    //! @brief Minimum x, y, z, m values of the file
    //!
    //! @note These are the values stored in the file header, and as such may
    //! not accurately reflect the data in the file.
    //!
    //! @return A point_m_t containing the minimum bound values. The values of
    //! z and m should be 0 when they are unused, but this is not guaranteed.
    [[nodiscard]] geometry::point_xyzm_t min_bound() const noexcept;

    //! @brief Maximum x, y, z, m values of the file
    //!
    //! @note These are the values stored in the file header, and as such may
    //! not accurately reflect the data in the file.
    //!
    //! @return A point_xyzm_t containing the minimum bound values. The values of
    //! z and m should be 0 when they are unused, but this is not guaranteed.
    [[nodiscard]] geometry::point_xyzm_t max_bound() const noexcept;

  private:
    shp_handle_t m_handle;

    std::size_t m_num_shapes = 0;
    shape_type m_shape_type = shape_type::null;
    geometry::point_xyzm_t m_min_bound = {};
    geometry::point_xyzm_t m_max_bound = {};
};
}  // namespace shpxx
