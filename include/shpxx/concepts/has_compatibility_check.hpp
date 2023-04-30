#pragma once

#include "shpxx/shape_type.hpp"

#include <concepts>

namespace shpxx::concepts {

template<class ObjectT>
concept HasCompatibilityCheck = requires() {
    {
        ObjectT::is_compatible(std::declval<shape_type>())
    } -> std::convertible_to<bool>;
};

}  // namespace shpxx::concepts
