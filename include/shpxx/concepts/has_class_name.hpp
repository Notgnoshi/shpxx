#pragma once

#include <concepts>
#include <string>

namespace shpxx::concepts {

template<class ObjectT>
concept HasClassName = requires() {
    {
        ObjectT::class_name()
    } -> std::convertible_to<std::string>;
};

}  // namespace shpxx::concepts
