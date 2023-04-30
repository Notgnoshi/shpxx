#pragma once

#include "shpxx/concepts/has_class_name.hpp"
#include "shpxx/concepts/has_compatibility_check.hpp"
#include "shpxx/concepts/is_feature.hpp"
#include "shpxx/geometry/point.hpp"
#include "shpxx/shape_type.hpp"
#include "shpxx/shp_handle.hpp"

#include <cstdint>
#include <optional>

namespace shpxx {

//! @brief Handle representing an opened shp file
//!
//! The shpfile_t can be used to query file-level details and access the
//! individual elements of the file.
class shpfile_t
{
  public:
    //! @brief Read-Only iterator for features in a file
    //!
    //! Type of the features is defined at the point the shape iterator is created
    //! from the top-level shape file object.
    template<shpxx::concepts::IsFeature FeatureT>
    class const_feature_iterator_t
    {
      public:
        using value_type = std::optional<FeatureT>;
        using reference = const std::optional<FeatureT>&;
        using pointer = const std::optional<FeatureT>*;
        using difference_type = std::int32_t;
        using iterator_category = std::forward_iterator_tag;

        //! @todo Make this a proper random-access iterator?
        //!
        //! shplib requires that SHX files exist, which means the internal record of
        //! offsets can be built.

        reference operator*() const noexcept;
        pointer operator->() const noexcept;

        const_feature_iterator_t& operator++() noexcept;
        const_feature_iterator_t operator++(int) noexcept;

        bool operator==(const_feature_iterator_t<FeatureT> other) const noexcept;
        bool operator!=(const_feature_iterator_t<FeatureT> other) const noexcept;

      private:
        explicit const_feature_iterator_t(const shpfile_t& file);
        const_feature_iterator_t(const shpfile_t& file, std::size_t index);

        const shpfile_t& m_file;
        std::size_t m_curr_index;
        std::optional<FeatureT> m_curr_obj;
    };

    //! @brief Construct a shpfile_t for a specific file handle
    //!
    //! The shpfile_t will take ownership of the file handle and close it when
    //! it is destroyed.
    //!
    //! @note shpxx users should use one of the functions defined in shpxx::io
    //! to open a shapefile and obtain a shpfile_t.
    //!
    //! @param[in] handle Pointer to the handle for the opened file. Must not be
    //! null.
    explicit shpfile_t(shplib::opaque_file_t handle);
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

    //! @brief Create an read-only iterator pointing to the beginning of the
    //! file
    //!
    //! @tparam FeatureT The feature type that all data in the file should be
    //! converted to. This must be compatible with the shape_type returned by
    //! type()
    //!
    //! @throw incompatible_feature_type_error_t if FeatureT is not compatible
    //! with the type of features indicated by the file header
    //!
    //! @return A new feature iterator which can be used to access the features
    //! of the file as a specific feature type
    template<shpxx::concepts::IsFeature FeatureT>
    [[nodiscard]] const_feature_iterator_t<FeatureT> begin() const;

    //! @brief @see begin()
    template<shpxx::concepts::IsFeature FeatureT>
    [[nodiscard]] const_feature_iterator_t<FeatureT> cbegin() const
    {
        return begin<FeatureT>();
    }

    //! @brief Create an read-only iterator pointing past-the-end of the file
    //!
    //! @tparam FeatureT The feature type that all data in the file should be
    //! converted to. This must be compatible with the shape_type returned by
    //! type()
    //!
    //! @throw incompatible_feature_type_error_t if FeatureT is not compatible
    //! with the type of features indicated by the file header
    //!
    //! @return A new feature iterator which cannot be dereferenced, but can be
    //! used as an end iterator for reading through the file
    template<shpxx::concepts::IsFeature FeatureT>
    [[nodiscard]] const_feature_iterator_t<FeatureT> end() const;

    //! @brief @see end()
    template<shpxx::concepts::IsFeature FeatureT>
    [[nodiscard]] const_feature_iterator_t<FeatureT> cend() const
    {
        return end<FeatureT>();
    }

    //! @brief Get the shape at a specific index
    //!
    //! @tparam FeatureT The feature type that the data in the file should be
    //! converted to. This must be compatible with the shape_type returned by
    //! type()
    //!
    //! @throw incompatible_feature_type_error_t if FeatureT is not compatible
    //! with the type of features indicated by the file header
    //!
    //! @throw std::out_of_range if the index is past the end of the file
    //!
    //! @note This currently returns a copy of the feature feature indicated,
    //! but it may change eventually to return a const& that is valid as long as
    //! the shpfile_t remains in scope
    //!
    //! @return A copy of the feature indicated, or a null option if the shape
    //! at the given index is null
    template<shpxx::concepts::IsFeature FeatureT>
    [[nodiscard]] std::optional<FeatureT> at(std::size_t index) const;

  private:
    shplib::opaque_file_t m_handle;

    std::size_t m_num_shapes = 0;
    shape_type m_shape_type = shape_type::null;
    geometry::point_xyzm_t m_min_bound = {};
    geometry::point_xyzm_t m_max_bound = {};

    //! @brief Read the object at a specific index
    //!
    //! @param index Index of the object to read
    //!
    //! @return The underlying shplib object, or a null option if the feature is
    //! null
    [[nodiscard]] std::optional<shplib::opaque_object_t>
    read_index(std::size_t index) const noexcept;

    //! @brief Throw incompatible_feature_type_error_t if the template type is
    //! not compatible with this file
    template<shpxx::concepts::HasCompatibilityCheck CompatibilityCheckT>
        requires shpxx::concepts::HasClassName<CompatibilityCheckT>
    void throw_if_incompatible() const;
};
}  // namespace shpxx

#include "shpxx/shpfile_impl.hpp"
