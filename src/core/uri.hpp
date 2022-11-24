#ifndef ADBLOCK_CORE_URI_HPP
#define ADBLOCK_CORE_URI_HPP

#include "url/url.hpp"
#include "url/error.hpp"

namespace adblock {

class Uri : public urls::url
{
    using Base = urls::url;
public:
    using Base::Base;
    using Base::operator=;
};

using uri_error = urls::error;

inline Uri
operator"" _u(char const* str, size_t const len)
{
    return { str, len };
}

} // namespace adblock

#endif //ADBLOCK_CORE_URI_HPP
