#pragma once

#include "shpxx/concepts/is_coordinate.hpp"

#include <concepts>

namespace shpxx::concepts {

template<class ObjectT>
concept HasZ = IsCoordinate<ObjectT> && requires(ObjectT o) {
    {
        o.z
    };
};

template<class ObjectT>
concept HasM = IsCoordinate<ObjectT> && requires(ObjectT o) {
    {
        o.m
    };
};

}  // namespace shpxx::concepts
