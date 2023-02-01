#define BOOST_TEST_MODULE mytests
#include <boost/test/included/unit_test.hpp>

#include "FileScannerTest.h"
#include "FileTest.h"

BOOST_AUTO_TEST_CASE(same_files_count1)
{
    std::string current_path_str = boost::filesystem::current_path().string() + "\\test_objects";

    std::vector<std::string> include = { current_path_str + "\\test" };
    std::vector<std::string> exclude = {};
    size_t level = 1;
    size_t min_size = 1;
    size_t block_size = 5;
    std::string hash = "crc16";
    std::vector<std::string> masks = {"*\.txt"};

    FileScannerTest fst(include, exclude, level, min_size, masks, block_size, hash);

    BOOST_TEST(fst.get_same_files_count(current_path_str + "\\test\\goodbye2.txt") == 3);
}

BOOST_AUTO_TEST_CASE(same_files_count2)
{
    std::string current_path_str = boost::filesystem::current_path().string() + "\\test_objects";

    std::vector<std::string> include = { current_path_str + "\\test" };
    std::vector<std::string> exclude = {};
    size_t level = 1;
    size_t min_size = 1;
    size_t block_size = 5;
    std::string hash = "crc16";
    std::vector<std::string> masks = { "*\.txt" };

    FileScannerTest fst(include, exclude, level, min_size, masks, block_size, hash);

    BOOST_TEST(fst.get_same_files_count(current_path_str + "\\test\\folder1\\data.txt") == 1);
}

BOOST_AUTO_TEST_CASE(same_files_count3)
{
    std::string current_path_str = boost::filesystem::current_path().string() + "\\test_objects";

    std::vector<std::string> include = { current_path_str + "\\test" };
    std::vector<std::string> exclude = {};
    size_t level = 1;
    size_t min_size = 1;
    size_t block_size = 5;
    std::string hash = "crc16";
    std::vector<std::string> masks = { "*\.txt" };

    FileScannerTest fst(include, exclude, level, min_size, masks, block_size, hash);

    BOOST_TEST(fst.get_same_files_count(current_path_str + "\\test\\some_text.txt") == 3);
}


BOOST_AUTO_TEST_CASE(hash_file_hello)
{
    auto current_path_str = boost::filesystem::current_path().string() + "\\test_objects";
    FileTest ft(current_path_str + "\\test\\hello.txt", 14, 5, "crc16");
    BOOST_TEST(ft.hash_block(0) == 0xF353);
    BOOST_TEST(ft.hash_block(1) == 0x0386);
    BOOST_TEST(ft.hash_block(2) == 0x2FBB);
}

BOOST_AUTO_TEST_CASE(hash_file_hello_2)
{
    auto current_path_str = boost::filesystem::current_path().string() + "\\test_objects";
    FileTest ft(current_path_str + "\\test\\hello2.txt", 14, 5, "crc16");
    BOOST_TEST(ft.hash_block(0) == 0xF353);
    BOOST_TEST(ft.hash_block(1) == 0x0386);
    BOOST_TEST(ft.hash_block(2) == 0x823B);
}

BOOST_AUTO_TEST_CASE(hash_file_goodbye)
{
    auto current_path_str = boost::filesystem::current_path().string() + "\\test_objects";
    FileTest ft(current_path_str + "\\test\\goodbye.txt", 7, 5, "crc16");
    BOOST_TEST(ft.hash_block(0) == 0x2F32);
    BOOST_TEST(ft.hash_block(1) == 0x1D97);
}


BOOST_AUTO_TEST_CASE(copies)
{
    auto current_path_str = boost::filesystem::current_path().string() + "\\test_objects";

    FileTest ft1(current_path_str + "\\test\\some_text.txt", 14, 5, "crc16");
    FileTest ft2(current_path_str + "\\test\\folder2\\some_other_text.txt", 14, 5, "crc16");


    BOOST_TEST(ft1.hash_block(0) == ft2.hash_block(0));
    BOOST_TEST(ft1.hash_block(1) == ft2.hash_block(1));
    BOOST_TEST(ft1.hash_block(2) == ft2.hash_block(2));
}


BOOST_AUTO_TEST_CASE(not_copies1)
{
    auto current_path_str = boost::filesystem::current_path().string() + "\\test_objects";

    FileTest ft1(current_path_str + "\\test\\hello.txt", 14, 5, "crc16");
    FileTest ft2(current_path_str + "\\test\\goodbye.txt", 7, 5, "crc16");

    BOOST_TEST(ft1.hash_block(0) != ft2.hash_block(0));
}

BOOST_AUTO_TEST_CASE(not_copies2)
{
    auto current_path_str = boost::filesystem::current_path().string() + "\\test_objects";

    FileTest ft1(current_path_str + "\\test\\hello.txt", 14, 5, "crc16");
    FileTest ft2(current_path_str + "\\test\\hello2.txt", 14, 5, "crc16");

    BOOST_TEST(ft1.hash_block(0) == ft2.hash_block(0));
    BOOST_TEST(ft1.hash_block(1) == ft2.hash_block(1));
    BOOST_TEST(ft1.hash_block(2) != ft2.hash_block(2));
}
