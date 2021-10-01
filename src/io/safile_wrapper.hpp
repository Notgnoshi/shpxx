#include "io/io_handler_concept.hpp"
#include "io/openmode_internal.hpp"
#include "shpxx/exceptions.hpp"
#include "shpxx/io/openmode.hpp"

#include <shapefil.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ios>
#include <type_traits>

namespace shpxx::io {

namespace detail {
    std::ios::seekdir to_seekdir(int dir)
    {
        switch (dir)
        {
        case SEEK_CUR:
            return std::ios::cur;
        case SEEK_END:
            return std::ios::end;
        case SEEK_SET:
        default:
            return std::ios::beg;
        }
    }
}  // namespace detail

//! @brief Create SAHooks object wrapping some type which provides the implementation
//!
//! The hooks produced will attempt to call static functions on the type passed as the
//! template parameter. The hooks produced will convert to/from the specific typedefs
//! used in the SAFile struct.
//!
//! SHPOpenLL will use the SAHooks pointer directly, or copy the contents
//! of the SAHooks struct; but it does not copy the pointer itself, so passing
//! the pointer to a stack var is acceptable
//!
//! Example:
//! @code
//!     auto hooks = make_sahooks<some_type_t>();
//!     return SHPOpenLL("a_file", "rb+", &hooks);
//! @endcode
template<IoHandler IoHandlerT>
SAHooks make_sahooks()
{
    SAHooks hooks;

    static_assert(std::is_pointer_v<SAFile>, "SAFile not a pointer");

    hooks.FOpen = [](const char* file_name, const char* perms) {
        return static_cast<SAFile>(IoHandlerT::open(file_name, io::to_openmode(perms)));
    };

    hooks.FRead = [](void* buffer, SAOffset object_size, SAOffset count, SAFile file) {
        const auto bytes_read = IoHandlerT::read(static_cast<handle_t>(file),
                                                 static_cast<uint8_t*>(buffer),
                                                 static_cast<offset_t>(object_size * count));

        return static_cast<SAOffset>(bytes_read / object_size);
    };

    hooks.FWrite = [](void* buffer, SAOffset object_size, SAOffset count, SAFile file) {
        const auto bytes_written = IoHandlerT::write(static_cast<handle_t>(file),
                                                     static_cast<const uint8_t*>(buffer),
                                                     static_cast<offset_t>(object_size * count));

        return static_cast<SAOffset>(bytes_written / object_size);
    };

    hooks.FSeek = [](SAFile file, SAOffset offset, int origin) {
        return static_cast<SAOffset>(IoHandlerT::seek(static_cast<handle_t>(file),
                                                      static_cast<offset_t>(offset),
                                                      detail::to_seekdir(origin)));
    };

    hooks.FTell = [](SAFile file) {
        return static_cast<SAOffset>(IoHandlerT::tell(static_cast<handle_t>(file)));
    };

    hooks.FFlush = [](SAFile file) {
        return IoHandlerT::flush(static_cast<handle_t>(file)) ? 0 : EOF;
    };

    hooks.FClose = [](SAFile file) {
        return IoHandlerT::close(static_cast<handle_t>(file)) ? 0 : EOF;
    };

    hooks.Remove = [](const char* file_name) {
        return IoHandlerT::remove(file_name);
    };

    hooks.Error = [](const char* error) {
        throw shpxx::invalid_file_error_t(error);
    };

    hooks.Atof = std::atof;

    return hooks;
}

}  // namespace shpxx::io
