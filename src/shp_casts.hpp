#include "shpxx/shp_handle.hpp"

#include <shapefil.h>

namespace shpxx {
//! @brief Cast shpxx::shp_info type to the SHPInfo struct
inline SHPInfo* cast_shp_info(shp_info* info)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<SHPInfo*>(info);
}

//! @brief Cast the SHPInfo struct to shpxx::shp_info
inline shp_info* cast_shp_info(SHPInfo* info)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<shp_info*>(info);
}
}  // namespace shpxx
