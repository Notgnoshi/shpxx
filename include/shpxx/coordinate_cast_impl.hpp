#pragma once

#include "shpxx/concepts/has_zm.hpp"
#include "shpxx/concepts/is_coordinate.hpp"
#include "shpxx/coordinate.hpp"
#include "shpxx/coordinate_cast.hpp"

namespace shpxx {
namespace internal {
    template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
        requires concepts::HasZ<ToCoordT> && concepts::HasZ<FromCoordT>
    void copy_z_if_present(ToCoordT& out, FromCoordT in)
    {
        out.z = in.z;
    }

    template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
        requires concepts::HasZ<ToCoordT> && (!concepts::HasZ<FromCoordT>)
    void copy_z_if_present(ToCoordT& out, FromCoordT in)
    {
        out.z = 0;
    }

    template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
        requires(!concepts::HasZ<ToCoordT>)
    void copy_z_if_present(ToCoordT& out, FromCoordT in)
    {
    }

    template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
        requires concepts::HasM<ToCoordT> && concepts::HasM<FromCoordT>
    void copy_m_if_present(ToCoordT& out, FromCoordT in)
    {
        out.m = in.m;
    }

    template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
        requires concepts::HasM<ToCoordT> && (!concepts::HasM<FromCoordT>)
    void copy_z_if_present(ToCoordT& out, FromCoordT in)
    {
        out.m = 0;
    }

    template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
        requires(!concepts::HasM<ToCoordT>)
    void copy_m_if_present(ToCoordT& out, FromCoordT in)
    {
    }
}  // namespace internal

template<concepts::IsCoordinate ToCoordT, concepts::IsCoordinate FromCoordT>
ToCoordT coordinate_cast(FromCoordT from)
{
    ToCoordT result;
    result.x = from.x;
    result.y = from.y;
    copy_z_if_present(result, from);
    copy_m_if_present(result, from);
    return result;
}
}  // namespace shpxx
