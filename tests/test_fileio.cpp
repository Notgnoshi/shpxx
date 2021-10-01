#include "io/file_handler.hpp"
#include "io/io_handler_concept.hpp"

#include <string_view>

#include <gtest/gtest.h>

using io_handlers = ::testing::Types<shpxx::io::file_handler_t>;

template<shpxx::io::IoHandler HandlerT>
class io_handler_test_t : public ::testing::Test
{
};
TYPED_TEST_SUITE_P(io_handler_test_t);

TYPED_TEST_P(io_handler_test_t, FileOperations)
{
    // in/out will not create files unless trunc is also specified
    const shpxx::io::handle_t file_handle = TypeParam::open(
        "test_file", std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

    // Open shouldn't fail
    ASSERT_NE(file_handle, nullptr);

    // Should open at start of file
    EXPECT_EQ(TypeParam::tell(file_handle), 0);

    // Should open empty file
    EXPECT_EQ(TypeParam::seek(file_handle, 0, std::ios::end), 0);
    EXPECT_EQ(TypeParam::tell(file_handle), 0);

    // Write some data
    uint8_t data_out[] = "abcdefghijklmnop";
    EXPECT_EQ(TypeParam::write(file_handle, data_out, sizeof(data_out)), sizeof(data_out));
    EXPECT_EQ(TypeParam::tell(file_handle), sizeof(data_out));
    EXPECT_TRUE(TypeParam::flush(file_handle));

    // Read some data
    uint8_t data_in[16] = {0};
    EXPECT_EQ(TypeParam::seek(file_handle, 8, std::ios::beg), 0);
    EXPECT_EQ(TypeParam::tell(file_handle), 8);

    EXPECT_EQ(TypeParam::read(file_handle, data_in, 8), 8);
    EXPECT_EQ(std::char_traits<uint8_t>::length(data_in), 8);
    EXPECT_TRUE(std::equal(data_in, data_in + 8, data_out + 8))
        << (data_in + 8) << " != " << data_out;

    EXPECT_TRUE(TypeParam::close(file_handle));

    // Open the file again
    const shpxx::io::handle_t file_handle_2 =
        TypeParam::open("test_file", std::ios::binary | std::ios::in);

    // Open shouldn't fail
    ASSERT_NE(file_handle_2, nullptr);

    EXPECT_EQ(TypeParam::seek(file_handle_2, 0, std::ios::end), 0);
    EXPECT_EQ(TypeParam::tell(file_handle_2), sizeof(data_out));

    EXPECT_TRUE(TypeParam::close(file_handle_2));

    // Delete the file
    EXPECT_EQ(TypeParam::remove("test_file"), 0);

    // Opening without create should fail
    const shpxx::io::handle_t file_handle_3 =
        TypeParam::open("test_file", std::ios::binary | std::ios::in | std::ios::out);

    EXPECT_EQ(file_handle_3, nullptr);
}

REGISTER_TYPED_TEST_SUITE_P(io_handler_test_t, FileOperations);
INSTANTIATE_TYPED_TEST_SUITE_P(IoHandling, io_handler_test_t, io_handlers);
