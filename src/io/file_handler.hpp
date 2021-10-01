#pragma once

#include "io/io_handler_concept.hpp"

namespace shpxx::io {

//! @brief IoHandler which reads/writes files
class file_handler_t
{
  public:
    static handle_t open(std::string_view filename, std::ios::openmode mode);
    static bool close(handle_t handle);
    static int remove(std::string_view filename);

    static offset_t read(handle_t handle, uint8_t* buffer, offset_t length);
    static offset_t write(handle_t handle, const uint8_t* buffer, offset_t length);
    static bool flush(handle_t handle);

    static int seek(handle_t handle, offset_t offset, std::ios::seekdir dir);
    static offset_t tell(handle_t handle);
};
}  // namespace shpxx::io
