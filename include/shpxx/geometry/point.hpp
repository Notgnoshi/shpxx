#pragma once

namespace shpxx::geometry {
struct point_xy_t
{
    double x;
    double y;
};

struct point_xym_t
{
    double x;
    double y;
    double m;
};

struct point_xyz_t
{
    double x;
    double y;
    double z;
};

struct point_xyzm_t
{
    double x;
    double y;
    double z;
    double m;
};
}  // namespace shpxx::geometry
