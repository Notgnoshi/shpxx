#pragma once

#include "shpxx/io/openmode.hpp"

#include <concepts>
#include <ios>
#include <string_view>
#include <utility>

namespace shpxx::io {
using handle_t = void*;
using offset_t = unsigned long;

//! @brief Constraint for types which can be wrapped by a SAHooks object for
//! reading/writing data
template<class HandlerT>
concept IoHandler = requires {
    // clang-format does weird things with constraints :/
    // clang-format off

    //! @brief Open a file
    //!
    //! @param[in] filename Name of the file to open.
    //!
    //! @param[in] mode Mode to open the file in. It is not strictly required to
    //! enforce the mode (I think?)
    //!
    //! @return An opaque pointer used as a handle to the file. This will be
    //! passed into the other functions below, eventually ending with close().
    //! Returns nullptr on error.
    {
        HandlerT::open(
            std::declval<std::string_view>(),
            std::declval<std::ios::openmode>()
        )
    } -> std::same_as<handle_t>;

    //! @brief Close a file
    //!
    //! If the file handle is associated with any heap memory, that should be
    //! freed at this time.
    //!
    //! @param[in] handle The file handle to close.
    //!
    //! @return true on successful close; otherwise false
    {
        HandlerT::close(std::declval<handle_t>())
    } -> std::same_as<bool>;

    //! @brief Delete a file
    //!
    //! It will be implementation-defined what this means in the case that the
    //! underlying data representation is not actually a filesystem (for
    //! example, a collection of memmapped files).
    //!
    //! @param[in] filename Name of the file to delete.
    //!
    //! @return 0 on success, otherwise non-0
    {
        HandlerT::remove(std::declval<std::string_view>())
    } -> std::same_as<int>;

    //! @brief Read a block of data from a file
    //!
    //! @param[in] handle A handle that was returned by open() previously
    //!
    //! @param[in] buffer Pointer to memory location to store data that is read
    //!
    //! @param[in] length Number of bytes to read. This will not be 0 (that case
    //! will be handled without invoking the derived type).
    //!
    //! @return Number of bytes read from the file. This may be less than length if
    //! an error occurred.
    {
        HandlerT::read(
            std::declval<handle_t>(),
            std::declval<uint8_t*>(),
            std::declval<offset_t>()
        )
    } -> std::same_as<offset_t>;

    //! @brief Write a block of data to a file
    //!
    //! @param[in] handle A handle that was returned by open() previously
    //!
    //! @param[in] buffer Pointer to memory location of data to store into the file
    //!
    //! @param[in] length Number of bytes to write to file. This will not be 0 (that case
    //! will be handled without invoking the derived type).
    //!
    //! @return Number of bytes written. This may be less than length if an error occurred
    {
        HandlerT::write(
            std::declval<handle_t>(),
            std::declval<const uint8_t*>(),
            std::declval<offset_t>()
        )
    } -> std::same_as<offset_t>;

    //! @brief Move the read/write pointer to a new location in the file
    //!
    //! @param[in] handle A handle that was returned by open() previously
    //!
    //! @param[in] offset Distance to seek
    //!
    //! @param[in] dir How to seek (from beginning, end, or current location)
    //!
    //! @return 0 on success, otherwise non-0
    {
        HandlerT::seek(
            std::declval<handle_t>(),
            std::declval<offset_t>(),
            std::declval<std::ios::seekdir>()
        )
    } -> std::same_as<int>;

    //! @brief Get the location of the read/write pointer in the file
    //!
    //! @return Location of the read/write pointer in the file. Returns
    //! -1 on failure (which wraps to offset_t max value if offset is unsigned)
    {
        HandlerT::tell(std::declval<handle_t>())
    } -> std::same_as<offset_t>;

    //! @brief Flush the contents of the file to disk
    //!
    //! @return True on success, otherwise false
    {
        HandlerT::flush(std::declval<handle_t>())
    } -> std::same_as<bool>;

    // clang-format on
};
}  // namespace shpxx::io
