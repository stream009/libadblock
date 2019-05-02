#include <adblock/database.hpp>

#include <cassert>
#include <cstring>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int
main()
{
    fs::path const project_root { PROJECT_ROOT };

    auto const& data_path = project_root / "benchmark/data/easylist.txt";
    if (!fs::exists(data_path)) {
        std::cerr << "data file doesn't exists: " << data_path << "\n";
        return -1;
    }

    adblock::database adb;
    adb.add_filter_list(data_path);

    //adb.print_statistics();
}
