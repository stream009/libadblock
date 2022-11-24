#ifndef ADBLOCK_CORE_URI_HPP
#define ADBLOCK_CORE_URI_HPP

#include <stream9/url.hpp>

namespace adblock {

class Uri : public stream9::url
{
    using Base = stream9::url;
public:
    using Base::Base;
    using Base::operator=;
};

inline Uri
operator"" _u(char const* str, size_t const len)
{
    return { str, len };
}

} // namespace adblock

#endif //ADBLOCK_CORE_URI_HPP
