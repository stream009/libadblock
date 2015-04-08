#ifndef ADBLOCK_TYPE_HPP
#define ADBLOCK_TYPE_HPP

#include <string>

#include <boost/range/iterator_range.hpp>
#include <boost/network/uri.hpp>

namespace adblock {

//using StringIterator = std::string::const_iterator;
using StringIterator = const char*;
using StringRange = boost::iterator_range<StringIterator>;
using Uri = boost::network::uri::uri;

} // namespace adblock

#endif //ADBLOCK_TYPE_HPP
