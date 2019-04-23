#include "file.hpp"

#include <cassert>
#include <fstream>
#include <sstream>

namespace adblock {

File::
File(Path path)
    : m_path { std::move(path) }
{
    assert(!m_path.empty());

    reload();
}

File::~File() = default;

void File::
reload()
{
    std::ostringstream buf;
    std::ifstream input { m_path.c_str() };
    if (!input.is_open()) throw std::ifstream::failure("Can't open file");

    buf << input.rdbuf();

    m_data = std::move(buf.str());
}


} // namespace adblock
