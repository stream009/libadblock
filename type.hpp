#ifndef ADBLOCK_TYPE_HPP
#define ADBLOCK_TYPE_HPP

#include "uri/uri.hpp"

#include <string>

#include <boost/range/iterator_range.hpp>

namespace adblock {

using Uri = uri::uri;
using StringIterator = const char*;
using StringRange = boost::iterator_range<StringIterator>;

inline StringRange
operator"" _r(const char *str, const size_t len)
{
    return { str, str + len };
}

inline Uri
operator"" _u(const char *str, const size_t len)
{
    return { str, len };
}

} // namespace adblock

#endif //ADBLOCK_TYPE_HPP
