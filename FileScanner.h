#pragma once
#include "File.h"

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <boost\regex.hpp>
namespace fs = boost::filesystem;

class FileScanner
{
public:

    friend class FileScannerTest;

    FileScanner(const std::vector<std::string>& _inc, const std::vector<std::string>& _exc, size_t _level, size_t _min_size, const std::vector<std::string>& _masks,
        size_t _block_size, const std::string& _hash)
        : include_dirs(_inc), exclude_dirs(_exc), level(_level), min_size(_min_size), masks(_masks), block_size(_block_size), hash(_hash)
    {

    }

    void scan()
    {
        find_files();
        find_copies();
        print_results();
    }

private:

    void find_files()
    {
        for (const auto& dir : include_dirs)
        {
            fs::path path(dir);

            if (!fs::exists(dir))
            {
                std::cout << std::endl << "Directory " << dir << " is not found " << std::endl;
            }
            else
            {
                fs::recursive_directory_iterator end_iter;
                for (fs::recursive_directory_iterator dir_itr(dir);
                    dir_itr != end_iter;)
                {
                    try
                    {
                        auto cur_obj_path = dir_itr->path().string();
                        if (level)
                        {
                            if (std::find(exclude_dirs.begin(), exclude_dirs.end(), cur_obj_path) == exclude_dirs.end())
                            {
                                add_file_object(dir_itr);
                            }
                            else
                                dir_itr.no_push();
                        }
                        else
                        {
                            if (!add_file_object(dir_itr))
                                dir_itr.no_push();
                        }

                        boost::system::error_code ec;
                        dir_itr.increment(ec);
                        if (ec)
                        {
                            std::cout << "*" << std::move(cur_obj_path) << " *" << ec.message() << std::endl;
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        std::cout << dir_itr->path() << " " << ex.what() << std::endl;
                    }
                }
            }
        }
    }

    void find_copies()
    {
        std::vector<std::shared_ptr<MyFile>> new_files;
        bool is_changed = false;
        do
        {
            is_changed = false;
            new_files.clear();
            for (const auto& [fsize, files] : fsize_to_files)
            {
                auto first_file_It = files.begin();
                auto& first_file = *(first_file_It);
                std::set<std::string> copies;
                for (auto it = std::next(first_file_It); it != files.end(); ++it)
                {
                    bool isCopy = true;
                    auto& file = *(it);
                    size_t blocks_count = file->get_block_count();
                    for (size_t i = 0; i < blocks_count; ++i)
                    {
                        bool first_file_block_written = false;
                        uint32_t first_hash;
                        if (!first_file_block_written)
                        {
                            first_hash = first_file.get()->get_hashed_block(i);
                            first_file_block_written = true;
                        }

                        auto hash = file.get()->get_hashed_block(i);
                        if (hash != first_hash)
                        {
                            isCopy = false;
                            new_files.push_back(file);
                            break;
                        }
                    }
                    if (isCopy)
                        copies.insert(file->get_path());
                }
                file_to_copies[first_file] = std::move(copies);

                if (new_files.empty())
                {
                    ++first_file_It;
                }
                else
                {
                    fsize_to_files[fsize] = new_files;
                    is_changed = true;
                }
            }

        } while (is_changed);
    }

    void print_results() const
    {
        for (const auto& [file, copies] : file_to_copies)
        {
            std::cout << std::endl << std::endl << file->get_path() << std::endl;
            for (const auto& cp : copies)
            {
                std::cout << cp << std::endl;
            }
            size_t blocks_count = file->get_block_count();
            for (size_t i = 0; i < blocks_count; ++i)
            {
                std::cout << std::hex << file.get()->get_hashed_block(i) << " ";
            }
        }
    }

    bool add_file_object(fs::recursive_directory_iterator dir_itr)
    {
        if (fs::is_regular_file(dir_itr->status()))
        {
            auto cur_obj_path = dir_itr->path().string();

            auto fsize = fs::file_size(cur_obj_path);
            if (fsize > min_size)
            {
                if (!masks.empty())
                {
                    for (const auto& mask : masks)
                    {
                        boost::smatch what;
                        const boost::regex filter(dir_itr->path().stem().string()+"."+mask);
                        if (boost::regex_match(dir_itr->path().filename().string(), what, filter))
                        {
                            fsize_to_files[fsize].emplace_back(std::make_shared<MyFile>(cur_obj_path, fsize, block_size, hash));
                            return true;
                        }
                    }
                }
                else
                {
                    fsize_to_files[fsize].emplace_back(std::make_shared<MyFile>(cur_obj_path, fsize, block_size, hash));
                    return true;
                }
            }
        }
        return false;
    }

    const std::vector<std::string> include_dirs;
    const std::vector<std::string> exclude_dirs;
    const size_t level;
    const size_t min_size;
    const std::vector<std::string> masks;
    const size_t block_size;
    const std::string hash;

    std::map<uintmax_t, std::vector<std::shared_ptr<MyFile>>> fsize_to_files;
    std::map<std::shared_ptr<MyFile>, std::set<std::string>> file_to_copies;
};