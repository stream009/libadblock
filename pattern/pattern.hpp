#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <string>

#include <boost/network/uri/uri.hpp>
#include <boost/range/iterator_range.hpp>

class Pattern
{
protected:
    using String = std::string;
    using Range = boost::iterator_range<String::const_iterator>;
    using Uri = boost::network::uri::uri;

public:
    virtual ~Pattern() = default;

    virtual bool match(const Uri &target) const = 0;
};

#endif // PATTERN_HPP
