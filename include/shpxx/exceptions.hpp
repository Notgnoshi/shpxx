#pragma once

#include <stdexcept>

namespace shpxx {

//! Base type for shpxx errors
class error_t : public std::runtime_error
{
  public:
    using std::runtime_error::runtime_error;
};

//! Indicates that the file data was not readable
class invalid_file_error_t : public error_t
{
  public:
    using error_t::error_t;
};

//! Indicates that an attempt was made to access features in the file as an
//! incorrect type
class incompatible_feature_type_error_t : public error_t
{
  public:
    using error_t::error_t;
};

}  // namespace shpxx
