#pragma once
#include "File.h"

class FileTest
{
public:
	FileTest(const std::string& path, uintmax_t size, size_t block_size, const std::string& hash)
	{
		mfile = std::make_unique<MyFile>(path, size, block_size, hash);
	}

	uint32_t hash_block( size_t num)
	{
		return mfile.get()->get_hashed_block(num);
	}

private:
	std::shared_ptr<MyFile> mfile;
};