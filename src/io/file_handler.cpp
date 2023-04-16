#include "file_handler.hpp"

#include "io/openmode_internal.hpp"
#include "shpxx/io/openmode.hpp"

#include <fstream>

namespace shpxx::io {

std::fstream* to_stream(handle_t handle)
{
    return reinterpret_cast<std::fstream*>(handle);  // NOLINT
}

handle_t file_handler_t::open(std::string_view filename, std::ios::openmode mode)
{
    std::fstream file(filename.data(), mode);
    if (!file)
    {
        return nullptr;
    }
    return new std::fstream(std::move(file));
}

bool file_handler_t::close(handle_t handle)
{
    auto* stream = to_stream(handle);
    stream->close();
    delete stream;  // NOLINT
    return true;
}

int file_handler_t::remove(std::string_view filename) { return std::remove(filename.data()); }

offset_t file_handler_t::read(handle_t handle, uint8_t* buffer, offset_t length)
{
    auto* stream = to_stream(handle);
    stream->read(reinterpret_cast<char*>(buffer), length);  // NOLINT
    return stream->gcount();
}

offset_t file_handler_t::write(handle_t handle, const uint8_t* buffer, offset_t length)
{
    auto* stream = to_stream(handle);
    const auto start_point = stream->tellp();
    stream->write(reinterpret_cast<const char*>(buffer), length);  // NOLINT
    return stream->tellp() - start_point;
}

bool file_handler_t::flush(handle_t handle)
{
    auto* stream = to_stream(handle);
    stream->flush();
    return !stream->fail();
}

int file_handler_t::seek(handle_t handle, offset_t offset, std::ios::seekdir dir)
{
    auto* stream = to_stream(handle);

    // std::fstream uses the same position indicator for both put and get
    // so we can use either/or for these operations
    stream->seekg(offset, dir);  // NOLINT

    return stream->fail() ? 1 : 0;
}

offset_t file_handler_t::tell(handle_t handle)
{
    auto* stream = to_stream(handle);

    // std::fstream uses the same position indicator for both put and get
    // so we can use either/or for these operations
    return stream->tellg();
}

}  // namespace shpxx::io
