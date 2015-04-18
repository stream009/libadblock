#ifndef ADBLOCK_TYPE_HPP
#define ADBLOCK_TYPE_HPP

#include <string>

#include <boost/range/iterator_range.hpp>
#include <boost/network/uri.hpp>

namespace adblock {

using StringIterator = const char*;
using StringRange = boost::iterator_range<StringIterator>;
using Uri = boost::network::uri::uri;

inline StringRange
operator"" _r(const char *str, const size_t len)
{
    return { str, str + len };
}

inline Uri
operator"" _u(const char *str, const size_t len)
{
    return { str, str + len };
}

} // namespace adblock

#endif //ADBLOCK_TYPE_HPP
