#pragma once

#include <memory>

namespace shpxx {

//! Shapelib defines an anonymous struct typedef SHPInfo as the struct containing
//! an opened file's data. Unfortunately, we can't just forward-declare it an
//! use it as an opaque pointer, because it's a typedef.
//!
//! https://stackoverflow.com/questions/18501993/struct-forward-declaration-error-typedef-redefinition-with-different-types
//!
//! This type exists as a stand-in in the context of this library. The public
//! API will use pointers of type shp_info, and those will be cast to/from the
//! appropriate shapelib type in the source code where it is used.
struct shp_info;

namespace shp_handle {
    //! @brief Functor which closes a shpfile
    struct close_shp_t
    {
        void operator()(shp_info*) const;
    };

}  // namespace shp_handle

using shp_handle_t = std::unique_ptr<shp_info, shp_handle::close_shp_t>;

}  // namespace shpxx
