#ifndef ADBLOCK_PARSE_ERROR_HPP
#define ADBLOCK_PARSE_ERROR_HPP

#include "core/string_range.hpp"

#include <string>

namespace adblock {

struct ParseError
{
    int64_t line_no;
    int64_t from;
    int64_t to;
    StringRange line;
    std::string message;
};

} // namespace adblock

#endif // ADBLOCK_PARSE_ERROR_HPP
