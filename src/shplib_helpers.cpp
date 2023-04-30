#include "shplib_helpers.hpp"

namespace shpxx::shplib {
coordinate_xyzm_t get_min_bound(const SHPObject* obj)
{
    return {obj->dfXMin, obj->dfYMin, obj->dfZMin, obj->dfMMin};
}

coordinate_xyzm_t get_max_bound(const SHPObject* obj)
{
    return {obj->dfXMax, obj->dfYMax, obj->dfZMax, obj->dfMMax};
}

}  // namespace shpxx::shplib
