#ifndef ADBLOCK_CORE_URI_HPP
#define ADBLOCK_CORE_URI_HPP

#include "uri/uri.hpp"

namespace adblock {

class Uri : public uri::uri
{
    using Base = typename uri::uri;
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
