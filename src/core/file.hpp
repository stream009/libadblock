#ifndef ADBLOCK_FILE_HPP
#define ADBLOCK_FILE_HPP

#include <cstdint>
#include <filesystem>
#include <string>

namespace adblock {

class File
{
    using Path = std::filesystem::path;
public:
    File(Path);
    ~File();

    // accessor
    Path const& path() const { return m_path; }

    // query
    char const* data() const { return m_data.data(); }
    size_t size() const { return m_data.size(); }

    // command
    void reload();

private:
    Path m_path;
    std::string m_data;
};

} // namespace adblock

#endif // ADBLOCK_FILE_HPP
