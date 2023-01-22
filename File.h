#pragma once
#include <boost/filesystem.hpp>
#include <boost/crc.hpp>

#include <fstream> 

class MyFile
{
public:
	MyFile(const std::string& _path, uintmax_t _size, size_t _block_size, const std::string& _hash)
		: path(_path), fsize(_size), block_size(_block_size), hash(_hash)
	{
		file.open(path, std::fstream::in);
	}

	const size_t get_block_count() const
	{
		return static_cast<size_t>(ceil((float)fsize / block_size));
	}

	const std::string& get_path() const
	{
		return path;
	}

	const uint32_t get_hashed_block(size_t num)
	{
		if (num < hashed_blocks.size())
		{
			return get_block(num);
		}
		else
		{
			return read_block(num);
		}
	}

private:

	const uint32_t get_block(size_t num) const
	{
		return hashed_blocks[num];
	}

	const uint32_t read_block(size_t num)
	{
		std::shared_ptr<char> block = std::make_shared<char>();

		size_t cnt = 0;
		char ch;
		while (cnt < block_size && file.get(ch))
		{
			block.get()[cnt++] = ch;
		}

		if (cnt < block_size)
		{
			for (auto i=cnt; i<block_size; ++i)
				block.get()[i] = '0';
		}

		auto hash_value = hash_block(block);

		hashed_blocks.emplace_back(hash_value);

		return hash_value;
	}

	const uint32_t hash_block(std::shared_ptr<char> block) const
	{
		if (hash == "crc16")
			return get_CRC<boost::crc_16_type>(block);
		if (hash == "crc32")
			return get_CRC<boost::crc_32_type>(block);
		if (hash == "ccitt")
			return get_CRC<boost::crc_ccitt_type>(block);
		else if (hash == "xmodem")
			return get_CRC<boost::crc_xmodem_type>(block);
		else
			return 0;
	}

	template<typename T>
	const uint32_t get_CRC(std::shared_ptr<char> data) const
	{
		T result{};
		result.process_bytes(data.get(), block_size);
		return result.checksum();
	}

	const std::string path;
	const uintmax_t fsize;
	const size_t block_size;
	const std::string hash;

	std::ifstream file;
	std::vector<uint32_t> hashed_blocks;
};