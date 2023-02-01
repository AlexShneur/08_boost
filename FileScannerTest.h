#pragma once
#include "FileScanner.h"

class FileScannerTest
{
public:
    FileScannerTest(const std::vector<std::string>& include, const std::vector<std::string>& exclude, size_t level, size_t min_size, const std::vector<std::string>& masks, size_t block_size, const std::string& hash)
    {
        fscan = std::make_unique<FileScanner>(include, exclude, level, min_size, masks, block_size, hash);
    }


    size_t get_same_files_count(const std::string& file_path)
    {
        fscan.get()->find_files();
        fscan.get()->find_copies();

        for (const auto& [file, copies] : fscan.get()->file_to_copies)
        {
            if (file->get_path() == file_path || (copies.find(file_path) != copies.end()))
            {
                return copies.size() > 0 ? copies.size() + 1 : 1;
            }
        }
        return 0;
    }

private:
    std::unique_ptr<FileScanner> fscan;
};