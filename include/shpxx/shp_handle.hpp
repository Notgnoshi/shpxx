#pragma once

#include <memory>

//! Namespace for shim operations into shplib that are necessary to use within
//! the headers of shpxx - any operations performed in source files can be done
//! by casting these opaque pointers and then using them directly.
//!
//! This allows us to avoid including any of shplib in the public headers of
//! shpxx.
namespace shpxx::shplib {

//! Shapelib defines an anonymous struct typedefs as the structs containing
//! an opened file's data. Unfortunately, we can't just forward-declare those
//! types and use them as opaque pointers, because they are typedefs.
//!
//! https://stackoverflow.com/questions/18501993/struct-forward-declaration-error-typedef-redefinition-with-different-types
//!
//! These types exist as stand-ins in the context of this library. The public
//! API will use pointers of these types, and those will be cast to/from the
//! appropriate shapelib types in the source code where they are used.
//!
//! @{
struct shp_info;
struct shp_object;
//! @}

//! @brief Functor which closes a shpfile
struct close_file_t
{
    void operator()(shp_info*) const;
};

//! @brief Functor destroys an object
struct destroy_object_t
{
    void operator()(shp_object*) const;
};

using opaque_file_t = std::unique_ptr<shp_info, close_file_t>;
using opaque_object_t = std::unique_ptr<shp_object, destroy_object_t>;

}  // namespace shpxx::shplib
