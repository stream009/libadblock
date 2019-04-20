#ifndef ADBLOCK_CORE_STRING_RANGE_HPP
#define ADBLOCK_CORE_STRING_RANGE_HPP

#include <cassert>
#include <string_view>

namespace adblock {

class StringRange : public std::string_view
{
    using Base = std::string_view;
public:
    StringRange() = default;

    StringRange(char const* begin, char const* end)
        : Base { begin, static_cast<size_t>(end - begin) }
    {
        assert(begin <= end);
    }

    StringRange(std::string_view const other)
        : Base { other }
    {}

    using Base::Base;
    using Base::operator=;
};

inline StringRange
operator"" _r(char const* str, size_t const len)
{
    return { str, len };
}

} // namespace adblock

#endif // ADBLOCK_CORE_STRING_RANGE_HPP
