#include "shpxx/shp_handle.hpp"

#include <shapefil.h>

namespace shpxx {
//! @brief Cast shpxx::shp_info type to the SHPInfo struct
inline SHPInfo* cast_shp_info(shplib::shp_info* info)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<SHPInfo*>(info);
}

//! @brief Cast the SHPInfo struct to shpxx::shp_info
inline shplib::shp_info* cast_shp_info(SHPInfo* info)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<shplib::shp_info*>(info);
}

//! @brief Cast shpxx::shp_object type to the SHPInfo struct
inline SHPObject* cast_shp_object(shplib::shp_object* info)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<SHPObject*>(info);
}

//! @brief Cast the SHPInfo struct to shpxx::shp_info
inline shplib::shp_object* cast_shp_object(SHPObject* info)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<shplib::shp_object*>(info);
}
}  // namespace shpxx
