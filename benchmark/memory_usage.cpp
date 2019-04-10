#include <adblock/api.h>

#include <cassert>
#include <cstring>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class adblock_db
{
public:
    adblock_db()
    {
        m_handle = ::adblock_create();
        assert(m_handle);
    }

    ~adblock_db()
    {
        ::adblock_destroy(m_handle);
    }

    void add_filter_list(fs::path const& data_path)
    {
        ::adblock_add_filter_set(
            m_handle,
            data_path.c_str(), std::strlen(data_path.c_str())
        );
    }

    void print_statistics() const
    {
        char const* json = nullptr;
        size_t json_len = 0;

        ::adblock_statistics(m_handle, &json, &json_len);

        std::cout << json << "\n";
    }

private:
    ::adblock_t m_handle;
};

int
main()
{
    fs::path const project_root { PROJECT_ROOT };

    auto const& data_path = project_root / "benchmark/data/easylist.txt";
    if (!fs::exists(data_path)) {
        std::cerr << "data file doesn't exists: " << data_path << "\n";
        return -1;
    }

    adblock_db adb;
    adb.add_filter_list(data_path);

    //adb.print_statistics();
}
