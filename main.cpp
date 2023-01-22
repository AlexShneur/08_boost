#include "FileScanner.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace std
{
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
    {
        for (auto item : vec)
        {
            os << item << " ";
        }
        return os;
    }
}

int main(int argc, const char* argv[])
{
    try
    {
        std::vector<std::string> default_directory_path;
        default_directory_path.push_back(fs::current_path().string());
        po::options_description desc{ "Options" };
        desc.add_options()
            ("help,h", "help message")
            ("include", po::value<std::vector<std::string>>()->multitoken()->default_value(std::move(default_directory_path)), "directories to scan")
            ("exclude", po::value<std::vector<std::string>>()->multitoken()->default_value({}), "directories to exclude from scanning")
            ("level", po::value<size_t>()->default_value(1), "scanning level")
            ("min_size", po::value<size_t>()->default_value(1), "minimum file size")
            ("masks", po::value<std::vector<std::string>>()->multitoken()->default_value({}), "file name masks allowed for comparison")
            ("block_size", po::value<size_t>()->default_value(5), "file read block size")
            ("hash", po::value<std::string>()->default_value("crc16"), "hash algorithm");

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        std::unique_ptr<FileScanner> fscan = std::make_unique<FileScanner>(vm["include"].as<std::vector<std::string>>(), vm["exclude"].as<std::vector<std::string>>(),
            vm["level"].as<size_t>(), vm["min_size"].as<size_t>(), vm["masks"].as<std::vector<std::string>>(), vm["block_size"].as<size_t>(),
            vm["hash"].as<std::string>());

        fscan->scan();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}